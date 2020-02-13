// Display to Window
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.


#include "stdafx.h"
#include "Display.h"
#include "EditBoxes.h"

#include "MessageBox.h"


static const long maxScroll = 2147483648;

Manip dCenter;
Manip dRight;      // right align the String following up to the nCrlf
Manip dCrlf;
Manip dEndPage;    // add to stream to terminate a page when printing or do nothing
Manip dflushFtr;   // add to stream to terminate a footer when printing
Manip dClrTabs;    // add to stream to clear tabs: dsp << dClrTabs;
Manip dTab;
Manip dBeginLine;
Manip dEndLine;
Manip dPrevFont;   // Restore previous font
Manip dBoldFont;
Manip dItalicFont;
Manip dUnderLineFont;
Manip dStrikeOutFont;


// Manipulator functions -- do not call directly
static Display& doCrlf(          Display& d);
static Display& doEndPage(       Display& d);
static Display& doFlushFtr(      Display& d);
static Display& doClrTabs(       Display& d) {d.tPos.clrTabs(); return d;}
static Display& doTab(           Display& d);
static Display& doCenter(        Display& d);
static Display& doRight(         Display& d);
static Display& doPrevFont(      Display& d);
static Display& doBoldFont(      Display& d);
static Display& doItalicFont(    Display& d);
static Display& doUnderLineFont( Display& d);
static Display& doStrikeOutFont( Display& d);


static Display& doBeginLine(   Display& d);
static Display& doEndLine(     Display& d);

static Display& doSetLMargin(  Display& d, int v) {d.tPos.setLeftMargin(v); return d;}
static Display& doSetTab(      Display& d, int v) {d.tPos.setTab(v);  return d;}
static Display& doSetRTab(     Display& d, int v) {d.tPos.setRTab(v); return d;}
static Display& doFSize(       Display& d, int v);
static Display& doEditBox(     Display& d, int v);

Manip1& dSetLMargin(int val) {Manip1* m = new Manip1(doSetLMargin,  val); return *m;}
Manip1& dSetTab(    int val) {Manip1* m = new Manip1(doSetTab,      val); return *m;}
Manip1& dSetRTab(   int val) {Manip1* m = new Manip1(doSetRTab,     val); return *m;}
Manip1& dFSize(     int val) {Manip1* m = new Manip1(doFSize,       val); return *m;}
Manip1& dEditBox(   int val) {Manip1* m = new Manip1(doEditBox,     val); return *m;}



Display::Display() : tPos() {

  noPoints = maxHeight = curHeight = toLine = maxY = y = topEdge = bottomEdge = 0;
  center = right = beginPage = endPageFlag = false;
  dc = 0; tPos.clrTabs();
  points[0].x = 0;

  output = printing = false;   noPages = 0;  nonBlankLine = true;

  dCrlf.d           = this; dCrlf.func           = &doCrlf;
  dEndPage.d        = this; dEndPage.func        = &doEndPage;
  dflushFtr.d       = this; dflushFtr.func       = &doFlushFtr;
  dTab.d            = this; dTab.func            = &doTab;
  dClrTabs.d        = this; dClrTabs.func        = &doClrTabs;
  dCenter.d         = this; dCenter.func         = &doCenter;
  dRight.d          = this; dRight.func          = &doRight;
  dBeginLine.d      = this; dBeginLine.func      = &doBeginLine;
  dEndLine.d        = this; dEndLine.func        = &doEndLine;

  dPrevFont.d       = this; dPrevFont.func       = &doPrevFont;
  dBoldFont.d       = this; dBoldFont.func       = &doBoldFont;
  dItalicFont.d     = this; dItalicFont.func     = &doItalicFont;
  dUnderLineFont.d  = this; dUnderLineFont.func  = &doUnderLineFont;
  dStrikeOutFont.d  = this; dStrikeOutFont.func  = &doStrikeOutFont;
  }


void Display::prepareDisplay( TCchar* face, int fontSize, CDC* pDC, CPrintInfo* pInfo, bool doOutput) {
RECT  winSz;
CWnd* win     = 0;
int   yOffset = 0;
int   cx      = 0;
int   cy      = 0;

  dc = pDC; output = doOutput; printing = false;

  sum.clear(); noPoints = maxHeight = curHeight = y = 0;

  tPos.initialize(); center = right = rightTab.right = false; tPos.clrTabs();

  win = pDC->GetWindow(); win->GetClientRect(&winSz);

  cy = winSz.bottom; cx = winSz.right; tPos.iPos(Margin, cx-Margin); y = yOffset = Margin; setMaxY(y);

  initializeFont(face, fontSize);
  }


