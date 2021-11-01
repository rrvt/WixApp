
// WixApp.h : main header file for the WixApp application
//
#pragma once
#include "CApp.h"
#include "iniFile.h"
#include "Resource.h"       // main symbols


// WixApp:
// See WixApp.cpp for the implementation of this class
//

class WixApp : public CApp {
String helpFile;
public:

String myPath;
String iniPath;                                   // Path to ini File

  WixApp() : CApp(this) { }

  // Overrides

  virtual BOOL InitInstance();
  virtual int  ExitInstance();

  DECLARE_MESSAGE_MAP()

  afx_msg void OnHelp();
  };


extern WixApp  theApp;    // The application code
extern IniFile iniFile;   // Roaming data file for app



