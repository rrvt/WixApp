// Web folder -- Content of one directory at the web site


#include "pch.h"
#include "WebFiles.h"
#include "LexT.h"
#include "MainFrame.h"
#include "NotePad.h"
#include "Resource.h"
#include "SendMsg.h"
#include "SftpDataIter.h"
#include "SFTPDoc.h"
#include "SftpSSL.h"
#include "SiteFileDscs.h"
#include "SiteID.h"
#include "StringInput.h"
#include "Utilities.h"
#include "WorkerThrd.h"

static UINT webFilesThrd(void* param);

typedef LexT<StringInput, true, false, false> Lex;


WebFiles webFiles;


void WebNode::clear() {
WebNodeIter iterWN(*this);
WebItem*    item;

  for (item = iterWN(); item; item = iterWN++) {
    if (item->typ == WebDirType) {NewAlloc(WebNode);   FreeNode(item->node);   item->node = 0;}
    }

  data.clear();
  }


// Start Thread to load web directory into rootNode

void WebFiles::start() {

  if (isLocked()) return;

  notePad.clear();

  if (!siteID.login()) return;

  mainFrm()->startPrgBar(curFileDscs.nData());

  workerThrd.start(webFilesThrd, 0, ID_FinWebFilesMsg);
  }

bool WebFiles::isLocked(bool prevent) {return doc()->isLocked(prevent);}


UINT webFilesThrd(void* param) {

  if (!webFiles.load(siteID.remoteRoot)) return false;

  return 0;
  }


bool WebFiles::load(TCchar* root) {

  if (loaded) return true;

  clear();   if (!loadOne(root, rootNode)) return false;

  loaded = rootNode.nData() > 0; return loaded;
  }


bool WebFiles::loadOne(TCchar* path, WebNode& webNode) {
String       pth = path;
SftpDataIter iter(sftpSSL.fileData());
String*      line;

  sftpSSL.list(path);    webNode.path = fixRemotePath(pth);

  indent += 3;   notePad << nClrTabs << nSetTab(indent) << nSetTab(indent + 30);

  for (line = iter(); line; line = iter++) {
    WebItem item;   if (parse(*line, webNode.path, item)) webNode.data = item;

    if (item.typ == WebFileType) sendStepPrgBar();
    }

  WebNodeIter iterWN(webNode);
  WebItem*    item;

  for (item = iterWN(); item; item = iterWN++) {
    if (item->typ == WebDirType)
         {NewAlloc(WebNode);   item->node = AllocNode;   loadOne(webNode.path + item->name, *item->node);}
    }

  indent -= 3;   return webNode.nData() > 0;
  }



bool WebFiles::parse(String& line, TCchar* path, WebItem& item) {
Lex    lex;
Token* t;
int    i;
int    noWhite;
bool   collect;
String attr;
String name;

  lex.initialize(); lex.input.set(line);

  for (i = 0, noWhite = 0, collect = false; lex.get_token() != EOFToken; i++) {
    t = lex.token;

    if (!i) attr = t->name;

    if (collect)         {name += t->name;  lex.accept_token();   continue;}

    if (t->code == WhiteToken) {noWhite++;   lex.accept_token();   continue;}

    if (noWhite < 8)                        {lex.accept_token();   continue;}

    if (t->code == PeriodToken || t->name[0] == _T('_')) break;

    name = t->name;    collect = true;   lex.accept_token();
    }

  if (!collect) return false;

  item.typ  = attr[0] == _T('d') ? WebDirType : WebFileType;
  item.name = name;
  if (item.typ == WebFileType) sftpSSL.size(path + item.name, item.size);

  return true;
  }


// Finish the Thread and display the directory

LRESULT WebFiles::finishMsg(WPARAM wparam, LPARAM lParam) {

  mainFrm()->closePrgBar();   webFiles.display();   return 0;
  }


void WebFiles::display() {

  notePad << nBeginLine << rootNode.path << nEndLine << nCrlf;

  displayOne(rootNode, 1);
  }


void WebFiles::displayOne(WebNode& webNode, int tab) {
WebNodeIter iter(webNode);
WebItem*    item;
int         indent = tab * 3;

  notePad << nClrTabs << nSetTab(indent) << nSetRTab(45);

  for (item = iter(); item; item = iter++) {
    if (item->typ == WebFileType) {
      notePad << nTab << item->name << nTab << item->size << nCrlf;
      }
    }

  for (item = iter(); item; item = iter++) {
    if (item->typ == WebDirType) {

      WebNode& node = *item->node;

      notePad << nClrTabs << nSetTab(indent) << nSetRTab(45);
      notePad << nCrlf << nTab << nBeginLine << node.path << nEndLine << nCrlf;

      displayOne(node, tab+1);
      }
    }
  }


WebItem::~WebItem() {if (node) {NewAlloc(WebNode);   FreeNode(node);}}


void WebItem::copy(WebItem& wi) {
  typ = wi.typ; name = wi.name; size = wi.size;

  if (!wi.node) return;

  NewAlloc(WebNode);   node = AllocNode;   *node = *wi.node;
  }

