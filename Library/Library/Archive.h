// Replaces CArchive to allow for Unicode internally and ansi in external files
// Copyright Bob -- K6RWY, 2019

// Streaming output to the Archive File
//
// The Archive object is usually defined as "ar" (i.e. Archive ar) and is passed from function to
// function as &ar.  The file written by the streaming output is raw text (i.e. ascii characters).
// It is a streaming object in that entities are output to the file in the order they appear in the
// statement:
//
//   void fn(Archive& ar) {
//   String s = _T(" Test 2");
//
//     ar << _T("text") << s << aCrlf;        // Streaming means that the elements are output in
//     }                                      // the order they appear in the statement.  This
//                                            // example results in "text Test 2\n" where \n is a
//                                            // carriage return/line feed on the display.

// So here is the list of entities that may bestreamed.  Please note that the "\n and \r may not
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

//   ar << s;                                 // Writes a String
//   ar << cs;                                // Writes a CString
//   ar << bs;                                // Writes a bstring
//   ar << ch;                                // Writes a Unicode character
//   ar << cc;                                // Writes a 8 bit c string terminated by zero
//   ar << intV;                              // Writes an integer
//   ar << sizeV;                             // Writes an size_t (a typed integer)
//   ar << longV;                             // Writes a long integer
//   ar << ulongV;                            // Writes a unsigned long integer
//   ar << dblV;                              // Writes a double floating point value
//   ar << date;                              // Writes a date (as defined by the Date class)

// The following entities modify the stream following as described:

//   ar << aClrTabs;                          // Clear all Tabs
//   ar << aTab;                              // Move the next text to the next tab stop
//   ar << aCenter;                           // Center the next text on the line
//   ar << aRight;                            // Move the next text so that the right side of the
//                                            // text is at the right edge of the display
//   ar << aBeginLine;                        // Start an underline at the current position on the
//                                            // display
//   ar << aEndLine;                          // Stop underlining at the current position on the
//                                            // display
//   ar << aCrlf;                             // Execute a carriage return/line feed on the display

// The following modifiers take an argument that are used at the current position on the display
// The position values indicate the number of characters in the line of the tab position.

//   ar << aSetTab(<int>);                    // Sets a tab such that a nTab move the current
//                                            // position to position indicated by the <int>
//   ar << aSetRTab(<int>);                   // Sets a tab such that a nTab moves the next text
//                                            // such that the right end of the text fragment is
//                                            // at the position indicated by the <int>
//   ar << aSetLMargin(<double>>);            // Set the left margin on the screen

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



#pragma once
#include "ArchPos.h"
#include "Date.h"
#include "ManipT.h"
#include "NoteNmbr.h"

class NotePad;


class CDoc;

class Archive;

typedef ManipT<Archive>    ArManip;
typedef ManipIntT<Archive> ArManipInt;
typedef ManipDblT<Archive> ArManipDbl;
typedef ManipStgT<Archive> ArManipStg;



class Archive : public ArchPos {

ArchLine arLine;

NoteNmbr nmbr;

public:
           Archive(String& fileName, int mode);
           Archive(void*   arbObj,   int mode) : ArchPos(arbObj, mode) { }
          ~Archive() { }

  bool     isOpen()    {return ArchFile::isOpen();}
  bool     isStoring() {return ArchFile::isStoring();}
  void     seekEnd()   {ArchFile::seekEnd();}

  Archive& operator << (NotePad& np);                      // Archive the content of specified notepad

  Archive& operator << (TCchar*        tc) {return append(tc);}
  Archive& operator << (String&         s) {return append(s);}
  Archive& operator << (const CString& cs) {return append(cs);}
  Archive& operator << (const bstr_t   bs) {return append((TCchar*) bs);}
  Archive& operator << (Tchar          ch) {arLine.append(ch); return *this;}

  Archive& operator << (int             v) {return append((long) v);}
  Archive& operator << (long            v) {return append(v);}
  Archive& operator << (ulong           v) {return append(v);}

  Archive& operator << (double          v) {return append(v);}
  Archive& operator << (Date&          dt) {return append(dt);}

  Archive& operator <<(ArManip&        m) {return m.func(*this);}
  Archive& operator <<(ArManipInt&     m)
                            {NewAlloc(ArManipInt); m.func(*this, m.v); FreeNode(&m); return *this;}
  Archive& operator <<(ArManipDbl&     m)
                            {NewAlloc(ArManipDbl); m.func(*this, m.v); FreeNode(&m); return *this;}
  Archive& operator <<(ArManipStg&     m)
                            {NewAlloc(ArManipStg); m.func(*this, m.v); FreeNode(&m); return *this;}

  // Read from file, interpreting \n or \r

  String   getFilePath()              {return ArchFile::getFilePath();}

  bool     read(String&            s) {return ArchFile::read(s);}    // Interpretation of \n or \r
  bool     read(String& s,     int n) {return ArchFile::read(s, n);} // Read n chars into a string
  bool     read(Tchar&            ch) {return ArchFile::read(ch);}

// Binary Read/Write

  bool     wrtBin(int           x)         {return ArchFile::wrtBin(x);}
  bool     wrtBin(ulong         x)         {return ArchFile::wrtBin(x);}
  bool     wrtBin(const void* blk, int n)  {return ArchFile::wrtBin(blk, n);}
  bool     wrtBin(Byte          v)         {return ArchFile::wrtBin(v);}
  int      spaces(int     nSpaces);

