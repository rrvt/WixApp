// WixApp.h -- Template for a Dialog Box Application


#include "pch.h"
#include "WixApp.h"
#include "DefaultPath.h"
#include "FileName.h"
#include "IniFileEx.h"
#include "PFFdirectories.h"
#include "PMFdirectories.h"
#include "Product.h"
#include "Solution.h"
#include "WixAppDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


WixApp         theApp;                      // The one and only WixApp object
IniFileEx      iniFile(theApp);
Solution       solution;
PFFdirectories pffDirectories;
PMFdirectories pmfDirectories;
Icons          icons;
Features       features;
DefaultPath    defaultPath;


// WixApp construction

WixApp::WixApp() noexcept : CDialogApp(this) { }


// WixApp initialization -- Start the MFC Dialog Box

BOOL WixApp::InitInstance() {
WixAppDlg dlg(m_pszHelpFilePath);

  myPath  = getPath(m_pszHelpFilePath);   product.setInstallerIconPath(myPath);

  iniPath = iniFile.getAppDataPath(m_pszHelpFilePath);

  CWinApp::InitInstance();

  iniFile.setAppDataPath(m_pszHelpFilePath);   dlg.DoModal();   m_pMainWnd = 0;   return 0;
  }


int WixApp::ExitInstance() {return CDialogApp::ExitInstance();}

