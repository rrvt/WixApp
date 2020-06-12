// Display to Window
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.


#include "stdafx.h"
#include "Display.h"
#include "EditBoxes.h"

#include "MessageBox.h"


static const long maxScroll = 2147483648;
static const int  Margin    = 3;

static DspManip1& setupManip1(DspManip1::Func fn, int val);


DspManip dClrTabs;    // add to stream to clear tabs: dsp << dClrTabs;
DspManip dCrlf;
DspManip dEndPage;    // add to stream to terminate a page when printing or do nothing
DspManip dTab;
DspManip dCenter;
DspManip dRight;      // right align the String following up to the nCrlf
DspManip dBeginLine;
DspManip dEndLine;
DspManip dPrevFont;   // Restore previous font
DspManip dBoldFont;
DspManip dItalicFont;
DspManip dUnderLineFont;
DspManip dStrikeOutFont;
DspManip dflushFtr;   // add to stream to terminate a footer when printing

DspManip1& dSetLMargin(int val) {return setupManip1(Display::doSetLMargin, val);}
DspManip1& dSetTab(    int val) {return setupManip1(Display::doSetTab,     val);}
DspManip1& dSetRTab(   int val) {return setupManip1(Display::doSetRTab,    val);}
DspManip1& dFSize(     int val) {return setupManip1(Display::doFSize,      val);}
DspManip1& dEditBox(   int val) {return setupManip1(Display::doEditBox,    val);}

DspManip1& setupManip1(DspManip1::Func fn, int val)
              {NewAlloc(DspManip1);   DspManip1* m = AllocNode;  new(m) DspManip1(fn, val);  return *m;}



Display::Display()          : tPos(), initialYPos(0),   points{{0,0}}    {initialize();}
Display::Display(int initY) : tPos(), initialYPos(initY), points{{0,0}} {initialize();}


void Display::initialize() {

  noPoints = maxHeight = curHeight = toLine = maxY = topEdge = bottomEdge = 0;   y = initialYPos + Margin;
  center = right = beginPage = endPageFlag = false;
  dc = 0; tPos.clrTabs();
  points[0] = {0,0};

  output = printing = false;   noPages = 0;  nonBlankLine = true;

  dCrlf.n           = this; dCrlf.func           = &doCrlf;
  dEndPage.n        = this; dEndPage.func        = &doEndPage;
  dflushFtr.n       = this; dflushFtr.func       = &doFlushFtr;
  dTab.n            = this; dTab.func            = &doTab;
  dClrTabs.n        = this; dClrTabs.func        = &doClrTabs;
  dCenter.n         = this; dCenter.func         = &doCenter;
  dRight.n          = this; dRight.func          = &doRight;
  dBeginLine.n      = this; dBeginLine.func      = &doBeginLine;
  dEndLine.n        = this; dEndLine.func        = &doEndLine;

  dPrevFont.n       = this; dPrevFont.func       = &doPrevFont;
  dBoldFont.n       = this; dBoldFont.func       = &doBoldFont;
  dItalicFont.n     = this; dItalicFont.func     = &doItalicFont;
  dUnderLineFont.n  = this; dUnderLineFont.func  = &doUnderLineFont;
  dStrikeOutFont.n  = this; dStrikeOutFont.func  = &doStrikeOutFont;
  }




Display& Display::doEditBox(Display& d, int v) {
int curPos = d.tPos.cursorPos;

  d.rightTab = d.tPos.tab();

  editBoxes.create(d.y, v, curPos+1, d.y-2, d.tPos.cursorPos-curPos-1, d.curHeight+1);

  return d;
  }


void Display::crlf() {

  y += maxHeight; maxHeight = curHeight; setMaxY(y);

  if (printing && y >= bottomEdge)
    endPageFlag = true;

  tPos.crlf();
  }



Display& Display::doEndPage(Display& d) {

  if (d.printing && d.topEdge < d.y && d.y < d.bottomEdge) {

    d.textOut();

    d.y = d.bottomEdge; d.endPageFlag = true;

    d.tPos.crlf();
    }

  return d;
  }


Display& Display::doFlushFtr(Display& d) {

  d.textOut();

  d.y += d.maxHeight; d.maxHeight = d.curHeight;  d.nonBlankLine = false; d.setMaxY(d.y);

  d.tPos.crlf(); return d;
  }


