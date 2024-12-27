// WixApp.h -- Template for a Dialog Box Application


#include "pch.h"
#include "WixApp.h"
#include "FileName.h"
#include "Product.h"
#include "WixAppDlg.h"
#include "IniFile.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


WixApp  theApp;                           // The one and only WixApp object
IniFile iniFile;


// WixApp construction

WixApp::WixApp() noexcept : CDialogApp(this) { }


// WixApp initialization -- Start the MFC Dialog Box

BOOL WixApp::InitInstance() {
WixAppDlg dlg(m_pszHelpFilePath);

  myPath  = getPath(m_pszHelpFilePath);   product.setInstallerIconPath(myPath);

  iniPath = iniFile.getAppDataPath(m_pszHelpFilePath);

  CWinApp::InitInstance();

  iniFile.setAppDataPath(m_pszHelpFilePath, *this);   dlg.DoModal();   m_pMainWnd = 0;   return 0;
  }


int WixApp::ExitInstance() {return CWinApp::ExitInstance();}

