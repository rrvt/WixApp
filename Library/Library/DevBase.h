// Output DevBase to Window
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.

#pragma once

#include "DevCtx.h"
#include "ManipT.h"
#include "TxtOut.h"
#include "VertMgmt.h"


typedef ManipT<   DevBase> DspManip;
typedef ManipIntT<DevBase> DsManipInt;
typedef ManipDblT<DevBase> DsManipDbl;
typedef ManipStgT<DevBase> DsManipStg;


class DevBase {
protected:

DevCtx    dvx;                      // Device Context and all details related to DCD

TxtOutStk txtStk;                   // Text Output Context Stack
TxtOut*   txt;                      // Current Text Output context

VertMgmt  vert;                     // Manage vertical movement in window or page

bool      footer;                   // ???

public:

bool      printing;                      // Printing when true, else displaying
uint      noPages;

                   DevBase();
                  ~DevBase() {}

          void     clear();

  virtual void     prepare(CDC* dc, CPrintInfo* pInfo) { }
  virtual void     initFont(TCchar* face, double fontSize);

          void     initTxt() {txt->initialize();}
//          void     initPageSize();
//          void     initPageSize() {txt->initialize();}
          void     setHorzMgns(double left, double right);
          void     setVertMgns(double top,  double bot);

          void     setBeginPage() {vert.setBeginPage();}
          void     incPageNo() {if (vert.isBeginPage()) noPages++;}
          void     startContext();
          void     endContext();

          void     setFooter() {footer = true;   startContext();   if (printing) vert.setBottom();}
          void     clrFooter() {footer = false;    endContext();   txt->nonBlankLine = false;}

          void     lf() {vert.lf(printing, footer);}

          double   getFontScale()      {return dvx.scale;}
          void     setFontScale(double scale) {dvx.scale = scale;}

          void     clearTabs()       {txt->clearTabs();}
          DevTabs& getTabs()         {return txt->getTabs();}
          void     restoreTabs(DevTabs& tm) {txt->restoreTabs(tm);}

          void     disableWrap() {txt->wrapEnabled = false;}
          void     enableWrap()  {txt->wrapEnabled = true;}
          void     cleanupWrap() {txt->cleanupWrap();}
          void     clearOps()    {txt->clearOps();}

          void     suppressOutput() {dvx.suppress = true;}
          void     negateSuppress() {dvx.suppress = false;}

          CDC*     getDC() {return dvx.get();}
          int      chHeight() {return vert.heightCh();}
          int      lgChWidth(){return dvx.lgChWidth();}

          DevBase& operator<< (NoteNmbr&  nn);
          DevBase& operator<< (String&     s) {return stg(s);}
          DevBase& operator<< (TCchar*     s) {return stg(s);}
          DevBase& operator<< (int         v) {return append(v);}
          DevBase& operator<< (ulong       v) {return append(v);}
          DevBase& operator<< (DspManip&   m) {return m.func(*this);}
          DevBase& operator<< (DsManipInt& m);
          DevBase& operator<< (DsManipDbl& m);
          DevBase& operator<< (DsManipStg& m);

  virtual bool     doEndPageChk() {return false;}
  virtual void     atEndPageCond() { }
          bool     isEndPage() {return vert.isEndPage();}
          bool     isWrapFin() {return txt->isWrapFin();}

          void      examineCurFont(TCchar* tc) {dvx.examineCurFont(tc);}

protected:
         bool     isPortrait(CPrintInfo* pInfo);

  static DevBase& doSetLMargin(    DevBase& d, int v)     {(*d.txt)(LeftAftr, v); return d;}

  static DevBase& doClrTabs(       DevBase& d)            {(*d.txt)(clrTbsAftr);  return d;}
  static DevBase& doTab(           DevBase& d)            {(*d.txt)(TabAftr);     return d;}

  static DevBase& doCrlf(          DevBase& d)            {(*d.txt)(DoCrlfAftr);  return d;}
  static DevBase& doCR(            DevBase& d)            {(*d.txt)(CRAftr);      return d;}

