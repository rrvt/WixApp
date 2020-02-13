
// WixApp.h : main header file for the WixApp application
//
#pragma once

#ifndef __AFXWIN_H__
  #error "include 'stdafx.h' before including this file for PCH"
#endif

#include "iniFile.h"
#include "Resource.h"       // main symbols


// WixApp:
// See WixApp.cpp for the implementation of this class
//

class WixApp : public CWinAppEx {
String helpFile;
public:

String myPath;
String iniPath;                                   // Path to ini File

  WixApp();

  // Overrides

  virtual BOOL InitInstance();
  virtual int  ExitInstance();

  DECLARE_MESSAGE_MAP()

  afx_msg void OnHelp();
  };


extern WixApp  theApp;    // The application code
extern IniFile iniFile;   // Roaming data file for app



