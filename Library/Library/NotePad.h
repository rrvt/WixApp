// NotePad
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.

// Streaming output to the display
// The idea is that a NotePad data structure stores everything needed to display text on the
// display.  The View object is in charge of actually displaying the text.  The Doc object
// contains an enum that defines which NotePad object to just and the View object contains the
// functions called by MFC to implement displaying text in the window.  The View functions are:
//   * onDisplayOutput
//   * displayHeader
//   * displayFooter
// To use the NotePad here are the streaming items that store date for later delivery to the
// display by using the Doc object "display()" function.  The display function takes an argument
// that declares which NotePad object to display.
//
//   String s = _T(" Test 2");
//
//     notePad << _T("text") << s << nCrlf;   // Streaming means that the elements are output in
//                                            // the order they appear in the statement.  This
//                                            // example results in "text Test 2\n" where \n is a
//                                            // carriage return/line feed on the display.

// So here is the list of entities that may be streamed.  Please note that the "\n and \r may not
// appear in any string entity.
//   String  s;
//   CString cs;
//   bstr_t  bs;
//   Tchar   ch;
//   Cchar*  cc;
//   int     intV;
//   size_t  sizeV;
//   long    longV;
//   ulong   ulongV;
//   double  dblV;
//   Date    date;

//   notePad << s;                            // Display a String
//   notePad << cs;                           // Display a CString
//   notePad << bs;                           // Display a bstring
//   notePad << ch;                           // Display a Unicode character
//   notePad << cc;                           // Display a 8 bit c string terminated by zero
//   notePad << intV;                         // Display an integer
//   notePad << sizeV;                        // Display an size_t (a typed integer)
//   notePad << longV;                        // Display a long integer
//   notePad << ulongV;                       // Display a unsigned long integer
//   notePad << dblV;                         // Display a double floating point value
//   notePad << date;                         // Display a date (as defined by the Date class)

// The following entities modify the stream following as described:

//   notePad << nClrTabs;                     // Clear all Tabs
//   notePad << nTab;                         // Move the next text to the next tab stop
//   notePad << nCenter;                      // Center the next text on the line
//   notePad << nRight;                       // Move the next text so that the right side of the
//                                            // text is at the right edge of the display
//   notePad << nBeginLine;                   // Start an underline at the current position on the
//                                            // display
//   notePad << nEndLine;                     // Stop underlining at the current position on the
//                                            // display
//   notePad << nCrlf;                        // Execute a carriage return/line feed on the display
//   notePad << nEndPage;                     // During printing (yes a NotePad can be printed)
//                                            // end the page on the printer

// Font modifiers allow the font to manipulated on the screen.  Each modifier either pushes the
// current fong onto a stack or pops an earlier font from the stack.

//   notePad << nBold;                        // Start using the bold font from the current
//                                            // position on the display
//   notePad << nItalic;                      // Start using the Italic font from the current
//                                            // position on the display
//   notePad << nUnderLine;                   // Start using an Underline font from the current
//                                            // position on the display
//   notePad << nStrikeOut;                   // Start using a StrikeOut font (an line through the
//                                            // characters) from the current position on the
//                                            // display
//   notePad << nFont;                        // Each Font modifier pushes the current font onto a
//                                            // stack.  nFont pops one earlier font from the
//                                            // stack and start using that font from the current
//                                            // position on th display.

// The following modifiers take an argument that are used at the current position on the display
// The position values are multiples of an average width of a character.

//   notePad << nSetTab(<int>);               // Sets a tab such that a nTab move the current
//                                            // position to position indicated by the <int>
//   notePad << nSetRTab(<int>);              // Sets a tab such that a nTab moves the next text
//                                            // such that the right end of the text fragment is
//                                            // at the position indicated by the <int>

//   notePad << nSetLMargin(<double>>);       // Set the left & right margin on the screen in
//   notePad << nSetRMargin(<double>>);       // character widths