  static DevBase& doEndPage(       DevBase& d);

  static DevBase& doCenter(        DevBase& d)            {(*d.txt)(CenterAftr);  return d;}
  static DevBase& doRight(         DevBase& d)            {(*d.txt)(RightAftr);   return d;}
  static DevBase& doBeginLine(     DevBase& d);
  static DevBase& doEndLine(       DevBase& d);

  static DevBase& doBold(          DevBase& d)            {d.bold();              return d;}
  static DevBase& doItalic(        DevBase& d)            {d.italic();            return d;}
  static DevBase& doUnderLine(     DevBase& d)            {d.underLine();         return d;}
  static DevBase& doStrikeOut(     DevBase& d)            {d.strikeOut();         return d;}
  static DevBase& doPrev(          DevBase& d)            {d.prev();              return d;}

  static DevBase& doFlushFtr(      DevBase& d);
  static DevBase& doFlush(         DevBase& d);

  static DevBase& doSetTab(        DevBase& d, int     v) {d.txt->setTab(v);      return d;}
  static DevBase& doSetRTab(       DevBase& d, int     v) {d.txt->setRTab(v);     return d;}
  static DevBase& doFFace(         DevBase& d, String& v) {d.setFontFace(v);      return d;}
  static DevBase& doFSize(         DevBase& d, double  v) {d.setFontSize(v);      return d;}


          DevBase& stg(TCchar*  s)                        {txt->stg(s);           return *this;}
          DevBase& stg(String&  s)                        {txt->stg(s);           return *this;}
          DevBase& append(int   v);
          DevBase& append(ulong v);

//          DevBase& evalNmbr(NoteNmbr& nmbr);

          void     tab();
          void     findNextTab();
          bool     crlf();

          void     initializeFont(TCchar* face, double fontSize);
          void     setFontFace(TCchar* face);
          void     setFontSize(double v);
          void     bold();
          void     prev();
          void     italic();
          void     underLine();
          void     strikeOut();
          void     setPrevFont();
          void     setMetric();

  virtual void     setEndPage() { }

  friend DsManipInt& dSetLMargin(int val);
  friend DsManipInt& dSetTab(    int val);
  friend DsManipInt& dSetRTab(   int val);
  friend DsManipStg& dFFace( TCchar* val);
  friend DsManipDbl& dFSize(  double val);
  friend DsManipInt& dEditBox(   int val);
  friend class ToDevBase;                     // Debugging Only
  friend class  DspPrtDv;
  friend class  TxtOut;
  friend struct TxtOps;
  };


// No Argument Manipulator

extern DspManip dBold;
extern DspManip dItalic;
extern DspManip dUnderLine;
extern DspManip dStrikeOut;
extern DspManip dFont;

extern DspManip dClrTabs;        // add to stream to clear tabs:                   dsp << dClrTabs;
extern DspManip dCrlf;           // add to stream to terminate a line on display:  dsp << "xyz" << dCrlf;
extern DspManip dCR;             // add to stream to perform a carriage return (only)
extern DspManip dEndPage;        // add to stream to terminate a page when printing or do nothing
extern DspManip dFlush;          // flush text so tht the next sequence may be setup
extern DspManip dFlushFtr;       // add to stream to terminate a footer when printing
extern DspManip dTab;            // add to stream to tab to next tab position:     dsp << dTab << "xyz";
extern DspManip dCenter;         // center the string following up to the nCrlf
extern DspManip dRight;          // right align the string following up to the nCrlf
extern DspManip dBeginLine;      // begin line here
extern DspManip dEndLine;        // end line here -- Does not affect current position

// One Argument Manipulator

DsManipStg& dFFace(TCchar*  val);
DsManipDbl& dFSize(double   val);

DsManipInt& dSetLMargin(int val);
DsManipInt& dSetTab(    int val);
DsManipInt& dSetRTab(   int val);


