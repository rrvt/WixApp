// Device to Window
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.


#include "stdafx.h"
#include "Device.h"
#include "EditBoxes.h"
#include "MessageBox.h"


static const long maxScroll = 2147483648;
static const int  Margin    = 3;

static DsManipInt& setupManipInt(DsManipInt::Func fn, int     val);
static DsManipDbl& setupManipDbl(DsManipDbl::Func fn, double  val);
static DsManipStg& setupManipStg(DsManipStg::Func fn, TCchar* val);


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
DspManip dFlushFtr;                     // add to stream to terminate a footer when printing
DspManip dFlush;                        // flush text so tht the next sequence may be setup

DsManipInt&   dSetLMargin(int val) {return setupManipInt(Device::doSetLMargin, val);}
DsManipInt&   dSetTab(    int val) {return setupManipInt(Device::doSetTab,     val);}
DsManipInt&   dSetRTab(   int val) {return setupManipInt(Device::doSetRTab,    val);}
DsManipStg&   dFFace( TCchar* val) {return setupManipStg(Device::doFFace,      val);}
DsManipDbl&   dFSize(  double val) {return setupManipDbl(Device::doFSize,      val);}
DsManipInt&   dEditBox(   int val) {return setupManipInt(Device::doEditBox,    val);}

DsManipInt& setupManipInt(DsManipInt::Func fn, int val)
            {NewAlloc(DsManipInt);   DsManipInt* m = AllocNode;  m->set(fn, val); return *m;}

DsManipDbl& setupManipDbl(DsManipDbl::Func fn, double val)
            {NewAlloc(DsManipDbl);   DsManipDbl* m = AllocNode;  m->set(fn, val); return *m;}

DsManipStg& setupManipStg(DsManipStg::Func fn, TCchar* val)
            {NewAlloc(DsManipStg);   DsManipStg* m = AllocNode;  m->set(fn, val); return *m;}



