 // SFTPDoc.cpp : implementation of the SFTPDoc class


#include "pch.h"
#include "SFTPDoc.h"
#include "ClipLine.h"
#include "filename.h"
#include "IniFile.h"
#include "MessageBox.h"
#include "NotePad.h"
#include "Printer.h"
#include "Resource.h"
#include "SFTP.h"
#include "SftpLog.h"
#include "SFTPView.h"
#include "SiteFileDscs.h"
#include "UpdateDlg.h"
#include "WebFiles.h"
#include "workerThrd.h"


static TCchar* LocalWebPath = _T("LocalWebPath");

static UINT fromWebThrd(LPVOID param);


// SFTPDoc

IMPLEMENT_DYNCREATE(SFTPDoc, CDoc)

BEGIN_MESSAGE_MAP(SFTPDoc, CDoc)
  ON_COMMAND(ID_NewSite,          &onNewSite)
  ON_COMMAND(ID_PickSite,         &onPickSite)
  ON_COMMAND(ID_EditSite,         &onEditSite)
  ON_COMMAND(ID_WebCmpr,          &onPrepWebCmprCmd)
  ON_COMMAND(ID_PrevCmpr,         &onPrepPrevCmpr)

  ON_COMMAND(ID_UpdateSite,       &onUpdateSite)

  ON_COMMAND(ID_RemoteDir,        &onRemoteDir)
  ON_COMMAND(ID_DeleteSite,       &onDelSite)

  ON_COMMAND(ID_File_Save,        &OnFileSave)
  ON_COMMAND(ID_EDIT_COPY,        &onEditCopy)

//  ON_COMMAND(ID_CurrentTest,      &onCurrentTest)

//  ON_COMMAND(ID_SftpCmds,         &onSftpAcct)
  ON_COMMAND(ID_SftpAcct,         &onSftpAcct)
  ON_COMMAND(ID_SftpADAT,         &onSftpADAT)
  ON_COMMAND(ID_SftpALLO,         &onSftpALLO)
  ON_COMMAND(ID_SftpAPPE,         &onSftpAPPE)
  ON_COMMAND(ID_SftpAUTH,         &onSftpAUTH)
  ON_COMMAND(ID_SftpAVBL,         &onSftpAVBL)
  ON_COMMAND(ID_SftpCCC,          &onSftpCCC)
  ON_COMMAND(ID_SftpCDUP,         &onSftpCDUP)
  ON_COMMAND(ID_SftpCONF,         &onSftpCONF)
  ON_COMMAND(ID_SftpCSID,         &onSftpCSID)
  ON_COMMAND(ID_SftpCWD,          &onSftpCWD)
  ON_COMMAND(ID_SftpDELE,         &onSftpDELE)
  ON_COMMAND(ID_SftpDSIZ,         &onSftpDSIZ)
  ON_COMMAND(ID_SftpENC,          &onSftpENC)
  ON_COMMAND(ID_SftpFEAT,         &onSftpFEAT)
  ON_COMMAND(ID_SftpHELP,         &onSftpHELP)
  ON_COMMAND(ID_SftpLIST,         &onSftpLIST)
  ON_COMMAND(ID_SftpMDTM,         &onSftpMDTM)
  ON_COMMAND(ID_SftpMFCT,         &onSftpMFCT)
  ON_COMMAND(ID_SftpMIC,          &onSftpMIC)
  ON_COMMAND(ID_SftpMKD,          &onSftpMKD)
  ON_COMMAND(ID_SftpMLSD,         &onSftpMLSD)
  ON_COMMAND(ID_SftpMLST,         &onSftpMLST)
  ON_COMMAND(ID_SftpMODE,         &onSftpMODE)
  ON_COMMAND(ID_SftpNLST,         &onSftpNLST)
  ON_COMMAND(ID_SftpNOOP,         &onSftpNOOP)
  ON_COMMAND(ID_SftpOPTS,         &onSftpOPTS)
  ON_COMMAND(ID_SftpPWD,          &onSftpPWD)
  ON_COMMAND(ID_SftpQUIT,         &onSftpQUIT)
  ON_COMMAND(ID_SftpRETR,         &onSftpRETR)
  ON_COMMAND(ID_SftpRMD,          &onSftpRMD)
  ON_COMMAND(ID_SftpRMDA,         &onSftpRMDA)
  ON_COMMAND(ID_SftpRNFR,         &onSftpRNFR)
  ON_COMMAND(ID_SftpRNTO,         &onSftpRNTO)
  ON_COMMAND(ID_SftpSITE,         &onSftpSITE)
  ON_COMMAND(ID_SftpSIZE,         &onSftpSIZE)
  ON_COMMAND(ID_SftpSMNT,         &onSftpSMNT)
  ON_COMMAND(ID_SftpSPSV,         &onSftpSPSV)
  ON_COMMAND(ID_SftpSTAT,         &onSftpSTAT)
  ON_COMMAND(ID_SftpSTOR,         &onSftpSTOR)
  ON_COMMAND(ID_SftpSTOU,         &onSftpSTOU)
  ON_COMMAND(ID_SftpSYST,         &onSftpSYST)
  ON_COMMAND(ID_SftpTHMB,         &onSftpTHMB)
  ON_COMMAND(ID_SftpTYPE,         &onSftpTYPE)
  ON_COMMAND(ID_SftpUSER,         &onSftpUSER)
  ON_COMMAND(ID_SftpXCUP,         &onSftpXCUP)
  ON_COMMAND(ID_SftpXMKD,         &onSftpXMKD)
  ON_COMMAND(ID_SftpXPWD,         &onSftpXPWD)
  ON_COMMAND(ID_SftpXRMD,         &onSftpXRMD)
  ON_COMMAND(ID_SftpLogout,       &onSftpLogout)

  ON_COMMAND(ID_SftpNotImpl,      &onSftpNotImpl)

