// Display to Window
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.

#pragma once

#include "TextPosition.h"
#include "FontMgr.h"

//#include "Debug.h"

enum {MaxTabs = 20, Margin = 3};


class Display;


struct Manip {

typedef Display& (*Func)(Display&);

Display* d;
Func     func;

  Manip()                        : d(0),   func(0)    {}
  Manip(Display* dsp, Func func) : d(dsp), func(func) {}
  };


struct Manip1 {

typedef Display& (*Func)(Display&, int);

Func     func;
int      v;

  Manip1()                : func(0), v(0)   {}
  Manip1(Func f, int val) : func(f), v(val) {}
  };



class Display {

String       sum;
FontMgr      font;
int          noPoints;
POINT        points[20];
CDC*         dc;
int          curHeight;                        // Text height
int          maxHeight;                     // Max text height to deal with font changes
int          toLine;                        // Distance to Line
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
UINT         noPages;

  Display();
 ~Display() {}

  int  getMaxVertPos() {return y;}

  void prepareDisplay( TCchar* face, int fontSize, CDC* pDC, CPrintInfo* pInfo, bool doOutput);
  void beginPrinting(  CDC* pDC, CPrintInfo* pInfo, bool doOutput);
  void preparePrinting(TCchar* face, int fontSize);
  void resetMaxY()    {maxY = 0;}
  void setMaxY(int t) {if (t > maxY) maxY = t;}
  void setLMargin(int lm) {tPos.setLeftMargin(lm);}
  void getMaxPos(int& maxX, int& maxY) {maxX = tPos.maxCursorPos; maxY = this->maxY;}

  int  getHeight()  {return curHeight;}
  void crlf();
  int  getWidth()   {return tPos.width;}
  bool isEndPage()  {
    if (endPageFlag) {
      int x = topEdge;
    }
    return printing && endPageFlag && y > topEdge;
    }
  bool setEndPage() {endPageFlag = printing; return endPageFlag;}
  bool withinBounds() {return topEdge < y && y < bottomEdge;}

  void initializeFont(TCchar* face, int fontSize);
  void setFontSize(int fontSize);
  void setMetric();
  void setFontBold();
  void setFontItalicize();
  void setFontUnderline();
  void setFontStrikeOut();
  void prevFont();
  void boldFont();
  void italicFont();
  void underLineFont();
  void strikeOutFont();
  void clrFont();


  Display& stg(TCchar* stg);
  Display& stg(String& s) {return this->stg(s.str());}
  void     textOut();
  void     fragmentOut(String& s);
  bool     exceedsMargin(String& s);
  int      getWidth(String& s, CString& cs);          // returns CString and width in current units
  int      findLastBlank(String& s);
  int      findLastComma(String& s);
  int      findLast(Tchar ch, String& s);
  int      findLastChar(String& s);
  void     outError(TCchar* stg);

  friend Display& doClrTabs(      Display& d);
  friend Display& doSetLMargin(   Display& d, int v);
  friend Display& doSetTab(       Display& d, int v);
  friend Display& doSetRTab(      Display& d, int v);
  friend Display& doFSize(        Display& d, int v);
  friend Display& doEditBox(      Display& d, int v);
  friend Display& doTab(          Display& d);
  friend Display& doCrlf(         Display& d);
  friend Display& doEndPage(      Display& d);
  friend Display& doFlushFtr(     Display& d);
  friend Display& doCenter(       Display& d);
  friend Display& doRight(        Display& d);
  friend Display& doBeginLine(    Display& d);
  friend Display& doEndLine(      Display& d);
  friend Display& doPrevFont(     Display& d);
  friend Display& doBoldFont(     Display& d);
  friend Display& doItalicFont(   Display& d);
  friend Display& doUnderLineFont(Display& d);
  friend Display& doStrikeOutFont(Display& d);
  };


inline Display& operator <<(Display& d, String& s) {return d.stg(s);}
inline Display& operator <<(Display& d, TCchar* s) {return d.stg(s);}

// no Argument Manipulator

extern Manip dClrTabs;        // add to stream to clear tabs:                   dsp << dClrTabs;
extern Manip dCrlf;           // add to stream to terminate a line on display:  dsp << "xyz" << dCrlf;
extern Manip dEndPage;        // add to stream to terminate a page when printing or do nothing
extern Manip dflushFtr;       // add to stream to terminate a footer when printing
extern Manip dTab;            // add to stream to tab to next tab position:     dsp << dTab << "xyz";
extern Manip dCenter;         // center the string following up to the nCrlf
extern Manip dRight;          // right align the string following up to the nCrlf
extern Manip dBeginLine;      // begin line here
extern Manip dEndLine;        // end line here -- Does not affect current position
extern Manip dPrevFont;       // Restore previous font
extern Manip dBoldFont;
extern Manip dItalicFont;
extern Manip dUnderLineFont;
extern Manip dStrikeOutFont;


inline Display& operator <<(Display& d, Manip& manip)  {return manip.func(d);}

// One Argument Manipulator

inline Display& operator <<(Display& d, Manip1& manip)
                                               {Manip1* m = &manip; m->func(d, m->v); delete m; return d;}

// insert in stream dsp << dSetTab(n) << ... where n is ~no of characters from margin

Manip1& dSetLMargin(int val);
Manip1& dSetTab(    int val);
Manip1& dSetRTab(   int val);
Manip1& dFSize(     int val);
Manip1& dEditBox(   int val);
