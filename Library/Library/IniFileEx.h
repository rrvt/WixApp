// Manage Ini File (A Place where stuff is remembered from execution to execution)
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.



#pragma once
#include "pch.h"
#include "IniFileBase.h"


class IniFileEx : public IniFileBase {

CWinApp& theApp;

public:

  IniFileEx(CWinApp& app) : IniFileBase(), theApp(app) { }
 ~IniFileEx() {clrTheAppPath();}

  void    setAppDataPath(TCchar* helpPath);
  void    setFilePath(String& pth);
  void    setFilePath(TCchar* pth);

private:

  void    setTheAppPath();
  void    clrTheAppPath();

  IniFileEx() : theApp(*(CWinApp*)0) {}
  };


extern IniFileEx iniFile;
