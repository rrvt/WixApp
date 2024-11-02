// SFTP.h : main header file for the SFTP application

#pragma once
#include "CApp.h"
#include "MainFrame.h"

class SFTPDoc;
class SFTPView;


extern TCchar* AppTitle;


class SFTP : public CApp {

String roamPath;
String appPath;

public:

               SFTP() noexcept : CApp(this) { }

  virtual BOOL InitInstance();
  virtual int  ExitInstance();

    SFTPDoc*   doc()         {return (SFTPDoc*)  CApp::getDoc();}
    SFTPView*  view()        {return (SFTPView*) CApp::getView();}
    MainFrame* mainFrm()     {return (MainFrame*) m_pMainWnd;}
    String&    roamingPath() {return roamPath;}
    String&    thisAppPath() {return appPath;}

  DECLARE_MESSAGE_MAP()

  afx_msg void onAppAbout();
  afx_msg void onHelp();
  };


extern SFTP theApp;

inline void       invalidate() {theApp.invalidate();}
inline SFTPDoc*   doc()        {return theApp.doc();}
inline SFTPView*  view()       {return theApp.view();}
inline MainFrame* mainFrm()    {return theApp.mainFrm();}
inline MyToolBar& getToolBar() {return mainFrm()->getToolBar();}

