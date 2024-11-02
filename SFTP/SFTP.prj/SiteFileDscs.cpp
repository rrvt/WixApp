

#include "pch.h"
#include "SiteFileDscs.h"
#include "Archive.h"
#include "CSVLex.h"
#include "CSVOut.h"
#include "DirList.h"
#include "Expandable.h"
#include "FileName.h"
#include "FileSrch.h"
#include "SFTP.h"
#include "Resource.h"
#include "SftpDoc.h"
#include "SftpLog.h"
#include "SftpSSL.h"
#include "SftpDataIter.h"
#include "SftpTransport.h"
#include "SiteID.h"
#include "Utilities.h"
#include "WorkerThrd.h"


static TCchar* Version = _T("1");

SiteFileDscs prvFileDscs;
SiteFileDscs curFileDscs;


void SiteFileDscs::copy(SiteFileDscs& sf) {
FileDscsIter iter(sf);
SiteFileDsc* dsc;

  clear();   root = sf.root;   rootLng = sf.rootLng;   loaded = sf.loaded;

  for (dsc = iter(); dsc; dsc = iter++) {SiteFileDsc* p = add(*dsc);   p->clrSts();}
  }



void SiteFileDscs::setCheck() {
FileDscsIter iter(*this);
SiteFileDsc* dsc;

  for (dsc = iter(); dsc; dsc = iter++)
    if (dsc->status != NilSts)
      dsc->check = true;
  }


bool SiteFileDscs::updateFromPC() {
SiteFileDscs newDscs;
FileDscsIter iter(newDscs);
SiteFileDsc* dsc;
SiteFileDsc* p;

  if (!newDscs.loadFromPC()) return false;

  checkForDel(newDscs);

  for (dsc = iter(); dsc; dsc = iter++) {
    SiteFileKey& key  = dsc->key;
    String       path = siteID.localRoot + key.path;

    p = find(key);   if (!p) {add(key);   continue;}

    if (!key.dir) p->addLclAttr(path);
    }

  return true;
  }


void SiteFileDscs::checkForDel(SiteFileDscs& newDscs) {
FileDscsIter iter(*this);
SiteFileDsc* dsc;

  for (dsc = iter(); dsc; dsc = iter++) {

    if (dsc->updated) {
      if (dsc->status == DelSts) {iter.remove();   continue;}

      if (dsc->status == DifPutSts) dsc->clrSts();
      }

    if (!newDscs.find(dsc->key)) iter.remove();
    }
  }



bool SiteFileDscs::loadFromPC() {
String& path = siteID.localRoot;

  if (path.isEmpty()) return false;

  clear();   setRoot(path);   loadOneDir(path);

  return !isEmpty();
  }


void SiteFileDscs::loadOneDir(TCchar* path) {
Expandable<String, 2> data;
FileSrch              srch;
String                sub;
String                file;
int                   i;
int                   n;

  addDir(path);

  if (srch.findAllSubDirs(path)) while (srch.getName(sub)) data = sub;

  if (srch.findAllFiles(path)) while (srch.getName(file)) addFile(file);

  for (i = 0, n = data.end(); i < n; i++) loadOneDir(data[i]);

  data.clear();
  }



SiteFileDsc* SiteFileDscs::add(SiteFileKey& key)
            {String path = siteID.localRoot + key.path;   return key.dir ? addDir(path) : addFile(path);}


SiteFileDsc* SiteFileDscs::addFile(TCchar* path) {
String       pth = path;
SiteFileDsc* sf;
FileIO       lu;
CTime        time;

  if (filterFile(path)) return 0;

  sf  = data.allocate();

  if (pth.find(_T('/')) >= 0) {sftpSSL.size(pth, sf->size);   sftpSSL.date(pth, sf->date);}
  else                         sf->addLclAttr(pth);

  sf->key.path = siteID.toRelative(pth);   sf->name = removePath(pth);   data = sf;   return sf;
  }


