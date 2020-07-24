// Display to Window
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.


#include "stdafx.h"
#include "Display.h"
#include "EditBoxes.h"
#include "MessageBox.h"


static const long maxScroll = 2147483648;
static const int  Margin    = 3;

static DspManip1& setupManip1(DspManip1::Func fn, int val);


DspManip dClrTabs;                      // add to stream to clear tabs: dsp << dClrTabs;
DspManip dCrlf;
DspManip dCR;                           // add to stream to perform a carriage return (only)
DspManip dEndPage;                      // add to stream to terminate a page when printing or do nothing
DspManip dTab;
DspManip dCenter;
DspManip dRight;                        // right align the String following up to the nCrlf
DspManip dBeginLine;
DspManip dEndLine;
DspManip dPrevFont;                     // Restore previous font
DspManip dBoldFont;
DspManip dItalicFont;
DspManip dUnderLineFont;
DspManip dStrikeOutFont;
DspManip dflushFtr;                     // add to stream to terminate a footer when printing

DspManip1& dSetLMargin(int val) {return setupManip1(Display::doSetLMargin, val);}
DspManip1& dSetTab(    int val) {return setupManip1(Display::doSetTab,     val);}
DspManip1& dSetRTab(   int val) {return setupManip1(Display::doSetRTab,    val);}
DspManip1& dFSize(     int val) {return setupManip1(Display::doFSize,      val);}
DspManip1& dEditBox(   int val) {return setupManip1(Display::doEditBox,    val);}

DspManip1& setupManip1(DspManip1::Func fn, int val)
              {NewAlloc(DspManip1);   DspManip1* m = AllocNode;  new(m) DspManip1(fn, val);  return *m;}




