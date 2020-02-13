// Manage Window Position and Size


#include "stdafx.h"
#include "WindowPos.h"
#include "IniFile.h"


WindowPos windowPos;



WindowPos::WindowPos() : window(0), dfltTop(0) {
RECT rsys;

  SystemParametersInfo(SPI_GETWORKAREA, 0, &rsys, 0);

  maxWidth   = rsys.right;
  maxHeight  = rsys.bottom;
  dfltWidth  = rsys.right/2;
  dfltHeight = rsys.bottom * 3 / 4;
  dfltLeft   = (rsys.right - dfltWidth) / 2;
  dfltHeight = rsys.bottom - dfltHeight;

  winRect.top = 0; winRect.left = 0; winRect.bottom = dfltHeight; winRect.right = dfltWidth;
  }


WindowPos::~WindowPos() {save();}


void WindowPos::set(CWnd* w) {
int  width;
int  height;
int  left;
int  top;

  window = w;
  left   = iniFile.readInt(_T("Position"), _T("Left"),   -1);
  top    = iniFile.readInt(_T("Position"), _T("Top"),    -1);
  width  = iniFile.readInt(_T("Position"), _T("Width"),  -1);
  height = iniFile.readInt(_T("Position"), _T("Height"), -1);

  normalizeWindowRect(left, top, width, height);

  w->SetWindowPos(0, left, top, width, height, SWP_NOCOPYBITS | SWP_NOACTIVATE );
  }


void WindowPos::save() {
int left   = winRect.left;
int top    = winRect.top;
int width  = winRect.right  - winRect.left;
int height = winRect.bottom - winRect.top;

  normalizeWindowRect(left, top, width, height);

  iniFile.writeInt(_T("Position"), _T("Left"),   left);
  iniFile.writeInt(_T("Position"), _T("Top"),    top);
  iniFile.writeInt(_T("Position"), _T("Width"),  width);
  iniFile.writeInt(_T("Position"), _T("Height"), height);
  }


void WindowPos::normalizeWindowRect(int& left, int& top, int& width, int& height) {

  if (width  < 150 || maxWidth      < width)  width  = dfltWidth;
  if (height < 100 || maxHeight     < height) height = dfltHeight;
  if (left   < 0   || maxWidth-150  < left)   left   = dfltLeft;
  if (top    < 0   || maxHeight-100 < top)    top    = dfltTop;
  }