SiteFileDsc* SiteFileDscs::addDir(TCchar* path) {
String       pth = path;
SiteFileKey  key;
SiteFileDsc* sf;
int          pos;

  if (pth.find(root) == 0) pth = toLocalPath(pth.substr(rootLng));   if (pth.isEmpty()) return 0;

  if (filterFile(pth)) return 0;

  key.dir = true;   key.path = pth;

  sf = find(key);   if (sf) return sf;

  sf = data.allocate();   sf->key = key;

  pos = pth.length() - 1;   if (pth[pos] == _T('\\')) pth = pth.substr(0, pos);

  sf->name = removePath(pth) + _T('\\');

  data = sf;   return sf;
  }


void SiteFileDsc::addLclAttr(TCchar* path) {
FileIO lu;
CTime  time;

  if (lu.open(path, FileIO::Read)) {

    size = lu.getLength();
    if (!lu.getModifiedTime(time)) time = CTime::GetCurrentTime();

    lu.close();
    }

  else {size = 1; time = CTime::GetCurrentTime();}

  date = time;
  }



void SiteFileDscs::update(SiteFileDsc& uf) {
SiteFileDsc* sf = find(uf.key);

  if (sf) {sf ->size = uf.size; sf->date = uf.date;}
  else     data = uf;
  }



static TCchar* filters[] = {_T("_notes\\"), _T("Junk\\"),   _T(".log"),   _T("tmp\\"),  _T(".vpj"),
                            _T(".vpw"),     _T(".vpwhist"), _T(".vtg"),   _T(".xml")
                            };

bool SiteFileDscs::filterFile(TCchar* path) {
String s = toLocalPath(path);
int    i;
int    n;

  for (i = 0, n = noElements(filters); i < n; i++ ) if (s.find(filters[i]) >= 0) return true;

  return false;
  }


SiteFileDsc*  SiteFileDscs::findDir(TCchar* filePath) {
SiteFileKey key;

  key.dir = true;   key.path = getPath(filePath);

  return find(key);
  }


SiteFileDsc* SiteFileDscs::find(SiteFileKey& key) {
String s = key.path;

  if (s.find(root) == 0) s = s.substr(rootLng);

  SiteFileKey k = s;   k.dir = key.dir;

  return data.bSearch(k);
  }


void SiteFileDscs::delRcd(SiteFileDsc& uf) {
FileDscsIter iter(*this);
SiteFileDsc* sf;

  for (sf = iter(); sf; sf = iter++) if (sf->key.path == uf.key.path) {iter.remove(); return;}
  }



bool SiteFileDscs::loadFromCSV(Archive& ar) {
CSVLex     lex(ar);
CSVtokCode code;
CSVtokCode code1;
int        i;
int        version = 0;
uint       t;

  clear();    setRoot(siteID.localRoot);

  for (code = lex.get_token(), i = 0; code != EOFToken; code = lex.get_token(), i++) {

    if (code == EolToken) {lex.accept_token(); break;}

    CSVtok& tok = lex.token;   code1 = lex.token1.code;

    if (code == CommaToken && code1 == StringToken)
              {notePad << _T("Unable to find field in line: ") << *tok.psource << nCrlf;  return false;}

    switch (i) {
      case 0  : if (tok.name != siteID.name) return false;
                break;

      case 1  : if (tok.name != siteID.url) {
                  notePad << _T("Web Name differs: ") << siteID.url << _T(" != ") << tok.name;
                  notePad << nCrlf;
                  }
                break;
      case 2  : if (tok.name != siteID.localRoot) {
                  notePad << _T("Path differs: ") << siteID.localRoot << _T(" != ") << tok.name;
                  notePad << nCrlf;
                  }
                break;

      case 3  : version = tok.name.stoi(t); break;

      default : notePad << _T("Whoops!") << nCrlf;
      }

    lex.accept_two_tokens();
    }

  switch (version) {
    case 1  : return loadV1(lex);
    case 0  :
    default : return load(lex);
    }
  }


bool SiteFileDscs::load(CSVLex& lex) {
int i;

  data.clear();

  for (i = 0; ; i++) {
    SiteFileDsc siteFile;

    if (!siteFile.load(lex)) return i > 0;

    data = siteFile;
    }
  }