// Modifiers for integers and doubles
//   ar << aSetWidth(<int>);                  // Set the width of the next double, unsigned integer
//                                            // or integer.  Negative values => left adjust in
//                                            // width, Positive value => right adjust in width
//   ar << aSetPrec(<int>);                   // Set the maximum number of significant digits
//                                            // for a double output.  Sets the width of an integer
//                                            // output, padding with leading zeros
//   ar << aSetHex;                           // Output the next integer value (unsigned or signed)
//                                            // as a hex number: eg 0xab123.  Note specify
//                                            // precision value to add leading zeros

// Font Modifiers
//   notePad << nFSize(<double>);             // Set font size, pushing the current size onto the
//                                            // stack
//   notePad << nFFace(<TCchar*>);            // Set font face (e.g. _T("Arial")), pushing the
//                                            // current face onto the stack


#pragma once
//#include "Archive.h"
#include "Currency.h"
#include "Date.h"
#include "ManipT.h"
#include "Note.h"
#include "NoteUndrLn.h"

enum {DefFSize = 120};              // pt * 10 i.e. 12.2pt == 122
class NotePad;


typedef ManipT<NotePad>    NoteManip;
typedef ManipIntT<NotePad> NtManipInt;
typedef ManipDblT<NotePad> NtManipDbl;
typedef ManipStgT<NotePad> NtManipStg;


class NotePad {
NoteList   noteList;
Note*      note;
int        noLines;

int        arWidth;                   // Width of output to a text file
double     tabFactor;                 // n = tab * tabFactor where tab is an integer and n is the
                                      // number of characters to move in the output text file.
NoteUndrLn undrLn;
public:

  NotePad();
 ~NotePad() {clear();}

  void clear();                         // To clear notepad for new input.

  bool     isEmpty() {return noteList.isEmpty();}

  NotePad& operator <<(TCchar*        tc) {return append(tc);}
  NotePad& operator <<(const String&   s) {return append((TCchar*)(s));}
  NotePad& operator <<(const CString& cs) {return append(cs);}
  NotePad& operator <<(const bstr_t   bs) {return append((TCchar*)bs);}

  NotePad& operator <<(Tchar           v) {return append(v);}
  NotePad& operator <<(Cchar*         cs) {return append(cs);}

  NotePad& operator <<(int             v) {return doNmbr((long)  v);}
  NotePad& operator <<(size_t          v) {return doNmbr((ulong) v);}
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

  void     setArchiveAttr(int w, double f = 1.0) {arWidth   = w; tabFactor = f;}

  bool     append(Note* note) {noteList.append(note); return note->crlf;}

private:

  String&  underLineStg();

  void     initialize();                      // Must open Notepad before first use.

  NotePad& append(String const& s);
  NotePad& append(TCchar* tc);
  NotePad& append(Tchar   v);
  NotePad& append(Cchar*  cs);

  NotePad& doNmbr(long   v);
  NotePad& doNmbr(ulong  v);
  NotePad& doNmbr(double v);

  NotePad& append(Date    v);
//  bool     doNumAttr(int& prec);

  NotePad& crlf();
  NotePad& endPage();

  Note&    getNote(NoteOrdr attr) {return note && !note->isAfter(attr) ? *note : allocNote();}
  Note&    getNote()              {return note                         ? *note : allocNote();}
  Note&    allocNote() {NewAlloc(Note);  note = AllocNode;  noteList.append(note);   return *note;}


  static NotePad& doFFace(        NotePad& n, String& v)
                                              {n.getNote(FontFaceNO).fFace      = v;     return n;}
  static NotePad& doFSize(        NotePad& n, double  v)
                                              {n.getNote(FontSizeNO).fSize      = v;     return n;}
  static NotePad& doBoldFont(     NotePad& n) {n.getNote(BoldNO).bold           = true;  return n;}
  static NotePad& doItalicFont(   NotePad& n) {n.getNote(ItalicNO).italic       = true;  return n;}
  static NotePad& doUnderLineFont(NotePad& n)
                                          {n.getNote(UnderlineFontNO).underline = true;  return n;}
  static NotePad& doStrikeOutFont(NotePad& n) {n.getNote(StrikeOutNO).strikeOut = true;  return n;}
  static NotePad& doPrevFont(     NotePad& n) {n.getNote(PrevFontNO).prevFont   = true;  return n;}

