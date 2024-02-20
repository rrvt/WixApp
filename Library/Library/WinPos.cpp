// Manage Window Position and Size


#include "pch.h"
#include "WinPos.h"
#include "IniFile.h"


WinPos::WinPos() {
RECT rsys;

  SystemParametersInfo(SPI_GETWORKAREA, 0, &rsys, 0);

  screenWidth = rsys.right;   screenHeight = rsys.bottom;
  }


void WinPos::initialPos(CWnd* wnd, RECT& defaultRect) {

  data.load(defaultRect);   data.normalize(screenWidth, screenHeight);

  wnd->SetWindowPos(0, data.left, data.top, data.width, data.depth, SWP_NOCOPYBITS);    // | SWP_NOACTIVATE
  }


static TCchar* WindowPosData = _T("Window Position Data");
static TCchar* Left          = _T("Left");
static TCchar* Top           = _T("Top");
static TCchar* Width         = _T("Width");
static TCchar* Depth         = _T("Depth");


void WinPosData::save() {
  iniFile.write(WindowPosData, Left,  left);
  iniFile.write(WindowPosData, Top,   top);
  iniFile.write(WindowPosData, Width, width);
  iniFile.write(WindowPosData, Depth, depth);
  }


void WinPosData::load(RECT& defaultRect) {
  left   = iniFile.readInt(WindowPosData, Left,  defaultRect.left);
  top    = iniFile.readInt(WindowPosData, Top,   defaultRect.top);
  width  = iniFile.readInt(WindowPosData, Width, defaultRect.right  - defaultRect.left);
  depth  = iniFile.readInt(WindowPosData, Depth, defaultRect.bottom - defaultRect.top);
  if (left   <  0) left   =   0;
  if (top    <  0) top    =   0;
  if (width  < 100) width = 100;
  if (depth  < 100) depth = 100;
  }



void WinPosData::normalize(int screenWidth, int screenHeight) {

  if (width        < 100 || screenWidth < width)  width = 100;
  if (depth        < 100 || screenHeight < depth) depth = 100;
  if (left         < 0)                           left  = 0;
  if (left + width > screenWidth)                 left  = screenWidth  - width;
  if (top          < 0)                           top   = 0;
  if (top  + depth > screenHeight)                top   = screenHeight - depth;
  }





//void WinPos::getRect(CRect& rect) {if (getWindow()) {wnd->GetWindowRect(&rect);   data = rect;}}