bool SiteFileDsc::load(CSVLex& lex) {
CSVtokCode code;
CSVtokCode code1;
int        i;
uint       x;

  for (code = lex.get_token(), i = 0; code != EOFToken; code = lex.get_token(), i++) {

    if (code == EolToken) {lex.accept_token(); return true;}

    CSVtok& tok = lex.token;   code1 = lex.token1.code;

    if (code != StringToken || code1 != CommaToken)
              {notePad << _T("Unable to find field in line: ") << *tok.psource << nCrlf;  return false;}

    switch (i) {
      case 0  : name = tok.name; break;

      case 1  : key.dir = 0; key.path = tok.name; break;

      case 2  : size = tok.name.stoi(x); break;

      case 3  : {ToDate lcldt(tok.name);  date = lcldt();} break;

      default : notePad << _T("Whoops!") << nCrlf;
      }
    lex.accept_two_tokens();
    }

  return false;
  }


bool SiteFileDscs::loadV1(CSVLex& lex) {
int i;

  data.clear();

  for (i = 0; ; i++) {
    SiteFileDsc siteFile;

    if (!siteFile.loadV1(lex)) return i > 0;

    data = siteFile;
    }
  }


bool SiteFileDsc::loadV1(CSVLex& lex) {
CSVtokCode code;
CSVtokCode code1;
int        i;
uint       x;

  for (code = lex.get_token(), i = 0; code != EOFToken; code = lex.get_token(), i++) {

    if (code == EolToken) {lex.accept_token(); return true;}

    CSVtok& tok = lex.token;   code1 = lex.token1.code;

    if (code != StringToken || code1 != CommaToken)
              {notePad << _T("Unable to find field in line: ") << *tok.psource << nCrlf;  return false;}

    switch (i) {
      case 0  : name = tok.name; break;

      case 1  : key.dir = tok.name.stoi(x); break;

      case 2  : key.path = tok.name; break;

      case 3  : size = tok.name.stoi(x); break;

      case 4  : {ToDate lcldt(tok.name);  date = lcldt();} break;

      default : notePad << _T("Whoops!") << nCrlf;
      }

    lex.accept_two_tokens();
    }

  return false;
  }



void SiteFileDscs::saveCSV(Archive& ar) {
CSVOut       csvOut(ar);
FileDscsIter iter(*this);
SiteFileDsc* dsc;

  csvOut << siteID.name << Comma << siteID.url << Comma << siteID.localRoot << Comma;
  csvOut << Version << Comma << vCrlf;

  for (dsc = iter(); dsc; dsc = iter++) {

    switch (dsc->status) {
      case DelSts   : if (!dsc->updated) dsc->save(csvOut);    break;

      case GetSts   : if ( dsc->updated) dsc->save(csvOut);    break;

      case WebPutSts:
      case DifPutSts: if (!dsc->updated)
                              {dsc = prvFileDscs.find(dsc->key);  if (!dsc) {iter.remove();  break;}}

      default       : dsc->save(csvOut); break;
      }
    }
  }


void SiteFileDsc::save(CSVOut& csvOut) {

  csvOut << name << Comma;
  csvOut << key.dir << Comma;
  csvOut << key.path << Comma;
  csvOut << size << Comma;
  csvOut << date << Comma;
  csvOut << vCrlf;
  }


void SiteFileDscs::update(SiteFileDscs& curDscs) {
FileDscsIter iter(curDscs);
SiteFileDsc* dsc;
SiteFileDsc* p;

  root = curDscs.root;   rootLng = curDscs.rootLng;   loaded = curDscs.loaded;

  updateDel(curDscs);

  for (dsc = iter(); dsc; dsc = iter++) {

    if (dsc->status == DelSts && dsc->updated) {iter.remove(); continue;}

    p = find(dsc->key);

    if (p) *p = *dsc;
    else    p = add(*dsc);

    p->clrSts();    dsc->clrSts();
    }
  }


void SiteFileDscs::updateDel(SiteFileDscs& curDscs) {
FileDscsIter iter(*this);
SiteFileDsc* dsc;
SiteFileDsc* p;

  for (dsc = iter(); dsc; dsc = iter++) {

    p = curDscs.find(dsc->key);

    if (p && p->status == DelSts && p->updated) iter.remove();
    }
  }


// Clear status for all entries that have been updated

void SiteFileDscs::clrSts() {
FileDscsIter iter(curFileDscs);
SiteFileDsc* dsc;

  for (dsc = iter(); dsc; dsc = iter++) if (dsc->updated) {

    switch (dsc->status) {
      case DelSts   : iter.remove();   break;

      case GetSts   : dsc->clrSts();   break;

      case WebPutSts:
      case DifPutSts: dsc->clrSts();   break;

      default       : break;
      }
    }
  }


