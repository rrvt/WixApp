// Window Data
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.


#pragma once

#include "IniFile.h"


struct WindowData {

IniFile& iniFile;

  WindowData(IniFile& ini) : iniFile(ini) {}

  void savePos( int  x, int  y);
  void saveSize(int cx, int cy);
  void getSize(RECT& rect);
  };

