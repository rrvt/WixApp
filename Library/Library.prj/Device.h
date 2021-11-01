// Output Device to Window
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.

#pragma once

#include "FontMgrII.h"
#include "Horiz.h"
#include "ManipT.h"
#include "TextPosition.h"
#include "VertMgmt.h"
#include "Wrap.h"


class Device;


typedef ManipT<   Device> DspManip;
typedef ManipIntT<Device> DsManipInt;
typedef ManipDblT<Device> DsManipDbl;
typedef ManipStgT<Device> DsManipStg;


class Device {
String       id;
CDC*         dc;
Horiz        hz;
VertMgmt     vert;
FontMgrII    font;

String       sum;
int          noPoints;
POINT        points[20];
bool         nonBlankLine;                  // true when first non blank line is encountered during
                                            // printing
double       topMargin;
double       botMargin;
double       leftMargin;
double       rightMargin;                   // Right Margin in multiples of character width

bool         center;                        // center string in window when true;
bool         right;                         // right align in window when true;
TabX         rightTab;                      // right align to tab value when true;

bool         footer;

public:

bool         printing;                      // Printing when true, else displaying
bool         suppress;
uint         noPages;
bool         wrapEnabled;

  Device(TCchar* src);
 ~Device() {}

  void     clear();

  void     prepareDisplay( TCchar* face, double fontSize, CDC* pDC);
  void     preparePrinting(TCchar* face, double fontSize, CDC* pDC, CPrintInfo* pInfo);
  void     suppressOutput() {suppress = true;}
  void     negateSuppress() {suppress = false;}
  void     disableWrap()    {wrapEnabled = false;}
  void     enableWrap()     {wrapEnabled = true;}

  void     setFooter() {footer = true; sum.clear(); vert.setBottom();}
  void     clrFooter() {footer = false;}
  void     setHorzMgns(double left, double right) {leftMargin = left; rightMargin = right;}
  void     setVertMgns(double top,  double bot)   {topMargin  = top;   botMargin  = bot;}

  double   getFontScale() {return font.getScale();}
  void     setFontScale(double scl) {font.setScale(scl);}

  Device& operator<< (String&    s) {return stg(s);}
  Device& operator<< (TCchar*    s) {return stg(s);}
  Device& operator<< (Wrap&      w) {return append(w);}
  Device& operator<< (int        v) {return append(v);}
  Device& operator<< (ulong      v) {return append(v);}
  Device& operator<< (DspManip&  m) {return m.func(*this);}
  Device& operator<< (DsManipInt& m)
                                  {NewAlloc(DsManipInt); m.func(*this, m.v); FreeNode(&m); return *this;}
  Device& operator<< (DsManipDbl& m)
                                  {NewAlloc(DsManipDbl); m.func(*this, m.v); FreeNode(&m); return *this;}
  Device& operator<< (DsManipStg& m)
                                  {NewAlloc(DsManipStg); m.func(*this, m.v); FreeNode(&m); return *this;}

  CDC*     getDC()     {return dc;}
  int      remaining() {return hz.remaining();}
  int      maxWidth()  {return hz.maxExtent();}
  int      lgChWidth() {return hz.lgChWidth();}
  int      flChWidth() {return hz.flChWidth();}
  int      chHeight()  {return vert.heightCh();}
  int      maxHeight() {return vert.maxY();}

  bool     isFontItalic() {return font.isFontItalic();}

private:
         void     initializeFrame(CPrintInfo* pInfo);
         bool     isPortrait(CPrintInfo* pInfo);

         void     clrFont()                         {font.clear();}
         void     getMaxPos(long& maxX, long& maxY) {maxY = hz.maxX(); maxY = vert.maxY();}
         bool     withinBounds()                    {return vert.withinBounds();}
         bool     doEndPageChk();
         void     atEndPageCond();
         int      getRemainder()                    {return hz.remaining();}
         int      vertPos()                         {return vert.pos();}
         void     clrLines()                        {vert.clrLines();}
         int      maxLines()                        {return vert.getMaxLines();}

  static Device& doClrTabs(       Device& d) {d.hz.clrTabs(); return d;}
  static Device& doCrlf(          Device& d) {if (d.textOut() && d.nonBlankLine) {d.crlf();}  return d;}
  static Device& doCR(            Device& d) {if (d.textOut()) d.hz.cr(); return d;}

  static Device& doEndPage(       Device& d);
  static Device& doTab(           Device& d) {if (d.textOut()) d.hz.tab(d.rightTab); return d;}

  static Device& doCenter(        Device& d) {if (d.textOut())  d.center = true;  return d;}
  static Device& doRight(         Device& d) {if (d.textOut())  d.right  = true;  return d;}
  static Device& doBeginLine(     Device& d);
  static Device& doEndLine(       Device& d);

  static Device& doBoldFont(      Device& d) {d.boldFont();      return d;}
  static Device& doItalicFont(    Device& d) {d.italicFont();    return d;}
  static Device& doUnderLineFont( Device& d) {d.underLineFont(); return d;}
  static Device& doStrikeOutFont( Device& d) {d.strikeOutFont(); return d;}
  static Device& doPrevFont(      Device& d) {d.prevFont();      return d;}

  static Device& doFlushFtr(      Device& d);
  static Device& doFlush(         Device& d);

  static Device& doSetTab(        Device& d, int v) {d.hz.setTab(v);  return d;}
  static Device& doSetRTab(       Device& d, int v) {d.hz.setRTab(v); return d;}
  static Device& doSetLMargin(    Device& d, int v) {d.textOut(); d.hz.setLeftMargin(v); return d;}
  static Device& doEditBox(       Device& d, int v);
  static Device& doFFace(         Device& d, String& v) {d.setFontFace(v);  return d;}
  static Device& doFSize(         Device& d, double v)  {d.setFontSize(v);  return d;}


  Device& stg(TCchar*  s) {if (*s) nonBlankLine = true;   sum += s; return *this;}
  Device& stg(String&  s) {return this->stg(s.str());}
  Device& append(Wrap& w);
  Device& append(int   v);
  Device& append(ulong v);

  void     crlf();

  void     initializeFont(TCchar* face, double fontSize);
  void     setFontFace(TCchar* face);
  void     setFontSize(double v);
  void     boldFont();
  void     prevFont();
  void     italicFont();
  void     underLineFont();
  void     strikeOutFont();
  void     setPrevFont();
  void     setMetric();

  bool     textOut();                                 // fails when end of page and printing
  void     fragmentOut(String& s);
  int      width(String& s);                          // returns CString and width in current units
  void     outError(TCchar* stg);
  void     setEndPage() {if (printing) vert.setEndPage();}

  friend DsManipInt& dSetLMargin(int val);
  friend DsManipInt& dSetTab(    int val);
  friend DsManipInt& dSetRTab(   int val);
  friend DsManipStg& dFFace( TCchar* val);
  friend DsManipDbl& dFSize(  double val);
  friend DsManipInt& dEditBox(   int val);
  friend class DisplayDev;
  };


// No Argument Manipulator

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
extern DspManip dPrevFont;       // Restore previous font
extern DspManip dBoldFont;
extern DspManip dItalicFont;
extern DspManip dUnderLineFont;
extern DspManip dStrikeOutFont;

// One Argument Manipulator

// insert in stream dsp << dSetTab(n) << ... where n is ~no of characters from margin, etc.

DsManipInt& dSetLMargin(int val);
DsManipInt& dSetTab(    int val);
DsManipInt& dSetRTab(   int val);
DsManipStg& dFFace(TCchar*  val);
DsManipDbl& dFSize(double   val);
DsManipInt& dEditBox(   int val);