Display& Display::doBeginLine(Display& d) {
  if (!d.sum.empty() || d.rightTab.right) d.textOut();

  d.points[0].x = d.tPos.cursorPos; d.points[0].y = d.y + d.toLine; d.noPoints = 1;
  return d;
  }


Display& Display::doEndLine(Display& d) {
  if (!d.sum.empty() || d.center || d.right || d.rightTab.right) d.textOut();

  d.points[1].x = d.tPos.cursorPos; d.points[1].y = d.y + d.toLine;

  if (d.points[0].y == d.points[1].y && d.output) d.dc->Polyline(d.points, 2);

  LOGPEN logPen;
  CPen*   pen = d.dc->GetCurrentPen(); pen->GetLogPen(&logPen);

  d.noPoints = 0; return d;
  }


void Display::prepareDisplay( TCchar* face, int fontSize, CDC* pDC, bool doOutput) {
RECT  winSz;
CWnd* win     = 0;
int   cx      = 0;
int   cy      = 0;

  dc = pDC; output = doOutput; printing = false;

  sum.clear(); noPoints = maxHeight = curHeight = 0;   y = initialYPos + Margin;

  tPos.initialize(); center = right = rightTab.right = false; tPos.clrTabs();

  win = pDC->GetWindow(); win->GetClientRect(&winSz);

  cy = winSz.bottom; cx = winSz.right; tPos.iPos(Margin, cx-Margin); setMaxY(y);

  dc->SetMapMode(MM_TEXT);  initializeFont(face, fontSize);
  }


void Display::preparePrinting(TCchar* face, int fontSize, CDC* pDC, bool doOutput) {

  dc = pDC; output = doOutput; printing = true;

  sum.clear(); noPoints = maxHeight = curHeight = y = 0;

  tPos.initialize(); center = right = rightTab.right = false; tPos.clrTabs();

  beginPage = true; endPageFlag = false;

  dc->SetMapMode(MM_TEXT);  initializeFont(face, fontSize);
  }


void Display::beginPrinting(CPrintInfo* pInfo) {
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

  if (pInfo) {
    pInfo->m_bContinuePrinting = true;

    pPD = pInfo->m_pPD;
    if (pPD) {
      devMode = pPD->GetDevMode();
      if (devMode && devMode->dmOrientation & DMORIENT_LANDSCAPE) portrait = false;
      }
    }

  cy      = dc->GetDeviceCaps(VERTRES);        cx = dc->GetDeviceCaps(HORZRES);
  h       = dc->GetDeviceCaps(PHYSICALHEIGHT);  w = dc->GetDeviceCaps(PHYSICALWIDTH);

  qtrInch = portrait ? w * 10 / 85 / 4 : w * 10 / 110 / 4;

  sixteenThInch = qtrInch / 4;  yOffset = (h - cy) / 2;   xOffset = (w - cx) / 2;

  xOffset = qtrInch + sixteenThInch - xOffset;

  topEdge = y = qtrInch + sixteenThInch - yOffset;                              // + sixteenThInch;

  bottomEdge = cy - y - 3*sixteenThInch;

  tPos.iPos(xOffset, cx - xOffset);
  }


void Display::initializeFont(TCchar* face, int fontSize)
                                        {font.initialize(face, fontSize, dc); setMetric();}


void Display::setFontSize(int fontSize) {font.setSize(fontSize); setMetric();}


void Display::setMetric() {
TEXTMETRIC metric;
int        buf[26];
int        i;
double     sum;
double     avg;
int        chWidth;

  if (dc->GetTextMetrics(&metric)) {

    dc->GetCharWidth(_T('A'), _T('Z'), buf);
    for (i = 0, sum = 0; i < noElements(buf); i++) sum += buf[i];
    avg = sum / noElements(buf) + 0.5;   chWidth = (int) avg;

    if (metric.tmAveCharWidth > chWidth) chWidth = metric.tmAveCharWidth;

    curHeight = metric.tmHeight + metric.tmExternalLeading + 2;

    if (!tPos.getCharPos() || curHeight > maxHeight) maxHeight = curHeight;

    toLine = curHeight - metric.tmInternalLeading - metric.tmExternalLeading;

    if (chWidth > tPos.width) tPos.width = chWidth;    tPos.lastWidth = chWidth;
    }
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

  cs = s;   sz = dc->GetOutputTextExtent(cs);

  if (italic) sz.cx += 2;

  return sz.cx;
  }



void Display::outError(TCchar* stg) {
String err = _T("Unable to output: '"); err += stg; err += _T("'");

  messageBox(err);
  }