void Display::beginPrinting(CDC* pDC, CPrintInfo* pInfo, bool doOutput) {
CPrintDialog* pPD           = 0;                // pointer to print dialog
LPDEVMODE     devMode       = 0;
bool          portrait      = true;
int           h             = 0;
int           w             = 0;
int           yOffset       = 0;
int           xOffset       = 0;
int           cx            = 0;
int           cy            = 0;
int           qtrInch       = 0;
int           sixteenThInch = 0;

  dc = pDC; output = doOutput; printing = true;

  if (pInfo) {
    pInfo->m_bContinuePrinting = true;

    pPD = pInfo->m_pPD;
    if (pPD) {
      devMode = pPD->GetDevMode();
      if (devMode && devMode->dmOrientation & DMORIENT_LANDSCAPE) portrait = false;
      }
    }

  sum.clear(); noPoints = maxHeight = curHeight = y = 0;

  tPos.initialize(); center = right = rightTab.right = false; tPos.clrTabs();

  dc->SetMapMode(MM_TEXT);

  cy      = pDC->GetDeviceCaps(VERTRES);        cx = pDC->GetDeviceCaps(HORZRES);
  h       = pDC->GetDeviceCaps(PHYSICALHEIGHT);  w = pDC->GetDeviceCaps(PHYSICALWIDTH);

  qtrInch = portrait ? w * 10 / 85 / 4 : w * 10 / 110 / 4;

  sixteenThInch = qtrInch / 4;  yOffset = (h - cy) / 2;   xOffset = (w - cx) / 2;

  xOffset = qtrInch + sixteenThInch - xOffset;

  topEdge = y = qtrInch + sixteenThInch - yOffset;                              // + sixteenThInch;

  bottomEdge = cy - y - 3*sixteenThInch;

  tPos.iPos(xOffset, cx - xOffset);
  }


void Display::preparePrinting(TCchar* face, int fontSize)
                                {beginPage = true; endPageFlag = false; initializeFont(face, fontSize);}


void Display::initializeFont(TCchar* face, int fontSize)
                           {font.initialize(face, fontSize, dc); setMetric();}


void Display::setFontSize(int fontSize) {font.setSize(fontSize); setMetric();}


void Display::setMetric() {
TEXTMETRIC metric;

  if (dc->GetTextMetrics(&metric)) {

    curHeight = metric.tmHeight + metric.tmExternalLeading + 2;

    if (!tPos.getCharPos() || curHeight > maxHeight) maxHeight = curHeight;

    toLine = curHeight - metric.tmInternalLeading - metric.tmExternalLeading;

    tPos.lastWidth = metric.tmAveCharWidth;  if (tPos.lastWidth > tPos.width) tPos.width = tPos.lastWidth;
    }
  }



Display& Display::stg(TCchar* s) {

  if (*s) nonBlankLine = true;

  sum += s; return *this;
  }


static Display& doFSize(Display& d, int v) {d.textOut();  d.setFontSize(v);  return d;}


static Display& doPrevFont(Display& d) {d.prevFont();   return d;}


void Display::prevFont() {textOut(); font.prevFont(); setMetric();}


Display& doBoldFont(      Display& d) {d.boldFont(); return d;}

void Display::boldFont() {textOut(); font.setBold();}


Display& doItalicFont(    Display& d) {d.italicFont(); return d;}

void Display::italicFont() {textOut(); font.setItalic();}


Display& doUnderLineFont( Display& d) {d.underLineFont(); return d;}

void Display::underLineFont() {textOut(); font.setUnderLine();}


Display& doStrikeOutFont( Display& d) {d.strikeOutFont(); return d;}

void Display::strikeOutFont() {textOut(); font.setStrikeOut();}


void Display::clrFont() {textOut(); font.uninstall();}



static Display& doEditBox(Display& d, int v) {
int curPos = d.tPos.cursorPos;

  d.rightTab = d.tPos.tab();

  editBoxes.create(d.y, v, curPos+1, d.y-2, d.tPos.cursorPos-curPos-1, d.curHeight+1);

  return d;
  }


static Display& doCenter(Display& d) {d.textOut();  d.center = true;  return d;}



static Display& doRight(Display& d) {d.textOut();  d.right = true;  return d;}



static Display& doTab(Display& d) {d.textOut();  d.rightTab = d.tPos.tab();  return d;}


static Display& doCrlf(Display& d) {

  d.textOut();

  if (d.nonBlankLine) {d.crlf();}

  return d;
  }


