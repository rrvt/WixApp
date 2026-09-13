// Clip Line using the Mouse Left Button to Select


#include "pch.h"
#include "ClipLine.h"
#include "ClipBoard.h"


ClipLine clipLine;


void ClipLine::clear() {
  dc = 0;   opened = false;   point = {-99, -99};   hzPos = 0;   tabSpaces = 0;

  clipped.clear();
  }


int ClipLine::tabWidth(int newPos, int maxChWidth) {
int width = newPos - hzPos;

  tabSpaces = (width + maxChWidth/2) / maxChWidth;

  return hzPos;
  }


void ClipLine::clipRegion(TCchar* txt, int xPos, int w, CDC* cdc) {
#if 1
#else
int y0 = vert.pos();
int y1 = y0 + vert.charHeight;

  if (point.y < y0 || y1 <= point.y) {close(dvx); return;}

  if (opened || (xPos <= point.x && point.x < xPos + w)) {open(dvx);   add(txt);}
#endif
  }


static String Spaces =
                 _T("                                                                           ");


void ClipLine::add(TCchar* txt) {

  if (tabSpaces) {clipped += Spaces.substr(0, tabSpaces);  tabSpaces = 0;}

  clipped += txt;
  }



/*
  COLORREF getBkColor()  {return dc->GetBkColor();}                     // Background color
  COLORREF getTxtColor() {return dc->GetTextColor();}                   // Text Color

  void     setBkColor( COLORREF color) {dc->SetBkColor(color);}         // Background color
  void     setTxtColor(COLORREF color) {dc->SetTextColor(color);}       // Text Color
*/

void ClipLine::open(CDC* cdc) {

  if (opened) return;

  opened = true;   dc = cdc;   clipped.clear();

  bkgdColor = dc->GetBkColor();;
  textColor = dc->GetTextColor();               // InvertRgn(CRgn* rgn);

  dc->SetBkColor(  invert(bkgdColor));
  dc->SetTextColor(invert(textColor));
  }


COLORREF ClipLine::invert(COLORREF c) {
int red = GetRValue(c);   red ^= 0xff;
int grn = GetGValue(c);   grn ^= 0xff;
int blu = GetBValue(c);   blu ^= 0xff;

  return RGB(red, grn, blu);
  }



void ClipLine::close(CDC* cdc) {
  if (opened) {restore(cdc);   opened = false;}
  }


void ClipLine::save(CDC* dc) {savedDC = dc->SaveDC();}


void ClipLine::restore(CDC* dc) {if (savedDC) dc->RestoreDC(savedDC);   savedDC = 0;}


bool ClipLine::load() {
ClipBoard clipBoard;
bool rslt = clipped.isEmpty() ? false : clipBoard.load(clipped);

  if (rslt) {point.x = -99;   point.y = -99;}

  return rslt;
  }



///-------------------

#if 0
bool loadClipBoard(TCchar* s) {
ToAnsi  ansi(s);
size_t  cbStr = ansi.length() + 1;
char*   p;
HGLOBAL hData;

  NewArray(char);   p = AllocArray(cbStr);   if (!p) return false;

  hData = GlobalAlloc(GMEM_MOVEABLE, cbStr);

  memcpy_s(GlobalLock(hData), cbStr, ansi(), cbStr);

  GlobalUnlock(hData);   FreeArray(p);

  if (!OpenClipboard(0)) {messageBox(_T("Cannot open the Clipboard")); return false;}

  if (!EmptyClipboard())
                 {messageBox(_T("Cannot empty the Clipboard"));    CloseClipboard(); return false;}

  if (::SetClipboardData(CF_TEXT, hData) == NULL)
                 {messageBox(_T("Unable to set Clipboard data"));  CloseClipboard(); return false;}

  CloseClipboard();   return true;
  }
#endif
