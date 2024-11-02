// Update Local or Remote Site


#include "pch.h"
#include "UpdateCmd.h"
#include "NotePad.h"
#include "SFTP.h"
#include "SFTPDoc.h"
#include "SiteFileDscs.h"
#include "SiteID.h"
#include "WebFiles.h"
#include "WorkerThrd.h"


static UINT updateThrd(void* param);


UpdateCmd updateCmd;


void UpdateCmd::start() {

  if (isLocked()) return;

  notePad.clear();   noFiles = 0;

  if (!siteID.login()) return;

  notePad << _T("Update Files") << nCrlf << nCrlf;

  mainFrm()->startPrgBar(curFileDscs.nData());

  workerThrd.start(updateThrd, (void*) &curFileDscs, ID_UpdateMsg);
  }


bool UpdateCmd::isLocked(bool prevent) {return doc()->isLocked(prevent);}


UINT updateThrd(void* param) {

  sendWdwScroll();

  if (curFileDscs.isEmpty()) return false;

  return updateCmd.doXfer(*(SiteFileDscs*) param) ? 0 : 1;
  }


bool UpdateCmd::doXfer(SiteFileDscs& dscs) {
FileDscsIter iter(dscs);
SiteFileDsc* dsc;
String       lclPath;
String       webPath;
String       rslt;

  for (dsc = iter(); dsc; dsc = iter++) {

    if (!dsc->check) continue;

    switch (dsc->status) {
      case WebPutSts:
      case DifPutSts: if (!dsc->put(noFiles)) continue;

                      webFiles.modified();    break;

      case GetSts   : if (!dsc->get(noFiles)) continue;   break;

      case DelSts   : if (!dsc->del(noFiles)) continue;

                      webFiles.modified();    break;

      default       : continue;
      }
    }

  return true;
  }


LRESULT UpdateCmd::finishMsg(WPARAM wparam, LPARAM lParam) {
String path;

  mainFrm()->closePrgBar();

  display();

  if (noFiles) doc()->saveData(CurSiteSrc, siteID.dbPath(path));

  sendDisplayMsg();   Sleep(1);   sendWdwScroll(false);   return 0;
  }


void UpdateCmd::display() {
FileDscsIter iter(curFileDscs);
SiteFileDsc* dsc;
int          n;

  notePad << nClrTabs << nSetTab(15);

  for (dsc = iter(), n = 0; dsc; dsc = iter++) if (dsc->updated) {

    switch (dsc->status) {
      case WebPutSts:
      case DifPutSts: notePad << _T(" Uploaded:");    break;
      case GetSts   : notePad << _T(" Downloaded:");  break;
      case DelSts   : notePad << _T(" Deleteed:");    break;
      default       : notePad << _T(" Unknown Op:");  break;
      }

    notePad << nTab << dsc->key.path<< nCrlf;    n++;
    }

  if      (n <= 0) notePad << _T("No Files");
  else if (n == 1) notePad << n << _T(" file");
  else                   notePad << n << _T(" files");

  notePad << _T(" updated") << nCrlf;

  sendDisplayMsg();
  }