END_MESSAGE_MAP()


// SFTPDoc construction/destruction                                   , cmdLock(false)

SFTPDoc::SFTPDoc() noexcept : dataSource(NotePadSrc), savePrv(false), noFiles(0) { }

SFTPDoc::~SFTPDoc() {logoutSite();}


void SFTPDoc::defaultSite() {
String sect;

  if (!iniFile.read(GlobalSect, LastSiteKey, sect)) {display();  return;}

  if (!loadSiteID(sect))                            {display();  return;}

  if (!loginSite())                {clearSiteID();   display();   return;}

  if (!loadSiteDescriptors())      {clearSiteID();   display();   return;}

  display();
  }


bool SFTPDoc::isLocked(bool prevent) {

  if (!workerThrd.isLocked() && !prevent && !siteID.isLoginPending()) return false;

  notePad << _T("Unable to perform command at this time") << nCrlf;   display();   return true;
  }



// Create a new site

void SFTPDoc::onNewSite() {                                   // XXX

  if (isLocked()) return;

  notePad.clear();    clearSiteID();

  iniFile.readString(GlobalSect, LocalWebPath, path);

  getDirPathDlg(_T("Web Site"), path);   fixLocalPath(path);

  String pth = ::getPath(path);

  iniFile.writeString(GlobalSect, LocalWebPath, pth);   siteID.localRoot = pth;

  if (!doEditSite()) {display();   return;}

  loadSiteDescriptors();   display();
  }


void SFTPDoc::onEditSite() {                                  // XXX

  if (isLocked()) return;

  notePad.clear();

  if (!doEditSite()) return;

  loadSiteDescriptors();   display();
  }


bool SFTPDoc::doEditSite() {editSite();   logoutSite();    return loginSite();}     // XXX


void SFTPDoc::onPickSite() {                                  // XXX

  if (isLocked()) return;

  logoutSite();   pickSite();

  notePad.clear();   if (!loginSite()) {display();   return;}

  setCurrentSite();   loadSiteDescriptors();   display();
  }


void SFTPDoc::onDelSite() {
String msg;

  if (isLocked() || siteID.name.isEmpty()) return;

  msg = _T("Delete Site: ") + siteID.name;

  switch (AfxMessageBox(msg, MB_YESNO)) {

    case IDYES: break;  // The Yes button was selected.
    default   : return;
    }

  notePad << _T("Deleting ") << siteID.name << nCrlf;

  prvFileDscs.clear();   curFileDscs.clear();

  ToAnsi ansi(siteID.dbPath(path));   remove(ansi());    siteID.delSiteData();

  display();
  }


bool SFTPDoc::loadSiteDescriptors() {
String path;

  if (!curFileDscs.loadFromPC())
                            {notePad << siteID.localRoot << _T(" does not exist") << nCrlf; return false;}

  if (!loadPrevSiteData()) {prvFileDscs.loadFromPC();   saveData(CurSiteSrc, siteID.dbPath(path));}

  return true;
  }


void SFTPDoc::onPrepPrevCmpr() {
FileDscsIter  iter(curFileDscs);
SiteFileDsc*  dsc;
String        prefix;

  if (isLocked()) return;

  notePad.clear();   curFileDscs.setCheck();   curFileDscs.updateFromPC();   findDeletedFiles();

  for (dsc = iter(); dsc; dsc = iter++) {

    if (dsc->check) continue;

    SiteFileDsc* prv = prvFileDscs.find(dsc->key);

    if (prv) {
       if (prv->size == dsc->size && prv->date == dsc->date) continue;
       if (prv->key.dir) continue;
       }

    dsc->check = true;   dsc->status = DifPutSts;   dsc->updated = false;
    }

  prepPrevCmprDsp();   display();
  }


void SFTPDoc::findDeletedFiles() {
FileDscsIter  iter(prvFileDscs);
SiteFileDsc*  dsc;

  for (dsc = iter(); dsc; dsc = iter++) {

    if (dsc->check) continue;

    SiteFileDsc* cur = curFileDscs.find(dsc->key);   if (cur) continue;

    dsc->check = true;   dsc->status = DelSts;   dsc->updated = false;   curFileDscs.add(*dsc);
    }
  }


