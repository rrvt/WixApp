// Prepare Upload for update command


#include "pch.h"
#include "PrepWebCmprCmd.h"
#include "NotePad.h"
#include "SFTP.h"
#include "SFTPDoc.h"
#include "SiteFileDscs.h"
#include "WebFiles.h"
#include "WorkerThrd.h"


static UINT prepUploadThrd(void* param);


PrepWebCmprCmd prepWebCmprCmd;


void PrepWebCmprCmd::start() {

  if (isLocked()) return;

  notePad.clear();   doc()->setNoFiles(0);

  if (!siteID.login()) return;

  curFileDscs.setCheck();   curFileDscs.updateFromPC();

  mainFrm()->startPrgBar(curFileDscs.nData());

  workerThrd.start(prepUploadThrd, (void*) &webFiles.root(), ID_WebCmprMsg);
  }


bool PrepWebCmprCmd::isLocked(bool prevent) {return doc()->isLocked(prevent);}


UINT prepUploadThrd(void* param) {

  sendWdwScroll();

  if (!webFiles.load(siteID.remoteRoot)) return false;

  prepWebCmprCmd.setChecks();

  prepWebCmprCmd.setCurFileDscs(*(WebNode*) param);

  return 0;
  }


void PrepWebCmprCmd::setChecks() {
FileDscsIter iter(curFileDscs);
SiteFileDsc* dsc;

  for (dsc = iter(); dsc; dsc = iter++) if (!dsc->check) {dsc->check = true; dsc->status = OthSts;}
  }


void PrepWebCmprCmd::setCurFileDscs(WebNode& node) {
String       path = node.path;
WebNodeIter  iter(node);
WebItem*     item;
SiteFileDsc* dsc;

  setCurFileDir(path);

  for (item = iter(); item; item = iter++) if (item->typ == WebFileType) {
    String      s = path + item->name;
    SiteFileKey key;

    key.path = siteID.toRelative(s);   dsc = curFileDscs.find(key);

    if (!dsc) {
      dsc = curFileDscs.addFile(s);   if (dsc) {dsc->check = true;   dsc->status = GetSts;}

      continue;
      }

    if (dsc->status == OthSts) {dsc->check = false;   dsc->status = NilSts;}
    }

  for (item = iter(); item; item = iter++) if (item->typ == WebDirType) setCurFileDscs(*item->node);
  }


void PrepWebCmprCmd::setCurFileDir(TCchar* webDirPath) {
String       path = webDirPath;       siteID.webToRelative(path);
SiteFileKey  key;
SiteFileDsc* dsc;

  if (path.isEmpty()) return;

  key.dir = true;   key.path = path;

  dsc = curFileDscs.find(key);

  if (!dsc) {

    dsc = curFileDscs.addDir(path);   if (!dsc) return;

    dsc->check = true;   dsc->status = GetSts;   return;
    }

  if (dsc->status == OthSts) {dsc->check = false;   dsc->status = NilSts;}
  }


LRESULT PrepWebCmprCmd::finishMsg(WPARAM wparam, LPARAM lParam) {
String path;

  mainFrm()->closePrgBar();

  display();   doc()->setNoFiles(noFiles);

  sendDisplayMsg();   Sleep(10);   sendWdwScroll(false);   return 0;
  }


void PrepWebCmprCmd::display() {
FileDscsIter iter(curFileDscs);
SiteFileDsc* dsc;

  notePad << nClrTabs << nSetTab(15);

  for (dsc = iter(), noFiles = 0; dsc; dsc = iter++) if (dsc->check) {

    switch (dsc->status) {
      case OthSts   : dsc->status = WebPutSts;
      case DifPutSts:
      case WebPutSts: if (dsc->key.dir) notePad << _T("Create Web Dir:");
                      else              notePad << _T("Upload:");
                      break;
      case GetSts   : if (dsc->key.dir) notePad << _T("Create Local Dir:");
                      else              notePad << _T("Download:");
                      break;
      case DelSts   : notePad << _T("Delete:");    break;
      default       : notePad << _T("Unknown:");   break;
      }

    notePad << nTab << dsc->key.path << nCrlf;   noFiles++;   sendDisplayMsg();
    }

  if      (noFiles <= 0) notePad << _T("No entities");
  else if (noFiles == 1) notePad << noFiles << _T(" entity");
  else                   notePad << noFiles << _T(" entities");

  notePad << _T(" marked for up/down load because they are absent on the web or in the local files");
  notePad << nCrlf;
  }



#if 0
doc()->debugDsp(_T("Before SiteFileDscs::updateFromPC"));

    curFileDscs.updateFromPC();

doc()->debugDsp(_T("After SiteFileDscs::updateFromPC"));
#endif
//curFileDscs.logSelected(_T("Before Prep Web Cmpr"));
//curFileDscs.logSelected(_T("After Web UpdateFromPC"));

