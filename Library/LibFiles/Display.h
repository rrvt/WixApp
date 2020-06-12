// Display to Window
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.

#pragma once

#include "FontMgr.h"
#include "ManipT.h"
#include "TextPosition.h"


class Display;


typedef ManipT<   Display> DspManip;
typedef ManipValT<Display> DspManip1;


class Display {

String       sum;
FontMgr      font;
int          noPoints;
POINT        points[20];
CDC*         dc;
int          curHeight;                     // Text height
int          maxHeight;                     // Max text height to deal with font changes
int          toLine;                        // Distance to Line
const int    initialYPos;                   // Initial vertical position in pixels from the top of window
int          y;                             // Vertical position in pixels from top of window
int          maxY;
int          topEdge;                       // pixels from top edge of screen/printer
int          bottomEdge;                    // pixels of bottom edge from printable top edge
bool         nonBlankLine;                  // true when first non blank line is encountered during
                                            // printing
TextPosition tPos;

bool         center;                        // center string in window when true;
bool         right;                         // right align in window when true;
Tab          rightTab;                      // right align to tab value when true;
bool         beginPage;
bool         endPageFlag;                   // Signal the end of a page

public:

bool         output;                        // Just count pages when false
bool         printing;                      // Printing when true, else displaying
uint         noPages;

  Display();
  Display(int initY);
 ~Display() {}

 Display& operator<< (String&    s) {return stg(s);}
 Display& operator<< (TCchar*    s) {return stg(s);}
 Display& operator<< (DspManip&  m) {return m.func(*this);}
 Display& operator<< (DspManip1& m) {m.func(*this, m.v); NewAlloc(DspManip1); FreeNode(&m); return *this;}

 void     prepareDisplay( TCchar* face, int fontSize, CDC* pDC, bool doOutput);
 void     preparePrinting(TCchar* face, int fontSize, CDC* pDC, bool doOutput);
 void     beginPrinting(CPrintInfo* pInfo);

 void     clrFont()                       {textOut(); font.uninstall();}
 void     resetMaxY()                     {maxY = 0;}
 void     getMaxPos(long& maxX, long& maxY) {maxX = tPos.maxCursorPos; maxY = this->maxY;}
 bool     withinBounds()                  {return topEdge < y && y < bottomEdge;}
 bool     isEndPage()                     {return printing && endPageFlag && y > topEdge;}
 int      getWidth()                      {return tPos.width;}
 int      getHeight()                     {return curHeight;}

private:

  static Display& doClrTabs(       Display& d) {d.tPos.clrTabs(); return d;}
  static Display& doCrlf(          Display& d) {d.textOut(); if (d.nonBlankLine) {d.crlf();}  return d;}
  static Display& doEndPage(       Display& d);
  static Display& doTab(           Display& d) {d.textOut();  d.rightTab = d.tPos.tab();  return d;}
  static Display& doCenter(        Display& d) {d.textOut();  d.center = true;  return d;}
  static Display& doRight(         Display& d) {d.textOut();  d.right = true;  return d;}
  static Display& doBeginLine(     Display& d);
  static Display& doEndLine(       Display& d);
  static Display& doPrevFont(      Display& d) {d.prevFont();      return d;}
  static Display& doBoldFont(      Display& d) {d.boldFont();      return d;}
  static Display& doItalicFont(    Display& d) {d.italicFont();    return d;}
  static Display& doUnderLineFont( Display& d) {d.underLineFont(); return d;}
  static Display& doStrikeOutFont( Display& d) {d.strikeOutFont(); return d;}
  static Display& doFlushFtr(      Display& d);

  static Display& doSetTab(        Display& d, int v) {d.tPos.setTab(v);  return d;}
  static Display& doSetRTab(       Display& d, int v) {d.tPos.setRTab(v); return d;}
  static Display& doSetLMargin(    Display& d, int v) {d.tPos.setLeftMargin(v); return d;}
  static Display& doEditBox(       Display& d, int v);
  static Display& doFSize(         Display& d, int v) {d.textOut();  d.setFontSize(v);  return d;}


  void     initialize();
  Display& stg(TCchar* s) {if (*s) nonBlankLine = true;   sum += s; return *this;}
  Display& stg(String& s) {return this->stg(s.str());}

  int      getMaxVertPos() {return y;}

  void     setMaxY(int t) {if (t > maxY) maxY = t;}
  void     setLMargin(int lm) {tPos.setLeftMargin(lm);}

  void     crlf();
  bool     setEndPage() {endPageFlag = printing; return endPageFlag;}

  void     initializeFont(TCchar* face, int fontSize);
  void     setFontSize(int fontSize);
  void     setMetric();
  void     boldFont()      {textOut(); font.setBold();}
  void     prevFont()      {textOut(); font.prevFont(); setMetric();}
  void     italicFont()    {textOut(); font.setItalic();}
  void     underLineFont() {textOut(); font.setUnderLine();}
  void     strikeOutFont() {textOut(); font.setStrikeOut();}

  void     textOut();
  void     fragmentOut(String& s);
  bool     exceedsMargin(String& s);
  int      getWidth(String& s, CString& cs);          // returns CString and width in current units
  int      findLastBlank(String& s);
  int      findLastComma(String& s);
  int      findLast(Tchar ch, String& s);
  int      findLastChar(String& s);
  void     outError(TCchar* stg);

  friend DspManip1& dSetLMargin(int val);
  friend DspManip1& dSetTab(    int val);
  friend DspManip1& dSetRTab(   int val);
  friend DspManip1& dFSize(     int val);
  friend DspManip1& dEditBox(   int val);
  };


// No Argument Manipulator

extern DspManip dClrTabs;        // add to stream to clear tabs:                   dsp << dClrTabs;
extern DspManip dCrlf;           // add to stream to terminate a line on display:  dsp << "xyz" << dCrlf;
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