  bool     readBin(int&         x)         {return ArchFile::readBin(x);}
  bool     readBin(ulong&       x)         {return ArchFile::readBin(x);}
  bool     readBin(void*      blk, int& n) {return ArchFile::readBin(blk, n);}
  bool     readBin(Byte&        v)         {return ArchFile::readBin(v);}

private:

  void     initialize();
  Archive& append(TCchar* tc) {arLine.append(tc);   return *this;}
  Archive& append(Cchar*  cs);
  Archive& append(long     v);
  Archive& append(ulong    v);
  Archive& append(double   v);
  Archive& append(Date&   dt);


  static Archive& doSetLMargin(Archive& n, double v);   // Left Margin relative to physical left
                                                        // edge, eg 2 spaces from left edge of file
  static Archive& doSetRMargin(Archive& n, double v);   // Right Margin relative to page Width
  static Archive& doPageWidth( Archive& n, int    v);   // Page width in characters (default 99)

  static Archive& doSetTab(    Archive& n, int    v);
  static Archive& doSetRTab(   Archive& n, int    v);


  static Archive& doSetWidth(Archive& n, int v);        // double width and precision
  static Archive& doSetPrec( Archive& n, int v);


  Archive& setLMargin(double v) {flush();   ArchPos::setLMargin(v);     return *this;}
  Archive& setRMargin(double v) {flush();   ArchPos::setRMargin(v);     return *this;}
  Archive& setPageWidth(int  v) {flush();   ArchPos::setPageWidth(v);   return *this;}
  Archive& setTab(      int  v) {flush();   ArchPos::setTab(v, false);  return *this;}
  Archive& setRTab(     int  v) {flush();   ArchPos::setTab(v, true);   return *this;}
  Archive& clrTabs()            {flush();   ArchPos::clrTabs();         return *this;}
  Archive& tab()                {flush();   arLine.tabSeen   = true;    return *this;}
  Archive& center()             {flush();   arLine.centerIt  = true;    return *this;}
  Archive& right()              {flush();   arLine.rightLine = true;    return *this;}
  Archive& beginULine()         {           arLine.begUndrLn = true;    return *this;}
  Archive& endULine()           {flush();   arLine.endUndrLn = true;    return *this;}
  Archive& setHex()             {flush();   nmbr.hex         = true;    return *this;}
  Archive& crlf()               {flush();   ArchPos::crlf();            return *this;}

  Archive& flush();

  static Archive& doClrTabs(  Archive& n) {n.flush(); return n.clrTabs();}
  static Archive& doTab(      Archive& n) {n.flush(); return n.tab();}
  static Archive& doCenter(   Archive& n) {n.flush(); return n.center();}
  static Archive& doRight(    Archive& n) {n.flush(); return n.right();}

  static Archive& doBeginLine(Archive& n) {n.flush(); return n.beginULine();}
  static Archive& doEndLine(  Archive& n) {n.flush(); return n.endULine();};
  static Archive& doSetHex(   Archive& n) {n.flush(); return n.setHex();}

  static Archive& doCrlf(     Archive& n) {n.flush(); return n.crlf();}

  Archive() : ArchPos(*(String*)0, 0) { }

  friend ArManipDbl& aSetLMargin(double val);   // Set left margin (no. chars)
  friend ArManipDbl& aSetRMargin(double val);   // Set right margin (no. chars)
  friend ArManipInt& aPageWidth( int    val);   // Set width (length) of line in file (no. chars)
  friend ArManipInt& aSetTab(    int    val);   // Set a tab (in no. chars) on the line
  friend ArManipInt& aSetRTab(   int    val);   // Set a right tab (end of fragment ends at tab)
  friend ArManipInt& aSetWidth(  int    val);   // Set width of double output (padded with spaces)
  friend ArManipInt& aSetPrec(   int    prec);  // Set no. of digits after decimal point of double
//  friend class NotePad;
  friend class ArchiveNtPd;
  };


extern ArManip aClrTabs;    // add to stream to clear tabs:                 dsp << dClrTabs;
extern ArManip aTab;        // add to stream to tab to next tab position:   dsp << dTab << "xyz";
extern ArManip aCenter;     // Set bit to center from here to next tab or crlf;
extern ArManip aRight;      // Set bit to right align from here to crlf;

extern ArManip aBeginLine;  // Begin and end line under text.
extern ArManip aEndLine;
extern ArManip aSetHex;     // Set hex modifier for next integer value (unsigned or signed)
extern ArManip aCrlf;       // add to stream to terminate a line on display: ar << "xyz" << nCrlf;

ArManipDbl& aSetLMargin(double val);  // Set left margin (no. chars)
ArManipDbl& aSetRMargin(double val);  // Set right margin (no. chars)
ArManipInt& aPageWidth( int    val);  // Set width (length) of line in file (no. chars)

ArManipInt& aSetTab(    int    val);  // Set tab at Tchar pos given by val (using avg Tchar width)
ArManipInt& aSetRTab(   int    val);

ArManipInt& aSetWidth(  int    val);  // double width value
ArManipInt& aSetPrec(   int    prec); // double precision value