void Display::crlf() {

  y += maxHeight; maxHeight = curHeight; setMaxY(y);

  if (printing && y >= bottomEdge) endPageFlag = true;

  tPos.crlf();
  }



static Display& doEndPage(Display& d) {

  if (d.printing && d.topEdge < d.y && d.y < d.bottomEdge) {

    d.textOut();

    d.y = d.bottomEdge; d.endPageFlag = true;

    d.tPos.crlf();
    }

  return d;
  }


static Display& doFlushFtr(Display& d) {

  d.textOut();

  d.y += d.maxHeight; d.maxHeight = d.curHeight;  d.nonBlankLine = false; d.setMaxY(d.y);

  d.tPos.crlf(); return d;
  }


static Display& doBeginLine(Display& d) {
  if (!d.sum.empty() || d.rightTab.right) d.textOut();

  d.points[0].x = d.tPos.cursorPos; d.points[0].y = d.y + d.toLine; d.noPoints = 1;
  return d;
  }


static Display& doEndLine(Display& d) {
  if (!d.sum.empty() || d.center || d.right || d.rightTab.right) d.textOut();

  d.points[1].x = d.tPos.cursorPos; d.points[1].y = d.y + d.toLine;

  if (d.points[0].y == d.points[1].y && d.output) d.dc->Polyline(d.points, 2);

  LOGPEN logPen;
  CPen*   pen = d.dc->GetCurrentPen(); pen->GetLogPen(&logPen);

  d.noPoints = 0; return d;
  }


void Display::textOut() {
CString cs;
int     width = getWidth(sum, cs);
int     pos;
int     posComma;
String  part;

  if (center) {        tPos.centerPos(width);             center         = false;}
  if (right)          {tPos.rightPos(width);              right          = false;}
  if (rightTab.right)
    {tPos.rightTabPos(rightTab, width); rightTab.right = false;}

  if (sum.empty()) return;

  if (beginPage) {noPages++; beginPage = false;}

  if (endPageFlag) {endPageFlag = false; beginPage = true;}

  while (exceedsMargin(sum)) {
    posComma = findLast(',', sum);
    pos      = findLastBlank(sum);

    if (pos < 0 && posComma < 0) {
      if (tPos.pastThreshold()) {crlf(); if (sum[0] == ' ') sum = sum.substr(1); continue;}
      pos = findLastChar(sum); posComma = findLast(',', sum);
      }

    if (pos < 0 && posComma < 0) break;

    if (pos < 0 || (tPos.pastThreshold() && posComma + 2 > pos)) pos = posComma+1;

    part = sum.substr(0, pos); fragmentOut(part); crlf();
    sum = sum[pos] == ' ' ? sum.substr(pos+1) : sum.substr(pos);
    }

  fragmentOut(sum);  sum.clear();
  }



void Display::fragmentOut(String& frag) {
CString cs;
int     width = getWidth(frag, cs);

  if (output) {
    try {if (!dc->TextOut(tPos.cursorPos, y, cs)) {outError(cs); return;}}
    catch (...)                                   {outError(cs); return;}
    }

  tPos.move(width);
  }


int Display::findLastBlank(String& s) {
int     i;
int     lng = s.size();
String  part;
CString cs;
int     lastPos = -1;

  for (i = 0; i < lng; i++) {
    if (s[i] == ' ') {
      part = s.substr(0, i);
      if (exceedsMargin(part)) break;
      lastPos = i;
      }
    }

  return lastPos;
  }


int Display::findLast(Tchar ch, String& s) {
int     i;
int     lng = s.size();
String  part;
CString cs;
int     lastPos = -1;

  for (i = 0; i < lng; i++) {
    if (s[i] == ch) {
      part = s.substr(0, i);
      if (exceedsMargin(part)) break;
      lastPos = i;
      }
    }

  return lastPos;
  }


int Display::findLastChar(String& s) {
int     i;
int     lng = s.size();
String  part;
CString cs;

  for (i = 0; i < lng; i++) {
    part = s.substr(0, i);
    if (exceedsMargin(part)) break;
    }

  return i-1;
  }


bool Display::exceedsMargin(String& s) {
CString cs;

  return tPos.exceedsRtMargin(getWidth(s, cs));
  }



// returns CString and width in current units

int Display::getWidth(String& s, CString& cs) {
CSize sz;
bool  italic = font.getAttr()->italic;

  cs = s;  sz = dc->GetOutputTextExtent(cs);

  if (italic) sz.cx += 2;

  return sz.cx;
  }



void Display::outError(TCchar* stg) {
String err = _T("Unable to output: '"); err += stg; err += _T("'");

  messageBox(err);
  }