void SFTPDoc::prepPrevCmprDsp() {
FileDscsIter  iter(curFileDscs);
SiteFileDsc*  dsc;
String        prefix;

  notePad << nClrTabs << nSetTab(15);

  for (dsc = iter(), noFiles = 0; dsc; dsc = iter++) {
    if (!dsc->check) continue;

    switch (dsc->status) {
      case WebPutSts:
      case DifPutSts: if (dsc->key.dir) prefix = _T("Create Dir on Web:");
                      else              prefix = _T("Upload to Web:");
                      break;
      case GetSts   : if (dsc->key.dir) prefix = _T("Create Local Dir:");
                      else              prefix = _T("Download from Web:");
                      break;
      case DelSts   : if (dsc->key.dir) prefix = _T("Delete Dir on Web:");
                      else              prefix = _T("Delete File on Web:");
                      break;
      case OthSts   :                   prefix = _T("Other:");   break;
      default       :                   prefix = _T("Unknown:"); break;
      }

    notePad << prefix << nTab << dsc->key.path << nCrlf;   noFiles++;
    }

  notePad << nCrlf;

  if      (noFiles <= 0) notePad << _T("No Files");
  else if (noFiles == 1) notePad << noFiles << _T(" file");
  else                   notePad << noFiles << _T(" files");

  notePad << _T(" marked for up load or deletion due to a change in the local files") << nCrlf;
  }


void SFTPDoc::onUpdateSite() {
SiteUpdateDlg dlg(curFileDscs);

  if (isLocked(noFiles == 0)) return;

  if (dlg.DoModal() == IDOK) updateCmd.start();
  }


void SFTPDoc::saveSite(DataSource src) {
String path;

  dataSource = src;   onSaveDocument(dbPath(path));   dataSource = NotePadSrc;
  }


void SFTPDoc::onEditCopy() {                                  // XXXX

  if (isLocked()) return;

  clipLine.load();
  }


void SFTPDoc::display(DataSource ds) {dataSource = ds; invalidate();}


void SFTPDoc::OnFileSave() {                                  // XXXX

  if (isLocked()) return;

#if 1
  dataSource = CurSiteSrc;   saveFile(_T("CSV File"), _T("save"), _T("csv"));
#else
  switch (dataSource) {
    case NotePadSrc : if (setSaveAsPath(pathDlgDsc)) OnSaveDocument(path);   break;
    case StoreSrc   : dataSource = StrTxtSrc;  saveFile(_T("Store"), _T("Str"), _T("txt")); break;
    }
#endif
  invalidate();
  }


void SFTPDoc::saveFile(TCchar* title, TCchar* suffix, TCchar* fileType) {
String fileName = path;
int    pos      = fileName.find_last_of(_T('\\'));
String ext      = _T("*."); ext += fileType;
String ttl      = title;    ttl += _T(" Output");

  fileName = fileName.substr(pos+1);   pos = fileName.find_first_of(_T('.'));
  fileName = fileName.substr(0, pos);  fileName += suffix;

  pathDlgDsc(ttl, fileName, fileType, ext);

  if (setSaveAsPath(pathDlgDsc)) OnSaveDocument(path);
  }


// Override the version in CDoc to force creation of file

BOOL SFTPDoc::OnSaveDocument(LPCTSTR lpszPathName) {
String  path = lpszPathName;
Archive ar(path, FileIO::Write | FileIO::Create);   if (!ar.isOpen()) return false;

  serialize(ar);   return true;
  }


// The load and save functions do the necessary things to open files and then call the serialize
// function which switches to the correct operation and function to do the work.  This was the MFC
// way of doing business and I just copied with my own flavor of opening files, etc...

bool SFTPDoc::loadPrevSiteData() {                            // XXX
String path;

  dataSource = PrvSiteSrc;

  return OnOpenDocument(dbPath(path)) && !prvFileDscs.isEmpty();
  }


// serialization

void SFTPDoc::serialize(Archive& ar) {

  if (ar.isStoring())
    switch(dataSource) {
      case NotePadSrc : notePad.archive(ar);      return;
      case CurSiteSrc : curFileDscs.saveCSV(ar);  prvFileDscs.update(curFileDscs);
                        curFileDscs.clrSts();     return;
      case StoreSrc   : sftpSSL.store(ar);        return;
      default         : return;
      }

  else
    switch(dataSource) {
      case PrvSiteSrc : prvFileDscs.loadFromCSV(ar);   return;
      default         : return;
      }
  }


void SFTPDoc::debugDsp(TCchar* title) {
FileDscsIter iter(curFileDscs);
SiteFileDsc* dsc;

  notePad << nCrlf << title << nCrlf << nCrlf;
  notePad << nClrTabs << nSetTab(3);

  for (dsc = iter(); dsc; dsc = iter++) if (dsc->key.path.find(_T("test0")) > 0)
               notePad << nTab << dsc->key.path << nTab << dsc->size << nTab << dsc->date << nCrlf;

  display();
  }


// SFTPDoc diagnostics

#ifdef _DEBUG
void SFTPDoc::AssertValid() const {         CDocument::AssertValid();}
void SFTPDoc::Dump(CDumpContext& dc) const {CDocument::Dump(dc);}
#endif //_DEBUG





