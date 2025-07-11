// Manage Ini File (A Place where stuff is remembered from execution to execution)
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.


#pragma once
#include "IniFileBase.h"


class IniFile : public IniFileBase {

public:

  IniFile() : IniFileBase() { }
 ~IniFile() { }
  };


extern IniFile iniFile;