SiteFileDsc* SiteFileDscs::add(SiteFileDsc& uf) {
SiteFileDsc* sf = find(uf.key);

  if (!sf) sf = data.allocate();    *sf = uf;   data = sf;    return sf;
  }



void SiteFileDscs::display(TCchar* title) {
FileDscsIter iter(*this);
SiteFileDsc* fl;

  notePad << nClrTabs << nSetRTab(32) << nSetRTab(43) << nSetTab(45) << nCrlf;

  notePad << nBeginLine << title << _T(" Files -- Path: ") << root;
  notePad << nTab << nData() << nEndLine << nCrlf << nCrlf;

  for (fl = iter(); fl; fl = iter++) fl->display();
  }


void SiteFileDsc::display() {
String d  = date;

  notePad << name << nTab << size << nTab << d << nTab << key.path << nCrlf;
  }


bool SiteFileDsc::put(int& noFiles) {
String       lclPath;
String       webPath;
SiteFileDsc* dsc;

  if (key.dir) return createWebDir(noFiles);

  dsc = curFileDscs.findDir(key.path);

  if (dsc && dsc->status != NilSts && !dsc->createWebDir(noFiles)) return false;

  lclPath = siteID.localRoot  + key.path;

  if (!sftpSSL.getLocalFile(lclPath)) return false;

  webPath = siteID.remoteRoot + key.path;

  if (!sftpSSL.stor(toRemotePath(webPath))) return false;

  sendStepPrgBar();   noFiles++;   updated = true;   return true;        //webFiles.modified();
  }


bool SiteFileDsc::createWebDir(int& noFiles) {
String       right = key.path;
String       left;
int          pos;
SiteFileKey  key;
SiteFileDsc* dsc;
String       fullPath;

  for (pos = right.find(_T('\\')); pos >= 0; pos = right.find(_T('\\'))) {

    left += right.substr(0, pos+1);   right = right.substr(pos+1);

    key.dir = true;   key.path = left;   dsc = curFileDscs.find(key);

    if (!dsc || (dsc->status != NilSts && !dsc->updated)) {
      fullPath = siteID.remoteRoot + left;   if (!sftpSSL.mkd(toRemotePath(fullPath))) return false;

      sendStepPrgBar();   noFiles++;   if (dsc) dsc->updated = true;
      }
    }

  return true;
  }


bool SiteFileDsc::get(int& noFiles) {
String webPath;
String lclPath;
String rslt;

  if (key.dir) return createLocalDir(noFiles);

  webPath = siteID.remoteRoot + key.path;

  if (!sftpSSL.retr(toRemotePath(webPath))) return false;

  lclPath = siteID.localRoot + key.path;

  if (!doc()->saveData(StoreSrc, toLocalPath(lclPath))) return false;

  sendStepPrgBar();   noFiles++;   updated = true;   addLclAttr(lclPath);

  sftpSSL.noop(rslt);    return true;
  }


bool SiteFileDsc::createLocalDir(int& noFiles) {
String       right = key.path;
String       left;
int          pos;
SiteFileKey  key;
SiteFileDsc* dsc;
String       fullPath;

  for (pos = right.find(_T('\\')); pos >= 0; pos = right.find(_T('\\'))) {

    left += right.substr(0, pos+1);   right = right.substr(pos+1);

    key.dir = true;   key.path = left;   dsc = curFileDscs.find(key);

    if (!dsc || (dsc->status != NilSts && !dsc->updated)) {
      fullPath = siteID.localRoot + left;

      if (!CreateDirectory(fullPath, 0)) return false;

      sendStepPrgBar();   noFiles++;   if (dsc) dsc->updated = true;
      }
    }

  return true;
  }


bool SiteFileDsc::del(int& noFiles) {
String webPath;

  if (key.dir) return removeDir(noFiles);

  webPath = siteID.remoteRoot + key.path;   webPath = toRemotePath(webPath);

  if (!sftpSSL.del(webPath)) return false;

  sendStepPrgBar();   noFiles++;   updated = true;   return true;
  }


