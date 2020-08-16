
// WixApp.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "WixApp.h"
#include "filename.h"
#include "htmlhelp.h"
#include "MainFrm.h"
#include "Product.h"
#include "Resources.h"
#include "WixDataDlg.h"
#include "afxwinappex.h"
#include "afxdialogex.h"


// WixApp

BEGIN_MESSAGE_MAP(WixApp, CWinAppEx)
  ON_COMMAND(ID_HELP, &WixApp::OnHelp)
END_MESSAGE_MAP()


// Global Variables

WixApp  theApp;                           // The one and only WixApp object
IniFile iniFile;                          // Implements Read/Write to Private Profile File


// WixApp construction

WixApp::WixApp() {
ResourceData res;
String       appID;

  if (res.getAppID(appID)) SetAppID(appID);  EnableHtmlHelp();
  }



// WixApp initialization

BOOL WixApp::InitInstance() {

  helpFile = m_pszHelpFilePath;  //change_extension(helpFile, "chm");

  myPath  = getPath(m_pszHelpFilePath);
  iniPath = iniFile.getAppDataPath(m_pszHelpFilePath);

  product.setInstallerIconPath(getPath(m_pszHelpFilePath));

  CWinAppEx::InitInstance();

  // To create the main window, this code creates a new frame window
  // object and then sets it as the application's main window object

  MainFrame* pFrame = new MainFrame;   if (!pFrame) return FALSE;

  m_pMainWnd = pFrame;

  // create and load the frame with its resources

  pFrame->LoadFrame(IDR_MAINFRAME, WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL, NULL);

  // Start Dialog box here to begin Wix construction

  WixDataDlg wixDataDlg;  wixDataDlg.DoModal();

  delete pFrame;   return 0;                        // Never open the main window

  // The one and only window has been initialized, so show and update it

  pFrame->SetWindowPos(&CWnd::wndTopMost, 200, 200, 600, 600, SWP_NOMOVE);

  pFrame->ShowWindow(SW_SHOW);   pFrame->UpdateWindow(); return TRUE;
  }


int WixApp::ExitInstance() {return CWinAppEx::ExitInstance();}




void WixApp::OnHelp() {
String topic;

  topic = helpFile; topic += _T(">Introduction");

  ::HtmlHelp(m_pMainWnd->m_hWnd, topic,  HH_DISPLAY_TOC, 20);
  }
