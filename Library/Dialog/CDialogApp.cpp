// Subclass of CWinAppEx


#include "pch.h"
#include "CDialogApp.h"
#include "ResourceData.h"


CDialogApp* theDlgApp;


CDialogApp::CDialogApp(CDialogApp* app) : WinAppEx(app) {
ResourceData res;

  theDlgApp = app;

  getAppName(appName);

  res.getVersion(version);

  if (res.getAppID(appID)) SetAppID(appID);   else messageBox(_T("Unable to create an appID"));

  EnableHtmlHelp();
  }


void CDialogApp::getAppName(String& name) {
ResourceData res;
int          pos;

  if (res.getInternalName(name))
                 {pos = name.find(_T('.'));   if (pos > 0) {name = name.substr(0, pos);   return;}}

  if (res.getFileDescription(name)) return;
  }


CDialogApp::~CDialogApp() {

  appID.~String();   version.~String();
  }


BOOL CDialogApp::InitInstance() {return WinAppEx::InitInstance();}






int CDialogApp::ExitInstance() {


  return CWinAppEx::ExitInstance();
  }



void CDialogApp::dbg(TCchar* fmt, void* v) {
  String f = fmt;   f += _T(": %08x");
  String s;   s.format(f, v);   messageBox(s);
  }





/////////-------------------
#if 0
// Get current default printer and then override the devmode parameters with saved parameters

void CDialogApp::initPrinter()
          {UpdatePrinterSelection(true);   getPrinterAttr();   prtrDevAttr.load(prtrDevAttr.name);}

HANDLE CDialogApp::getDefaultPrinter() {
PRINTDLG pd;
HANDLE   hdl = 0;

  if (m_hDevMode) return m_hDevMode;

  memset(&pd, 0, sizeof(PRINTDLG));   pd.lStructSize = sizeof(PRINTDLG);

  return getDefaultPrinter(pd) ? pd.hDevMode : 0;
  }


bool CDialogApp::getDefaultPrinter(PRINTDLG& pd) {

  if (GetPrinterDeviceDefaults(&pd)) return true;

  messageBox(_T("Default printer drivers are damaged, try reinstalling drivers")); return false;
  }
#endif