  static NotePad& doClrTabs(      NotePad& n);


  static NotePad& doTab(          NotePad& n);
  static NotePad& doCenter(       NotePad& n);
  static NotePad& doRight(        NotePad& n);

  static NotePad& doBeginLine(    NotePad& n);
  static NotePad& doEndLine(      NotePad& n);
  static NotePad& doSetHex(       NotePad& n);

  static NotePad& doCrlf(         NotePad& n) {return n.crlf();}
  static NotePad& doEndPage(      NotePad& n) {return n.endPage();}
  static NotePad& doDebug(        NotePad& n) {n.getNote(DebugNO).debug = true; return n;}

  static NotePad& doSetTab(       NotePad& n, int    v);
  static NotePad& doSetRTab(      NotePad& n, int    v);
  static NotePad& doSetLMargin(   NotePad& n, double v);
  static NotePad& doSetRMargin(   NotePad& n, double v);

  static NotePad& doSetWidth(     NotePad& n, int    v);
  static NotePad& doSetPrec(      NotePad& n, int    v);

  friend NtManipDbl& nSetLMargin(double val);
  friend NtManipDbl& nSetRMargin(double val);
  friend NtManipStg& nFFace(    TCchar* face);
  friend NtManipDbl& nFSize(     double val);
  friend NtManipInt& nSetTab(    int    val);
  friend NtManipInt& nSetRTab(   int    val);
  friend NtManipInt& nSetWidth(  int    val);
  friend NtManipInt& nSetPrec(   int    prec);

  friend class NotePadLoop;
  friend class NtPdIter;

  friend class NtPdToArch;
  };


extern NotePad notePad;

NtManipDbl&      nSetLMargin(double val);   // Set Left Margin measured in char width
NtManipDbl&      nSetRMargin(double val);   // Set right Margin measured in char width

NtManipStg&      nFFace( TCchar* face);     // Set font face, e.g.
NtManipDbl&      nFSize(  double val);      // Set font size 12.0 = 12 pt font

extern NoteManip nBold;                     // set font to bold
extern NoteManip nItalic;                   // set font to italic
extern NoteManip nUnderLine;                // underline font
extern NoteManip nStrikeOut;                // strike out font
extern NoteManip nFont;                     // restore previous font

extern NoteManip nClrTabs;                  // add to stream to clear tabs:  dsp << nClrTabs;
NtManipInt&      nSetTab(    int val);      // Set tab at Tchar pos given by val (using avg Tchar
                                            // width)
NtManipInt&      nSetRTab(   int val);      // Set right tab at val

extern NoteManip nTab;                      // add to stream to tab to next tab position:
                                            //   dsp << dTab << "xyz";
extern NoteManip nCenter;                   // Set bit to center from here to next tab or crlf;
extern NoteManip nRight;                    // Set bit to right align from here to crlf;

// Specify immediately before an integer or double to be effective.  Only effective for one value
NtManipInt&      nSetWidth(  int val);      // Set width for next integer or double, negativ
                                            // indicates left adj
NtManipInt&      nSetPrec(   int prec);     // Set precision (no of digits)
extern NoteManip nSetHex;                   // Set hex modifier for next integer value (unsigned or
                                            // signed)

extern NoteManip nBeginLine;                // Begin and end line under text.
extern NoteManip nEndLine;

extern NoteManip nCrlf;                     // add to stream to terminate a line on display:
                                            // dsp << "xyz" << dCrlf;
extern NoteManip nEndPage;                  // add to stream to terminate page when printing
                                            // otherwise ignore
extern NoteManip nDebug;                    // Set to start a debug sequence in the View Class


NtManipInt& nEditBox(   int x);


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





///----------------


//  void     movPos(TextPosition& from, int to, Archive& ar);
//  int      applyTabFactor(int tb);

//class TextPosition;