Device::Device(TCchar* src) : id(src) {
  clear();

  dCrlf.n           = this; dCrlf.func           = &doCrlf;
  dCR.n             = this; dCR.func             = &doCR;
  dEndPage.n        = this; dEndPage.func        = &doEndPage;
  dFlushFtr.n       = this; dFlushFtr.func       = &doFlushFtr;
  dFlush.n          = this; dFlush.func          = &doFlush;
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


void Device::clear() {

  dc = 0; hz.clear(); vert.clear();  font.clear();

  sum.clear(); noPoints = 0;  points[0] = {0,0};  nonBlankLine = true;

  topMargin = botMargin = leftMargin = rightMargin = 0.0;

  center = right = false;  rightTab.clear();

  footer = false;

  printing = suppress = false;  noPages = 0;  wrapEnabled = false;
  }


Device& Device::append(int   v) {String s;   s.format(_T("%li"),  v); sum += s; return *this; }


Device& Device::append(ulong v) {String s;   s.format(_T("%uli"), v);    sum += s; return *this;}


Device& Device::doEditBox(Device& d, int v) {
int curPos = d.hz.currentPos();
int curY   = d.vert.pos();

  d.hz.tab(d.rightTab);

  editBoxes.create(curY, v, curPos+1, curY-2, d.hz.currentPos() - curPos-1, d.chHeight()+1);

  return d;
  }


void Device::crlf() {if (vert.lf(printing, footer)) hz.cr();}


bool Device::doEndPageChk() {
  if (printing && vert.isEndPage()) {vert.atEndPageCond();  return true;}
  return false;
  }

void Device::atEndPageCond() {if (printing) vert.atEndPageCond();}


Device& Device::doEndPage(Device& d) {

  if (d.printing && d.vert.withinBounds()) {

    if (!d.textOut()) return d;

    d.setEndPage();

    d.hz.cr();
    }

  return d;
  }


Device& Device::doFlushFtr(Device& d) {

//  d.textOut();   d.nonBlankLine = false;
  d.doFlush(d);

  d.vert.lf(d.printing, d.footer);   d.hz.cr(); return d;
  }


Device& Device::doFlush(Device& d) {d.textOut();  d.nonBlankLine = false; return d;}


Device& Device::doBeginLine(Device& d) {

  if (!d.sum.empty() || d.rightTab.right) if (!d.textOut()) return d;

  d.points[0].x = d.hz.currentPos(); d.points[0].y = d.vert.getUlinePos(); d.noPoints = 1;

  return d;
  }


Device& Device::doEndLine(Device& d) {
  if (!d.sum.empty() || d.center || d.right || d.rightTab.right) if (!d.textOut()) return d;

  d.points[1].x = d.hz.currentPos(); d.points[1].y = d.vert.getUlinePos();

  if (d.points[0].y == d.points[1].y && !d.suppress) d.dc->Polyline(d.points, 2);

  LOGPEN logPen;
  CPen*   pen = d.dc->GetCurrentPen(); pen->GetLogPen(&logPen);

  d.noPoints = 0; return d;
  }


void Device::prepareDisplay( TCchar* face, double fontSize, CDC* pDC) {
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


void Device::preparePrinting(TCchar* face, double fontSize, CDC* pDC, CPrintInfo* pInfo) {

  dc = pDC; printing = true;   suppress = false;

  sum.clear(); noPoints = 0;

  center = right = rightTab.right = false;

  vert.setBeginPage();

  initializeFont(face, fontSize);   initializeFrame(pInfo);
  }


void Device::initializeFrame(CPrintInfo* pInfo) {
int           cx            = 0;
int           cy            = 0;

  if (pInfo) pInfo->m_bContinuePrinting = true;

  cy      = dc->GetDeviceCaps(VERTRES);        cx = dc->GetDeviceCaps(HORZRES);

  vert.setAttributes(cy, topMargin, botMargin);
  hz.setAttributes(cx, leftMargin, rightMargin);    hz.clrTabs();
  }


bool Device::isPortrait(CPrintInfo* pInfo) {
CPrintDialog* pPD     = pInfo->m_pPD;                  // pointer to print dialog
LPDEVMODE     devMode;

  if (pPD) {
    devMode = pPD->GetDevMode();
    if (devMode && devMode->dmOrientation & DMORIENT_LANDSCAPE) return false;
    }

  return true;
  }


void Device::initializeFont(TCchar* face, double fontSize)
                                           {font.set(face, fontSize, dc);    setMetric();}
void Device::setFontFace(TCchar* face) {if (textOut()) {font.setFace(face);  setMetric();}}
void Device::setFontSize(double v)     {if (textOut()) {font.setSize(v);     setMetric();}}
void Device::boldFont()                {if (textOut()) {font.setBold();      setMetric();}}
void Device::italicFont()              {if (textOut()) {font.setItalic();    setMetric();}}
void Device::underLineFont()           {if (textOut()) {font.setUnderLine(); setMetric();}}
void Device::strikeOutFont()           {if (textOut()) {font.setStrikeOut(); setMetric();}}
void Device::prevFont()                {if (textOut()) {font.pop();          setMetric();}}


void Device::setMetric() {hz.setAvgLgChWidth(dc);    vert.setHeight(dc);}


Device& Device::append(Wrap& w) {
WrapIter iter(w);
WrapData* wd;
int     i;

  if (w.isEmpty()) return *this;

  if (printing && !footer && vert.exceedsBnd(w.noLines())) {setEndPage(); return *this;}

  hz.saveCurPos();

  for (i = 0, wd = iter(); wd; i++, wd = iter++) {

    if (i) {crlf(); hz.restorePos();}

    sum = wd->line;  textOut();
    }

  return *this;
  }


bool Device::textOut() {
int       wth = width(sum);
Wrap      wrap(wrapEnabled);
WrapIter  iter(wrap);
WrapData* wd;
int       i;
int       nLines;

  if (center) {hz.centerText(wth); center = false; hz.centerText(wth);}
  if (right)  {hz.rightText(wth);  right  = false; hz.rightText(wth);}
  if (rightTab.right)
              {hz.rightTabText(rightTab, wth); rightTab.right = false;}

  if (sum.empty()) return true;

  if (vert.isBeginPage()) noPages++;

  vert.atEndPageCond();

  if (!hz.isAtLftBnd()) {
    String t;
    while (sum[0] == _T(' ') || sum[0] == _T('-')) {
      t += sum[0];  sum = sum.substr(1);
      }
    if (!t.isEmpty()) fragmentOut(t);
    }

  wrap.initialize(dc, hz.remaining(), hz.maxExtent(), isFontItalic());

  nLines = wrap(sum);

  if (!footer && printing && vert.exceedsBnd(nLines)) {setEndPage(); return false;}

  hz.saveCurPos();

  if (footer) vert.setBottom();

  for (i = 0, wd = iter(); wd; i++, wd = iter++) {

    if      (i)            {crlf(); if (wd->indented) hz.restorePos();}
    else if (!wd->indented) crlf();

    fragmentOut(wd->line);
    }

  sum.clear();  return true;
  }


void Device::fragmentOut(String& frag) {
CString cs;  cs = frag;

  if (!suppress) {

    try {if (!dc->TextOut(hz.currentPos(), vert.pos(), cs)) {outError(cs); return;}}
    catch (...)                                             {outError(cs); return;}
    }

  hz.move(width(frag));
  }



// returns CString and width in current units

int Device::width(String& s) {
CString cs;
CSize   sz;
bool    italic = isFontItalic();

  cs = s;   sz = dc->GetOutputTextExtent(cs);

  return italic ? sz.cx + 2 : sz.cx;
  }



void Device::outError(TCchar* stg) {
String err = _T("Unable to output: '"); err += stg; err += _T("'");

  messageBox(err);
  }

