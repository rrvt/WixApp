// WixApp.h -- Template for a Dialog Box Application


#pragma once
#include "CDialogApp.h"


class WixApp : public CDialogApp {

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
