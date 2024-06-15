// Manage Window Position and Size


#include "pch.h"
#include "WinPos.h"
#include "IniFile.h"
#include "NotePad.h"


static const double MinFactor = 3.0;

WinPos winPos;


WinPos::WinPos() : wnd(0), hDLUtoPxls(1), vDLUtoPxls(1), defWidth(100), defDepth(100) {
CRect rsys;

  SystemParametersInfo(SPI_GETWORKAREA, 0, &rsys, 0);

  screenWidth = rsys.right;   screenHeight = rsys.bottom;
  }


void WinPos::setDLUToPxls(CRect& rect, int hDLU, int vDLU) {
double dlu;

  dlu = hDLU;  hDLUtoPxls = (rect.right - rect.left) / dlu;
  dlu = vDLU;  vDLUtoPxls = (rect.bottom - rect.top) / dlu;
  }


int WinPos::dluToScreen(int dlu, bool horiz) {
double t = dlu;   return int(t * (horiz ? hDLUtoPxls : vDLUtoPxls) + 0.5);
}


void WinPos::initialPos(CWnd* cWnd, CRect& defaultRect) {
  wnd = cWnd;   data.load(defaultRect);   data.normalize(screenWidth, screenHeight);   setPos();
  data.get(defaultRect);
  }


static TCchar* WindowPosData = _T("Window Position Data");
static TCchar* Left          = _T("Left");
static TCchar* Top           = _T("Top");
static TCchar* Width         = _T("Width");
static TCchar* Depth         = _T("Depth");


WinPosData::WinPosData() : left(0), top(0), width(0), depth(0),
                                          invXbdr(0), invYbdr(0),  minWidth(200), minDepth(200) { }


void WinPosData::save() {
  iniFile.write(WindowPosData, Left,  left);
  iniFile.write(WindowPosData, Top,   top);
  iniFile.write(WindowPosData, Width, width);
  iniFile.write(WindowPosData, Depth, depth);
  }


void WinPosData::load(CRect& defaultRect) {
  left   = iniFile.readInt(WindowPosData, Left,  defaultRect.left);
  top    = iniFile.readInt(WindowPosData, Top,   defaultRect.top);
  width  = iniFile.readInt(WindowPosData, Width, defaultRect.right  - defaultRect.left);
  depth  = iniFile.readInt(WindowPosData, Depth, defaultRect.bottom - defaultRect.top);

  if (!width || !depth) {
    left   = defaultRect.left;
    top    = defaultRect.top;
    width  = defaultRect.right  - defaultRect.left;
    depth  = defaultRect.bottom - defaultRect.top;
    }

  defWidth = width;                    defDepth = depth;
  minWidth = int(width / MinFactor);   minDepth = int(depth / MinFactor);

  rationalize();
  }



WinPosData& WinPosData::set(CWnd* wnd, CRect& r) {
int w = r.right  - r.left;
int d = r.bottom - r.top;

  left = r.left; top = r.top;   getWidthDepth(wnd, w, d);   return *this;
  }


void WinPosData::set(CWnd* wnd, int& cx, int& cy) {
int w = cx + invXbdr;
int d = cy + invYbdr;

  getWidthDepth(wnd, w, d);    cx = width - invXbdr;    cy = depth - invYbdr;
  }


// width and depth in pixels including invisible boarders

void WinPosData::getWidthDepth(CWnd* wnd, int w, int d) {
  if      (w >  defWidth) {width = defWidth = w;   minWidth = int (width / MinFactor);}
  else if (w >= minWidth)  width = w;
  else                    {width = minWidth;   setPos(wnd);}

  if      (d >  defDepth) {depth = defDepth = d;   minDepth = int(depth / MinFactor);}
  else if (d >= minDepth)  depth = d;
  else                    {depth = minDepth;   setPos(wnd);}
  }


void WinPosData::setInvBdrs(CRect& winRect, int cx, int cy) {
int w = winRect.right  - winRect.left;
int d = winRect.bottom - winRect.top;

  invXbdr = w - cx;
  invYbdr = d - cy;
  }


bool WinPosData::setPos(CWnd* wnd)
                            {return wnd->SetWindowPos(0, left, top, width, depth, SWP_NOCOPYBITS);}


void WinPosData::display(TCchar* tgt, int d) {
String s;

  s.format(_T("%s: %i, %i, %i, %i"), tgt, d, width, defWidth, minWidth);
  notePad << s << nCrlf;
  }


void WinPosData::normalize(int screenWidth, int screenHeight) {

  rationalize();
  if (screenWidth  < width)        width = minWidth;
  if (screenHeight < depth)        depth = minDepth;
  if (left + width > screenWidth)  left  = screenWidth  - width;
  if (top  + depth > screenHeight) top   = screenHeight - depth;
  }



void WinPosData::rationalize() {
  if (left   <  0)       left   =   0;
  if (top    <  0)       top    =   0;
  if (width  < minWidth) width = minWidth;
  if (depth  < minDepth) depth = minDepth;
  }