bool SiteFileDsc::removeDir(int& noFiles) {
String       prefix = key.path;
FileDscsIter iter(curFileDscs);
SiteFileDsc* dsc;
DirList      dl;
DLIter       dlIter(dl);
DirItem*     item;

  for (dsc = iter(); dsc; dsc = iter++)
                               {String& path = dsc->key.path;   if (path.find(prefix) == 0) dl.add(dsc);}

  for (item = dlIter(); item; item = dlIter++) {
    SiteFileDsc* dsc = item->dsc;

    if (!dsc->key.dir) {if (!dsc->del(noFiles)) return false;   continue;}

//  notePad << item->level << nTab << dsc->key.path << nCrlf;

    String webPath = siteID.remoteRoot + dsc->key.path;   webPath = toRemotePath(webPath);

    if (!sftpSSL.rmd(webPath)) return false;

    sendStepPrgBar();   noFiles++;   dsc->updated = true;
    }

  return dl.nData() > 0;
  }



void SiteFileDscs::logSelected(TCchar* title) {
FileDscsIter iter(*this);
SiteFileDsc* dsc;

  if (!isLogging()) return;

  notePad << nClrTabs << nSetRTab(18) << nSetRTab(22) << nSetTab(24);
  notePad << nSetRTab(35) << nSetRTab(47) << nSetTab(49) << nCrlf;

  notePad << title << _T("   Log Selected -- Path: ") << root;
  notePad << _T(",   No of files: ") << nData() << nCrlf;
  notePad << nBeginLine << _T("Name") << nTab << _T("Chkd") << nTab << _T("Updt") << nTab << _T("Status");
  notePad << nTab << _T("Size") << nTab << _T("Date") << nTab << _T("Path") << nEndLine << nCrlf << nCrlf;

  for (dsc = iter(); dsc; dsc = iter++)
                                      if (dsc->check || dsc->status != NilSts || dsc->updated) dsc->log();
  }


void SiteFileDsc::log() {
String d    = date;
String chk  = check ? _T("X  ") : _T("_  ");
String sts;
String updt = updated ? _T("Updt") : _T("_  ");

  switch (status) {
    case NilSts   : sts = _T("NilSts");    break;
    case WebPutSts: sts = _T("WebPutSts"); break;
    case DifPutSts: sts = _T("DifPutSts"); break;
    case GetSts   : sts = _T("GetSts");    break;
    case DelSts   : sts = _T("DelSts");    break;
    case OthSts   : sts = _T("OthSts");    break;
    default       : sts = _T("Unknown");   break;
    }

  notePad << name << nTab << chk << nTab << updt << nTab << sts;
  notePad << nTab << size << nTab << d << nTab << key.path << nCrlf;
  }


bool SiteFileDsc::operator<  (SiteFileKey& s) {return s >  key;}
bool SiteFileDsc::operator<= (SiteFileKey& s) {return s >= key;}             // Required for Binary Search
bool SiteFileDsc::operator== (SiteFileKey& s) {return s == key;}             // Required for Binary Search
bool SiteFileDsc::operator!= (SiteFileKey& s) {return s != key;}
bool SiteFileDsc::operator>  (SiteFileKey& s) {return s <  key;}
bool SiteFileDsc::operator>= (SiteFileKey& s) {return s <= key;}


bool SiteFileKey::operator== (SiteFileKey& k) {return  dir == k.dir && _tcsicmp(path, k.path) == 0;}
bool SiteFileKey::operator!= (SiteFileKey& k) {return  dir != k.dir || _tcsicmp(path, k.path) != 0;}

bool SiteFileKey::operator<  (SiteFileKey& k)
                                {return  dir > k.dir || (dir == k.dir && _tcsicmp(path, k.path) <  0);}
bool SiteFileKey::operator<= (SiteFileKey& k)
                                {return (dir > k.dir || (dir == k.dir && _tcsicmp(path, k.path) <= 0));}

bool SiteFileKey::operator>  (SiteFileKey& k)
                                {return  dir < k.dir || (dir == k.dir && _tcsicmp(path, k.path) >  0);}
bool SiteFileKey::operator>= (SiteFileKey& k)
                                {return (dir < k.dir || (dir == k.dir && _tcsicmp(path, k.path) >= 0));}



