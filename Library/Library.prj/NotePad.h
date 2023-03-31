// NotePad
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.

#pragma once
#include "Archive.h"
#include "Currency.h"
#include "Date.h"
#include "ManipT.h"
#include "Note.h"

class TextPosition;

enum {DefFSize = 120};             // pt * 10 i.e. 12.2pt == 122
class NotePad;


typedef ManipT<NotePad>    NoteManip;
typedef ManipIntT<NotePad> NtManipInt;
typedef ManipDblT<NotePad> NtManipDbl;
typedef ManipStgT<NotePad> NtManipStg;


class NotePad {
NoteList noteList;
Note*    note;
int      noLines;

int      arWidth;                   // Width of output to a text file
double   tabFactor;                 // n = tab * tabFactor where tab is an integer and n is the number
                                    // of characters to move in the output text file.
public:

  NotePad();
 ~NotePad() {clear();}

  void clear();                         // To clear notepad for new input.

  bool append(Note* note) {noteList.append(note); return note->crlf;}

  void setArchiveAttr(int w, double f = 1.0) {arWidth   = w; tabFactor = f;}
  void archive(Archive& ar);

  NotePad& operator <<(const String&   s) {return append(s);}
  NotePad& operator <<(Tchar           v) {return append(v);}
  NotePad& operator <<(Cchar*         cs) {return append(cs);}

  NotePad& operator <<(int             v) {return doNmbr((long) v);}
  NotePad& operator <<(size_t          v) {return doNmbr((long) v);}
  NotePad& operator <<(long            v) {return doNmbr(v);}
  NotePad& operator <<(ulong           v) {return doNmbr(v);}
  NotePad& operator <<(double          v) {return doNmbr(v);}

  NotePad& operator <<(Date            v) {return append(v);}

  NotePad& operator <<(NoteManip&      m) {return m.func(*this);}
  NotePad& operator <<(NtManipInt&     m)
                                {NewAlloc(NtManipInt); m.func(*this, m.v); FreeNode(&m); return *this;}
  NotePad& operator <<(NtManipDbl&     m)
                                {NewAlloc(NtManipDbl); m.func(*this, m.v); FreeNode(&m); return *this;}
  NotePad& operator <<(NtManipStg&     m)
                                {NewAlloc(NtManipStg); m.func(*this, m.v); FreeNode(&m); return *this;}
private:

  void archive(NoteNmbr& nn, TextPosition& tPos, Archive& ar);

  void initialize();                          // Must open Notepad before first use.

  NotePad& append(const String& line);
  NotePad& append(Cchar*        cs);
  NotePad& append(Tchar         v);

  NotePad& doNmbr(long   v);
  NotePad& doNmbr(ulong  v);
  NotePad& doNmbr(double v);

  NotePad& append(Date    v);
  bool     doNumAttr(int& prec);

  NotePad& crlf();
  NotePad& endPage();

  Note&    getNote(NoteAttr attr) {return note && !note->isAfter(attr) ? *note : allocNote();}
  Note&    getNote()              {return note                         ? *note : allocNote();}
  Note&    allocNote() {NewAlloc(Note);   note = AllocNode;   noteList.append(note);   return *note;}

  void     movPos(TextPosition& from, int to, Archive& ar);
  int      applyTabFactor(int tb);

  static NotePad& doClrTabs(      NotePad& n);
  static NotePad& doTab(          NotePad& n);
  static NotePad& doCenter(       NotePad& n);
  static NotePad& doRight(        NotePad& n);

  static NotePad& doBeginLine(    NotePad& n);
  static NotePad& doEndLine(      NotePad& n);

  static NotePad& doCrlf(         NotePad& n) {return n.crlf();}
  static NotePad& doEndPage(      NotePad& n) {return n.endPage();}
  static NotePad& doDebug(        NotePad& n) {n.getNote(DbgNAttr).debug = true; return n;}

