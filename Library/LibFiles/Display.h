// Display to Window
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.

#pragma once

#include "FontMgr.h"
#include "Horiz.h"
#include "ManipT.h"
#include "TextPosition.h"
#include "VertMgmt.h"
#include "Wrap.h"


class Display;


typedef ManipT<   Display> DspManip;
typedef ManipValT<Display> DspManip1;


class Display {
CDC*         dc;
Horiz        hz;
VertMgmt     vert;
FontMgr      font;

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

  Display();
 ~Display() {}

  void     clear();

  void     prepareDisplay( TCchar* face, int fontSize, CDC* pDC);
  void     preparePrinting(TCchar* face, int fontSize, CDC* pDC, CPrintInfo* pInfo);
  void     suppressOutput() {suppress = true;}
  void     negateSuppress() {suppress = false;}
  void     disableWrap()    {wrapEnabled = false;}
  void     enableWrap()     {wrapEnabled = true;}

  void     setFooter() {footer = true; sum.clear(); vert.setBottom();}
  void     clrFooter() {footer = false;}
  void     setHorzMgns(double left, double right) {leftMargin = left; rightMargin = right;}
  void     setVertMgns(double top,  double bot)   {topMargin  = top;   botMargin  = bot;}

  Display& operator<< (String&    s) {return stg(s);}
  Display& operator<< (TCchar*    s) {return stg(s);}
  Display& operator<< (Wrap&      w) {return append(w);}
  Display& operator<< (int        v) {return append(v);}
  Display& operator<< (ulong      v) {return append(v);}
  Display& operator<< (DspManip&  m) {return m.func(*this);}
  Display& operator<< (DspManip1& m) {m.func(*this, m.v); NewAlloc(DspManip1); FreeNode(&m); return *this;}

  CDC*     getDC()     {return dc;}
  int      remaining() {return hz.remaining();}
  int      chWidth()   {return hz.chWidth();}
  int      chHeight()  {return vert.heightCh();}

  FontAttr& getCurFont() {return font.getAttr();}
  LOGFONT&  getLogFont() {return font.getLogFont();}

private:
         void     initializeFrame(CPrintInfo* pInfo);
         bool     isPortrait(CPrintInfo* pInfo);

         void     clrFont()                         {font.pop();}
         void     getMaxPos(long& maxX, long& maxY) {maxY = hz.maxX(); maxY = vert.maxY();}
         bool     withinBounds()                    {return vert.withinBounds();}
         bool     doEndPageChk();
         void     atEndPageCond();
         int      getRemainder()                    {return hz.remaining();}
         int      vertPos()                         {return vert.pos();}
         void     clrLines()                        {vert.clrLines();}
         int      maxLines()                        {return vert.getMaxLines();}

  static Display& doClrTabs(       Display& d) {d.hz.clrTabs(); return d;}
  static Display& doCrlf(          Display& d)
                                            {if (d.textOut() && d.nonBlankLine) {d.crlf();}  return d;}
  static Display& doCR(            Display& d) {if (d.textOut()) d.hz.cr(); return d;}

  static Display& doEndPage(       Display& d);
  static Display& doTab(           Display& d) {if (d.textOut()) d.hz.tab(d.rightTab); return d;}

  static Display& doCenter(        Display& d) {if (d.textOut())  d.center = true;  return d;}
  static Display& doRight(         Display& d) {if (d.textOut())  d.right  = true;  return d;}
  static Display& doBeginLine(     Display& d);
  static Display& doEndLine(       Display& d);

  static Display& doBoldFont(      Display& d) {d.boldFont();      return d;}
  static Display& doItalicFont(    Display& d) {d.italicFont();    return d;}
  static Display& doUnderLineFont( Display& d) {d.underLineFont(); return d;}
  static Display& doStrikeOutFont( Display& d) {d.strikeOutFont(); return d;}
  static Display& doPrevFont(      Display& d) {d.prevFont();      return d;}

  static Display& doFlushFtr(      Display& d);

  static Display& doSetTab(        Display& d, int v) {d.hz.setTab(v);  return d;}
  static Display& doSetRTab(       Display& d, int v) {d.hz.setRTab(v); return d;}
  static Display& doSetLMargin(    Display& d, int v) {d.hz.setLeftMargin(v); return d;}
  static Display& doEditBox(       Display& d, int v);
  static Display& doFSize(         Display& d, int v) {d.setFontSize(v);  return d;}


  Display& stg(TCchar*  s) {if (*s) nonBlankLine = true;   sum += s; return *this;}
  Display& stg(String&  s) {return this->stg(s.str());}
  Display& append(Wrap& w);
  Display& append(int   v);
  Display& append(ulong v);

  void     crlf();

  void     initializeFont(TCchar* face, int fontSize);
  void     setFontSize(int v);
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

  friend DspManip1& dSetLMargin(int val);
  friend DspManip1& dSetTab(    int val);
  friend DspManip1& dSetRTab(   int val);
  friend DspManip1& dFSize(     int val);
  friend DspManip1& dEditBox(   int val);
  friend class DisplayDev;
  };


// No Argument Manipulator

extern DspManip dClrTabs;        // add to stream to clear tabs:                   dsp << dClrTabs;
extern DspManip dCrlf;           // add to stream to terminate a line on display:  dsp << "xyz" << dCrlf;
extern DspManip dCR;             // add to stream to perform a carriage return (only)
extern DspManip dEndPage;        // add to stream to terminate a page when printing or do nothing
extern DspManip dflushFtr;       // add to stream to terminate a footer when printing
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

DspManip1& dSetLMargin(int val);
DspManip1& dSetTab(    int val);
DspManip1& dSetRTab(   int val);
DspManip1& dFSize(     int val);
DspManip1& dEditBox(   int val);


