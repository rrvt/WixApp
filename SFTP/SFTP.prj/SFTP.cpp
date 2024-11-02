// SFTP.cpp : Defines the class behaviors for the application.


#include "pch.h"
#include "SFTP.h"
#include "AboutDlg.h"
#include "filename.h"
#include "IniFile.h"
#include "NotePad.h"
#include "Resource.h"
#include "SFTPDoc.h"
#include "SFTPView.h"
#include "WorkerThrd.h"


SFTP    theApp;                       // The one and only SFTP object
IniFile iniFile;
TCchar* AppTitle = _T("Secure File Transfer Protocol");

// SFTP

BEGIN_MESSAGE_MAP(SFTP, CWinAppEx)
  ON_COMMAND(ID_Help,      &onHelp)
  ON_COMMAND(ID_App_About, &onAppAbout)
END_MESSAGE_MAP()


// SFTP initialization

BOOL SFTP::InitInstance() {

  CWinAppEx::InitInstance();

  iniFile.setAppDataPath(m_pszHelpFilePath, *this);

  roamPath = getPath(iniFile.getAppDataPath(m_pszHelpFilePath));
  appPath  = getPath(m_pszHelpFilePath);

  notePad.clear();

  SetRegistryKey(appID);

  LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)

  // Register the application's document templates.  Document templates
  // serve as the connection between documents, frame windows and views

  CSingleDocTemplate* pDocTemplate;

  pDocTemplate = new CSingleDocTemplate(
    IDR_MAINFRAME,
    RUNTIME_CLASS(SFTPDoc),
    RUNTIME_CLASS(MainFrame),       // main SDI frame window
    RUNTIME_CLASS(SFTPView));

  if (!pDocTemplate) return FALSE;

  AddDocTemplate(pDocTemplate);

//  EnableShellOpen(); RegisterShellFileTypes(TRUE);       // Enable DDE Execute open

  // Parse command line for standard shell commands, DDE, file open

  CCommandLineInfo cmdInfo;   ParseCommandLine(cmdInfo);

  // Dispatch commands specified on the command line.  Will return FALSE if
  // app was launched with /RegServer, /Register, /Unregserver or /Unregister.

  if (!ProcessShellCommand(cmdInfo)) return FALSE;

  setAppName(_T("SFTP")); setTitle(AppTitle);

  view()->setFont(_T("Arial"), 12.0);

  doc()->defaultSite();

  m_pMainWnd->ShowWindow(SW_SHOW);   m_pMainWnd->UpdateWindow();   return TRUE;
  }



int SFTP::ExitInstance() {

#ifdef DebugMemoryLeaks
  _CrtDumpMemoryLeaks();
#endif

  return CApp::ExitInstance();
  }


void SFTP::onHelp() {
String topic = m_pszHelpFilePath; topic += _T(">Introduction");

  if (doc()->isLocked()) return;

  ::HtmlHelp(m_pMainWnd->m_hWnd, topic,  HH_DISPLAY_TOC, 0);
  }


void SFTP::onAppAbout() {

  if (doc()->isLocked()) return;

  AboutDlg aboutDlg; aboutDlg.DoModal();
  }