  static NotePad& doFFace(        NotePad& n, String& v) {n.getNote(PFontNAttr).fFace = v; return n;}
  static NotePad& doFSize(        NotePad& n, double  v) {n.getNote(PFontNAttr).fSize = v; return n;}
  static NotePad& doBoldFont(     NotePad& n) {n.getNote(PFontNAttr).bold      = true; return n;}
  static NotePad& doItalicFont(   NotePad& n) {n.getNote(PFontNAttr).italic    = true; return n;}
  static NotePad& doUnderLineFont(NotePad& n) {n.getNote(PFontNAttr).underline = true; return n;}
  static NotePad& doStrikeOutFont(NotePad& n) {n.getNote(PFontNAttr).strikeOut = true; return n;}
  static NotePad& doPrevFont(     NotePad& n) {n.getNote(PFontNAttr).prevFont  = true; return n;}

  static NotePad& doSetTab(       NotePad& n, int v);
  static NotePad& doSetRTab(      NotePad& n, int v);
  static NotePad& doSetLMargin(   NotePad& n, int v);

  static NotePad& doSetWidth(     NotePad& n, int v);
  static NotePad& doSetPrec(      NotePad& n, int v);

  friend NtManipInt& nSetTab(    int val);
  friend NtManipInt& nSetRTab(   int val);
  friend NtManipInt& nSetLMargin(int val);
  friend NtManipInt& nSetWidth(  int val);
  friend NtManipInt& nSetPrec(   int prec);
  friend NtManipDbl& nFSize(  double val);
  friend NtManipStg& nFFace( TCchar* face);

  friend class NotePadLoop;
  friend class NtPdIter;
  };


extern NotePad notePad;

// no Argument Manipulator

extern NoteManip nClrTabs;    // add to stream to clear tabs:                  dsp << dClrTabs;
extern NoteManip nTab;        // add to stream to tab to next tab position:    dsp << dTab << "xyz";
extern NoteManip nCenter;     // Set bit to center from here to next tab or crlf;
extern NoteManip nRight;      // Set bit to right align from here to crlf;

extern NoteManip nBeginLine;
extern NoteManip nEndLine;    // Begin and end line under text.

extern NoteManip nCrlf;       // add to stream to terminate a line on display: dsp << "xyz" << dCrlf;
extern NoteManip nEndPage;    // add to stream to terminate page when printing otherwise ignore
                              //
extern NoteManip nBold;       // set font to bold
extern NoteManip nItalic;     // set font to italic
extern NoteManip nUnderLine;  // underline font
extern NoteManip nStrikeOut;  // strike out font
extern NoteManip nFont;       // restore previous font

extern NoteManip nDebug;      // Set to start a debug sequence in the View Class

// One Argument Manipulator
// insert in stream notePad << dSetTab(n) << ... where n is ~no of characters from margin, etc.

NtManipInt& nSetLMargin(int val);     // Set Left Margin at Tchar pos given by val (using avg Tchar width)
NtManipInt& nSetTab(    int val);     // Set tab at Tchar pos given by val (using avg Tchar width)
NtManipInt& nSetRTab(   int val);     // Set right tab at val

// Specify immediately before an integer or double to be effective.  Only effective for one value
NtManipInt& nSetWidth(  int val);     // Set width for next integer or double, negativ indicates left adj
NtManipInt& nSetPrec(   int prec);    // Set precision (no of digits)

NtManipInt& nEditBox(   int x);
NtManipStg& nFFace( TCchar* face);    // Set font face, e.g.
NtManipDbl& nFSize(  double val);     // Set font size 12.0 = 12 pt font


class NtPdIter : public ListLoop {
public:

  NtPdIter(NotePad& np) : ListLoop(np.noteList) {}
 ~NtPdIter() {}

  // initialize for scan of list and return first node on the list or zero if at end of list.

  virtual Note* operator() () {return (Note*) ListLoop::startLoop();}

  // move to next node on list and return pointer to that node or zero if at end of list

  virtual Note* operator++ (int) {return (Note*) ListLoop::nextNode();};

private:

  NtPdIter() : ListLoop(*(List*)0) { }
  };



