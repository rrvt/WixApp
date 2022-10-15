// WixApp.h -- Template for a Dialog Box Application


#pragma once
#include "CApp.h"


class WixApp : public CApp {

public:

String myPath;
String iniPath;                                   // Path to ini File
UINT   m_nAppLook;

  WixApp() noexcept;

public:

  virtual BOOL InitInstance();
  virtual int  ExitInstance();
  };


extern WixApp theApp;
