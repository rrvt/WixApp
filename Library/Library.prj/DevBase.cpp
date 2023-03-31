// DevBase to Window
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.


#include "pch.h"
#include "DevBase.h"
#include "EditBoxes.h"


static const long maxScroll = 2147483647;
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
DspManip dFont;
DspManip dBold;
DspManip dItalic;
DspManip dUnderLine;
DspManip dStrikeOut;
DspManip dFlushFtr;                     // add to stream to terminate a footer when printing
DspManip dFlush;                        // flush text so tht the next sequence may be setup


DsManipStg&   dFFace( TCchar* val) {return setupManipStg(DevBase::doFFace,      val);}
DsManipDbl&   dFSize(  double val) {return setupManipDbl(DevBase::doFSize,      val);}
DsManipInt&   dSetLMargin(int val) {return setupManipInt(DevBase::doSetLMargin, val);}
DsManipInt&   dSetTab(    int val) {return setupManipInt(DevBase::doSetTab,     val);}
DsManipInt&   dSetRTab(   int val) {return setupManipInt(DevBase::doSetRTab,    val);}


// Setup a Node with the data, then the << operator implements the function specified in the node

DsManipInt& setupManipInt(DsManipInt::Func fn, int val)
                      {NewAlloc(DsManipInt);   DsManipInt* m = AllocNode;   m->set(fn, val);   return *m;}
DevBase& DevBase::operator<< (DsManipInt& m)
                      {m.func(*this, m.v);   NewAlloc(DsManipInt);   FreeNode(&m); return *this;}

DsManipDbl& setupManipDbl(DsManipDbl::Func fn, double val)
                      {NewAlloc(DsManipDbl);   DsManipDbl* m = AllocNode;   m->set(fn, val);   return *m;}
DevBase& DevBase::operator<< (DsManipDbl& m)
                      {m.func(*this, m.v);   NewAlloc(DsManipDbl);   FreeNode(&m); return *this;}

DsManipStg& setupManipStg(DsManipStg::Func fn, TCchar* val)
                      {NewAlloc(DsManipStg);   DsManipStg* m = AllocNode;   m->set(fn, val);   return *m;}
DevBase& DevBase::operator<< (DsManipStg& m)
                      {m.func(*this, m.v);   NewAlloc(DsManipStg);   FreeNode(&m); return *this;}


DevBase::DevBase() : vert(dvx), txtStk(*this), txt(txtStk.push()) {
  clear();

  dFont.n           = this; dFont.func           = &doPrev;
  dBold.n           = this; dBold.func           = &doBold;
  dItalic.n         = this; dItalic.func         = &doItalic;
  dUnderLine.n      = this; dUnderLine.func      = &doUnderLine;
  dStrikeOut.n      = this; dStrikeOut.func      = &doStrikeOut;

  dClrTabs.n        = this; dClrTabs.func        = &doClrTabs;
  dTab.n            = this; dTab.func            = &doTab;

  dCenter.n         = this; dCenter.func         = &doCenter;
  dRight.n          = this; dRight.func          = &doRight;

  dBeginLine.n      = this; dBeginLine.func      = &doBeginLine;
  dEndLine.n        = this; dEndLine.func        = &doEndLine;

  dCrlf.n           = this; dCrlf.func           = &doCrlf;
  dCR.n             = this; dCR.func             = &doCR;
  dFlushFtr.n       = this; dFlushFtr.func       = &doFlushFtr;
  dFlush.n          = this; dFlush.func          = &doFlush;

  dEndPage.n        = this; dEndPage.func        = &doEndPage;
  }


void DevBase::clear() {txt->clear();   vert.clear();   noPages = 0;}


void DevBase::startContext() {

  dvx.saveDvx(txt->bkp);   txt->bkMaxHeight = vert.maxHeight;

  txt = txtStk.push();
  }


void DevBase::endContext() {
  txt = txtStk.pop();

  vert.maxHeight = txt->bkMaxHeight;   dvx.restoreDvx(txt->bkp);   dvx.restoreFontCtx();
  }


void DevBase::initFont(TCchar* face, double fontSize) {dvx.setBaseFont(face, fontSize);}


void DevBase::setHorzMgns(double left, double right)
                                       {dvx.leftMgn = left;   dvx.rightMgn = right;    txt->initBounds();}


void DevBase::setVertMgns(double top,  double bot)
                                       {dvx.topMgn  = top;     dvx.botMgn  = bot;      vert.initBounds();}


DevBase& DevBase::append(int   v) {txt->append(v); return *this; }


DevBase& DevBase::append(ulong v) {txt->append(v); return *this;}


DevBase& DevBase::operator<< (NoteNmbr& nn) {

  if (!nn.width) {String s = nn.stg();  stg(s);}
  else {txt->set(nn);  (*txt)(NmbrAftr);}

  return *this;
  }


void DevBase::tab() {txt->tab();}


void DevBase::findNextTab() {txt->findNextTab();}



DevBase& DevBase::doFlushFtr(DevBase& d) {

  d.doFlush(d);

  d.vert.lf(d.printing, d.footer);   d.txt->cr();   return d;
  }


DevBase& DevBase::doFlush(DevBase& d) {d.txt->flush(); return d;}


DevBase& DevBase::doBeginLine(DevBase& d) {d.txt->doBeginULine(d.vert.getUlinePos());   return d;}


DevBase& DevBase::doEndLine(DevBase& d) {d.txt->doEndULine(d.vert.getUlinePos());   return d;}


DevBase& DevBase::doEndPage(DevBase& d)
                            {if (d.printing && d.vert.withinBounds()) (*d.txt)(DoEndPgAftr);   return d;}


bool DevBase::isPortrait(CPrintInfo* pInfo) {
CPrintDialog* pPD = pInfo->m_pPD;                  // pointer to print dialog
LPDEVMODE     devMode;

  if (pPD) {
    devMode = pPD->GetDevMode();
    if (devMode && devMode->dmOrientation & DMORIENT_LANDSCAPE) return false;
    }

  return true;
  }


void DevBase::setFontFace(TCchar* face) {(*txt)(FaceAftr, 0, face);}
void DevBase::setFontSize(double v)     {(*txt)(SizeAftr, v);}
void DevBase::bold()                    {(*txt)(BoldAftr);}
void DevBase::italic()                  {(*txt)(ItalicAftr);}
void DevBase::underLine()               {(*txt)(UnderLineAftr);}
void DevBase::strikeOut()               {(*txt)(StrikeOutAftr);}
void DevBase::prev()                    {(*txt)(PopAftr);}