Display::Display() {
  clear();

  dCrlf.n           = this; dCrlf.func           = &doCrlf;
  dCR.n             = this; dCR.func             = &doCR;
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


void Display::clear() {

  dc = 0; hz.clear(); vert.clear();  font.clear();

  sum.clear(); noPoints = 0;  points[0] = {0,0};  nonBlankLine = true;

  topMargin = botMargin = leftMargin = rightMargin = 0.0;

  center = right = false;  rightTab.clear();

  footer = false;

  printing = suppress = false;  noPages = 0;  wrapEnabled = true;
  }


Display& Display::append(int   v) {String s;   s.format(_T("%li"),  v); sum += s; return *this; }


Display& Display::append(ulong v) {String s;   s.format(_T("%uli"), v);    sum += s; return *this;}


Display& Display::doEditBox(Display& d, int v) {
int curPos = d.hz.currentPos();
int curY   = d.vert.pos();

  d.hz.tab(d.rightTab);

  editBoxes.create(curY, v, curPos+1, curY-2, d.hz.currentPos() - curPos-1, d.chHeight()+1);

  return d;
  }


void Display::crlf() {

//  if (!footer && printing && vert.exceedsBnd(1)) {setEndPage(); return;}

  if (vert.lf(printing, footer)) hz.cr();
  }


bool Display::doEndPageChk() {
  if (printing && vert.isEndPage())
    {vert.atEndPageCond();  return true;}
  return false;
  }

void Display::atEndPageCond() {if (printing) vert.atEndPageCond();}


Display& Display::doEndPage(Display& d) {

  if (d.printing && d.vert.withinBounds()) {

    if (!d.textOut()) return d;

    d.setEndPage();

    d.hz.cr();
    }

  return d;
  }


Display& Display::doFlushFtr(Display& d) {

  d.textOut();

  d.nonBlankLine = false;   d.vert.lf(d.printing, d.footer);   d.hz.cr(); return d;
  }


Display& Display::doBeginLine(Display& d) {

  if (!d.sum.empty() || d.rightTab.right) if (!d.textOut()) return d;

  d.points[0].x = d.hz.currentPos(); d.points[0].y = d.vert.getUlinePos(); d.noPoints = 1;

  return d;
  }


Display& Display::doEndLine(Display& d) {
  if (!d.sum.empty() || d.center || d.right || d.rightTab.right) if (!d.textOut()) return d;

  d.points[1].x = d.hz.currentPos(); d.points[1].y = d.vert.getUlinePos();

  if (d.points[0].y == d.points[1].y && !d.suppress) d.dc->Polyline(d.points, 2);

  LOGPEN logPen;
  CPen*   pen = d.dc->GetCurrentPen(); pen->GetLogPen(&logPen);

  d.noPoints = 0; return d;
  }


void Display::prepareDisplay( TCchar* face, int fontSize, CDC* pDC) {
RECT  winSz;
CWnd* win     = 0;
int   cx      = 0;
int   cy      = 0;

  dc = pDC; printing = false;

  sum.clear(); noPoints = 0;

  center = right = rightTab.right = false;

  win = pDC->GetWindow(); win->GetClientRect(&winSz);

  cy = winSz.bottom; cx = winSz.right;

  initializeFont(face, fontSize);

  vert.setAttributes(cy, topMargin, botMargin);

  hz.setAttributes(cx, leftMargin + 0.3, rightMargin + 0.3);    hz.clrTabs();
  }


void Display::preparePrinting(TCchar* face, int fontSize, CDC* pDC, CPrintInfo* pInfo) {

  dc = pDC; printing = true;   suppress = false;

  sum.clear(); noPoints = 0;

  center = right = rightTab.right = false;

  vert.setBeginPage();

  initializeFont(face, fontSize);   initializeFrame(pInfo);
  }


void Display::initializeFrame(CPrintInfo* pInfo) {
int           cx            = 0;
int           cy            = 0;

  if (pInfo) pInfo->m_bContinuePrinting = true;

  cy      = dc->GetDeviceCaps(VERTRES);        cx = dc->GetDeviceCaps(HORZRES);

  vert.setAttributes(cy, topMargin, botMargin);
  hz.setAttributes(cx, leftMargin, rightMargin);    hz.clrTabs();
  }


bool Display::isPortrait(CPrintInfo* pInfo) {
CPrintDialog* pPD     = pInfo->m_pPD;                  // pointer to print dialog
LPDEVMODE     devMode;

  if (pPD) {
    devMode = pPD->GetDevMode();
    if (devMode && devMode->dmOrientation & DMORIENT_LANDSCAPE) return false;
    }

  return true;
  }


void Display::initializeFont(TCchar* face, int fontSize)
                                 {font.initialize(face, fontSize, dc); setMetric();}
void Display::setFontSize(int v) {if (textOut()) {font.setSize(v);     setMetric();}}
void Display::boldFont()         {if (textOut()) {font.setBold();      setMetric();}}
void Display::italicFont()       {if (textOut()) {font.setItalic();    setMetric();}}
void Display::underLineFont()    {if (textOut()) {font.setUnderLine(); setMetric();}}
void Display::strikeOutFont()    {if (textOut()) {font.setStrikeOut(); setMetric();}}
void Display::prevFont()         {if (textOut()) {font.pop();          setMetric();}}


void Display::setMetric() {hz.setAvgChWidth(dc);    vert.setHeight(dc);}


Display& Display::append(Wrap& w) {
WrapIter iter(w);
int     i;
String* s;

  if (w.isEmpty()) return *this;

  if (printing && !footer && vert.exceedsBnd(w.noLines())) {setEndPage(); return *this;}

  hz.saveCurPos();

  for (i = 0, s = iter(); s; i++, s = iter++) {
    if (i) {crlf(); hz.restorePos();}
    sum = *s;  textOut();
    }

  return *this;
  }


bool Display::textOut() {
int      wth = width(sum);
Wrap     wrap(wrapEnabled);
WrapIter iter(wrap);
int      i;
int      nLines;
String*  s;

  if (center) {hz.centerText(wth); center = false; hz.centerText(wth);}
  if (right)  {hz.rightText(wth);  right  = false; hz.rightText(wth);}
  if (rightTab.right)
              {hz.rightTabText(rightTab, wth); rightTab.right = false;}

  if (sum.empty()) return true;

  if (vert.isBeginPage()) noPages++;

  vert.atEndPageCond();

  wrap.initialize(dc, hz.remaining(), font.getAttr().italic);

  nLines = wrap(sum);

  if (!footer && printing && vert.exceedsBnd(nLines)) {setEndPage(); return false;}

  hz.saveCurPos();

  if (footer) vert.setBottom();

  for (i = 0, s = iter(); s; i++, s = iter++) {
    if (i) {crlf(); hz.restorePos();}

    fragmentOut(*s);
    }

  sum.clear();  return true;
  }


void Display::fragmentOut(String& frag) {
CString cs;  cs = frag;

  if (!suppress) {

    try {if (!dc->TextOut(hz.currentPos(), vert.pos(), cs)) {outError(cs); return;}}
    catch (...)                                             {outError(cs); return;}
    }

  hz.move(width(frag));
  }



// returns CString and width in current units

int Display::width(String& s) {
CString cs;
CSize   sz;
bool    italic = font.getAttr().italic;

  cs = s;   sz = dc->GetOutputTextExtent(cs);

  return italic ? sz.cx + 2 : sz.cx;
  }



void Display::outError(TCchar* stg) {
String err = _T("Unable to output: '"); err += stg; err += _T("'");

  messageBox(err);
  }

