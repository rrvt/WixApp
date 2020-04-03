// NotePad
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.

#pragma once
#include "Archive.h"
#include "Currency.h"
#include "Date.h"
#include "Note.h"

//#include "MessageBox.h"            // Debugging Only

enum {DefFSize = 120};

extern String dbgLine;

class NotePad;



struct NoteManip {

typedef NotePad& (*Func)(NotePad&);

NotePad* n;
Func     func;

  NoteManip()                            : n(0),       func(0)    {}
  NoteManip(NotePad* notePad, Func func) : n(notePad), func(func) {}
  };


struct NoteManip1 {

typedef NotePad& (*Func)(NotePad&, int);

Func func;
int  v;

  NoteManip1()                : func(0), v(0)   {}
  NoteManip1(Func f, int val) : func(f), v(val) {}
  };





class NotePad {
NoteList noteList;
Note*    note;
int      noLines;
int      maxLines;

public:

  NotePad();
 ~NotePad() {}

  void open();                          // Must open Notepad before first use.
  void close();                         // To clear notepad for new input.

  int  getNoLines() {return noLines;}
  void setMaxLines(int max) {maxLines = max;}
  void archive(Archive& ar);

private:

  NotePad& append(const String& line) {getNote().line += line.str();   return *this;}
  NotePad& append(TCchar* line)       {getNote().line += line;         return *this;}
  NotePad& append(Tchar       v)      {getNote().line += v;            return *this;}
  NotePad& append(int         v);
  NotePad& append(ulong       v);
  NotePad& append(Date        v);
  NotePad& append(CTimeSpan&  v);
  NotePad& append(double      v);

  NotePad& crlf();
  NotePad& endPage();
  NotePad& newLine(int n = 1) {for (int i = 0; i < n; i++) {append(_T(" ")); crlf();} return *this;}

  Note& getNote();

  friend NotePad& operator << (NotePad& n, ostringstream& os);
  friend NotePad& operator << (NotePad& n, const String& stg);
  friend NotePad& operator << (NotePad& n, int             v);
  friend NotePad& operator << (NotePad& n, ulong           v);
  friend NotePad& operator << (NotePad& n, variant_t&      v);
  friend NotePad& operator << (NotePad& n, Tchar           v);
  friend NotePad& operator << (NotePad& n, double          v);
  friend NotePad& operator << (NotePad& n, Date            v);
  friend NotePad& operator << (NotePad& n, CTimeSpan&      v);
  friend NotePad& doClrTabs(   NotePad& n);
  friend NotePad& doSetLMargin(NotePad& n, int v);
  friend NotePad& doSetTab(    NotePad& n, int v);
  friend NotePad& doSetRTab(   NotePad& n, int v);
  friend NotePad& doSetWidth(  NotePad& n, int val);
  friend NotePad& doSetPrec(   NotePad& n, int prec);
  friend NotePad& doFSize(     NotePad& n, int v);
  friend NotePad& doEditBox(   NotePad& n, int v);
  friend NotePad& doTab(       NotePad& n);
  friend NotePad& doCrlf(      NotePad& n);
  friend NotePad& doEndPage(   NotePad& n);
  friend NotePad& doCenter(    NotePad& n);
  friend NotePad& doRight(     NotePad& n);
  friend NotePad& doBeginLine( NotePad& n);
  friend NotePad& doEndLine(   NotePad& n);
  friend NotePad& doDebug(     NotePad& n);
  friend NotePad& doPrevFont(  NotePad& n);
  friend NotePad& doBoldFont(  NotePad& n);
  friend NotePad& doItalicFont(NotePad& n);
  friend NotePad& doUnderLineFont(NotePad& n);
  friend NotePad& doStrikeOutFont(NotePad& n);
  friend void     noFontReq(NotePad& n);

  friend NotePad& setTableName(NotePad& n);
  friend class NotePadLoop;
  };


extern NotePad notePad;


  inline NotePad& operator <<(NotePad& n, const String&   s) {return n.append(s);}
  inline NotePad& operator <<(NotePad& n, int             v) {return n.append(v);}
  inline NotePad& operator <<(NotePad& n, ulong           v) {return n.append(v);}

  inline NotePad& operator <<(NotePad& n, Tchar           v) {return n.append(v);}
  inline NotePad& operator <<(NotePad& n, double          v) {return n.append(v);}
  inline NotePad& operator <<(NotePad& n, Date            v) {return n.append(v);}
  inline NotePad& operator <<(NotePad& n, CTimeSpan&      v) {return n.append(v);}
// no Argument Manipulator

extern NoteManip nClrTabs;    // add to stream to clear tabs:                  dsp << dClrTabs;
extern NoteManip nCrlf;       // add to stream to terminate a line on display: dsp << "xyz" << dCrlf;
extern NoteManip nEndPage;    // add to stream to terminate page when printing otherwise ignore
extern NoteManip nTab;        // add to stream to tab to next tab position:    dsp << dTab << "xyz";
extern NoteManip nFont;       // restore previous font
extern NoteManip nBold;       // set font to bold
extern NoteManip nItalic;     // set font to italic
extern NoteManip nUnderLine;  // underline font
extern NoteManip nStrikeOut;  // strike out font
extern NoteManip nTableName;  // Set bit to identify table name note:          dsp << dTableName << "xyz";
extern NoteManip nCenter;     // Set bit to center from here to next tab or crlf;
extern NoteManip nRight;      // Set bit to right align from here to crlf;
extern NoteManip nBeginLine;
extern NoteManip nEndLine;    // Begin and end line under text.
extern NoteManip nDebug;      // Set to start a debug sequence in the View Class

inline NotePad& operator <<(NotePad& n, NoteManip& manip)  {return manip.func(n);}

// One Argument Manipulator

inline NotePad& operator <<(NotePad& n, NoteManip1& manip)
                                           {NoteManip1* m = &manip; m->func(n, m->v); delete m; return n;}

// insert in stream dsp << dSetTab(n) << ... where n is ~no of characters from margin

NoteManip1& nSetLMargin(int val);     // Set Left Margin at Tchar pos given by val (using avg Tchar width)
NoteManip1& nSetTab(    int val);     // Set tab at Tchar pos given by val (using avg Tchar width)
NoteManip1& nSetRTab(   int val);     // Set right tab at val
NoteManip1& nSetWidth(  int val);     // Set width for next integer or double
NoteManip1& nSetPrec(   int prec);    // Set precision (no of digits after period) of next double
NoteManip1& nFSize(     int val);     // Set font size (*10) e.g. 120 = 12 pt font
NoteManip1& nEditBox(   int x);




class NotePadLoop : public ListLoop {
public:

  NotePadLoop() : ListLoop(notePad.noteList) {}
 ~NotePadLoop() {}

  // initialize for scan of list and return first node on the list or zero if at end
  // of list.
  virtual Note* start() {return (Note*) ListLoop::startLoop();}

  // move to next node on list and return pointer to that node or zero if at end of list
  virtual Note* nextNode() {return (Note*) ListLoop::nextNode();};
  };

