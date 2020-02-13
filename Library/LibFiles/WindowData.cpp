// Window Data
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.


#include "stdafx.h"
#include "WindowData.h"


static TCchar* WindowSize    = _T("Window Data");
static TCchar* Left          = _T("Left");
static TCchar* Top           = _T("Top");
static TCchar* Width         = _T("Width");
static TCchar* Height        = _T("Height");


void WindowData::savePos(int x, int y) {
String left = toString(x);
String top  = toString(y);

  iniFile.writeString(WindowSize, Left, left);
  iniFile.writeString(WindowSize, Top,   top);
  }


void WindowData::saveSize(int cx, int cy) {
String width  = toString(cx);
String height = toString(cy);

  iniFile.writeString(WindowSize, Width,   width);
  iniFile.writeString(WindowSize, Height, height);
  }


void WindowData::getSize(RECT& rect) {

  rect.left   = iniFile.readInt(WindowSize, Left,    100); if (rect.left   <  0) rect.left   =  0;
  rect.top    = iniFile.readInt(WindowSize, Top,     100); if (rect.top    <  0) rect.top    =  0;
  rect.right  = iniFile.readInt(WindowSize, Width,  1031); if (rect.right  < 10) rect.right  = 10;
  rect.bottom = iniFile.readInt(WindowSize, Height,  923); if (rect.bottom < 10) rect.bottom = 10;
  }

