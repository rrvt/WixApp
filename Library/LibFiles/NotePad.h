// NotePad
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.

#pragma once
#include "Archive.h"
#include "Currency.h"
#include "Date.h"
#include "ManipT.h"
#include "Note.h"
#include "Wrap.h"


enum {DefFSize = 120};             // pt * 10 i.e. 12.2pt == 122
class NotePad;


typedef ManipT<NotePad>    NoteManip;
typedef ManipValT<NotePad> NoteManip1;


class NotePad {
NoteList noteList;
Note*    note;
int      noLines;

public:

  NotePad();
 ~NotePad() {clear();}

  void clear();                         // To clear notepad for new input.

  int  getNoLines() {return noLines;}
  void archive(Archive& ar);

  NotePad& operator <<(const String&   s) {return append(s);}
  NotePad& operator <<(Wrap&           w) {return append(w);}
  NotePad& operator <<(int             v) {return append(v);}
  NotePad& operator <<(ulong           v) {return append(v);}
  NotePad& operator <<(Tchar           v) {return append(v);}
  NotePad& operator <<(double          v) {return append(v);}
  NotePad& operator <<(Date            v) {return append(v);}
  NotePad& operator <<(CTimeSpan&      v) {return append(v);}
  NotePad& operator <<(NoteManip&      m) {return m.func(*this);}
  NotePad& operator <<(NoteManip1&     m)
                                  {NewAlloc(NoteManip1); m.func(*this, m.v); FreeNode(&m); return *this;}

private:

  void initialize();                          // Must open Notepad before first use.

  NotePad& append(const String& line) {getNote().line += line.str();   return *this;}
  NotePad& append(Wrap&         w);   //    {getNote().wrap = w; return *this;}
  NotePad& append(TCchar* line)       {getNote().line += line;         return *this;}
  NotePad& append(Tchar       v)      {getNote().line += v;            return *this;}
  NotePad& append(int         v);
  NotePad& append(ulong       v);
  NotePad& append(Date        v) {String s = v; return append(s);}
  NotePad& append(CTimeSpan&  v) {String s;     return append(s);}
  NotePad& append(double      v);

  NotePad& crlf();
  NotePad& endPage();

  Note& getNote() {if (!note) {NewAlloc(Note); note = AllocNode; noteList.append(note);}   return *note;}

  static NotePad& doClrTabs(      NotePad& n);
  static NotePad& doTab(          NotePad& n);
  static NotePad& doCrlf(         NotePad& n) {return n.crlf();}
  static NotePad& doEndPage(      NotePad& n)
  {return n.endPage();}
  static NotePad& setTableName(   NotePad& n) {n.getNote().isTable   = true; return n;}
  static NotePad& doCenter(       NotePad& n);
  static NotePad& doRight(        NotePad& n);
  static NotePad& doBeginLine(    NotePad& n);
  static NotePad& doEndLine(      NotePad& n) {              n.getNote().endLine   = true; return n;}
  static NotePad& doDebug(        NotePad& n) {              n.getNote().debug     = true; return n;}
  static NotePad& doPrevFont(     NotePad& n) {noFontReq(n); n.getNote().prevFont  = true; return n;}
  static NotePad& doBoldFont(     NotePad& n) {noFontReq(n); n.getNote().bold      = true; return n;}
  static NotePad& doItalicFont(   NotePad& n) {noFontReq(n); n.getNote().italic    = true; return n;}
  static NotePad& doUnderLineFont(NotePad& n) {noFontReq(n); n.getNote().underline = true; return n;}
  static NotePad& doStrikeOutFont(NotePad& n) {noFontReq(n); n.getNote().strikeOut = true; return n;}
  static void     noFontReq(      NotePad& n);

  static NotePad& doSetTab(       NotePad& n, int v);
  static NotePad& doSetRTab(      NotePad& n, int v);
  static NotePad& doSetLMargin(   NotePad& n, int v);
  static NotePad& doSetWidth(     NotePad& n, int v) {n.getNote().width     = v; return n;}
  static NotePad& doSetPrec(      NotePad& n, int v) {n.getNote().precision = v; return n;}
  static NotePad& doEditBox(      NotePad& n, int v);
  static NotePad& doFSize(        NotePad& n, int v) {noFontReq(n);  n.getNote().fSize = v; return n;}

  friend NoteManip1& nSetTab(    int val);
  friend NoteManip1& nSetRTab(   int val);
  friend NoteManip1& nSetLMargin(int val);
  friend NoteManip1& nSetWidth(  int val);
  friend NoteManip1& nSetPrec(   int prec);
  friend NoteManip1& nEditBox(   int val);
  friend NoteManip1& nFSize(     int val);

//  friend NotePad& operator << (NotePad& n, ostringstream& os);
//  friend NotePad& operator << (NotePad& n, variant_t&      v);
  friend class NotePadLoop;
  };


extern NotePad notePad;

// no Argument Manipulator

extern NoteManip nClrTabs;    // add to stream to clear tabs:                  dsp << dClrTabs;
extern NoteManip nCrlf;       // add to stream to terminate a line on display: dsp << "xyz" << dCrlf;
extern NoteManip nEndPage;    // add to stream to terminate page when printing otherwise ignore
extern NoteManip nTab;        // add to stream to tab to next tab position:    dsp << dTab << "xyz";
extern NoteManip nTableName;  // Set bit to identify table name note:          dsp << dTableName << "xyz";
extern NoteManip nCenter;     // Set bit to center from here to next tab or crlf;
extern NoteManip nRight;      // Set bit to right align from here to crlf;
extern NoteManip nBeginLine;
extern NoteManip nEndLine;    // Begin and end line under text.
extern NoteManip nDebug;      // Set to start a debug sequence in the View Class
extern NoteManip nFont;       // restore previous font
extern NoteManip nBold;       // set font to bold
extern NoteManip nItalic;     // set font to italic
extern NoteManip nUnderLine;  // underline font
extern NoteManip nStrikeOut;  // strike out font

// One Argument Manipulator
// insert in stream notePad << dSetTab(n) << ... where n is ~no of characters from margin, etc.

NoteManip1& nSetTab(    int val);     // Set tab at Tchar pos given by val (using avg Tchar width)
NoteManip1& nSetRTab(   int val);     // Set right tab at val
NoteManip1& nSetLMargin(int val);     // Set Left Margin at Tchar pos given by val (using avg Tchar width)
NoteManip1& nSetWidth(  int val);     // Set width for next integer or double
NoteManip1& nSetPrec(   int prec);    // Set precision (no of digits after period) of next double
NoteManip1& nEditBox(   int x);
NoteManip1& nFSize(     int val);     // Set font size (*10) e.g. 120 = 12 pt font


class NotePadLoop : public ListLoop {
public:

  NotePadLoop() : ListLoop(notePad.noteList) {}
 ~NotePadLoop() {}

  // initialize for scan of list and return first node on the list or zero if at end of list.

  virtual Note* start() {return (Note*) ListLoop::startLoop();}

  // move to next node on list and return pointer to that node or zero if at end of list

  virtual Note* nextNode() {return (Note*) ListLoop::nextNode();};
  };




//  NotePad& newLine(int n = 1) {for (int i = 0; i < n; i++) {append(_T(" ")); crlf();} return *this;}

