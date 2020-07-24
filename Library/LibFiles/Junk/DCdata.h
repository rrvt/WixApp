// Collect CDC data for debugging


#pragma once
#include "Display.h"


struct DCdata {
CDC*          dc;
CString       cs;
RECT          clip;
CSize         textExt;
TEXTMETRIC    metric;
int           spacing;
CSize         viewPt;
CSize         wndExt;
FontAttr      attr;
LOGFONT       logFont;
TextPosition  tPos;
int           textWidth;
int           remaining;
int           width;
int           clipWidth;
int           x;

  DCdata() : clip({0,0,0,0}), textExt(0,0), viewPt(0,0), wndExt(0,0), textWidth(0), remaining(0),
                                                                         width(0), clipWidth(0), x(0) { }

  void operator() (String& s, Display& dev, bool inDev = false);
  };



