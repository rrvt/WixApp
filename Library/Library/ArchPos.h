// Manage Archive Output Position


#pragma once
#include "ArchFile.h"
#include "ArchULines.h"
#include "Tabs.h"


struct ArchLine {
bool   tabSeen;                     // A Tab has been seen
bool   centerIt;                    // Center Fragment in line width
bool   rightLine;                   // Right Adjust Fragment at the right edge of line
bool   begUndrLn;                   // Save Begin Underline at current position
bool   endUndrLn;                   // Save End Underline at current position

String frag;

  ArchLine() : tabSeen(false), centerIt(false), rightLine(false),
                                                             begUndrLn(false), endUndrLn(false) { }
  void clear() {tabSeen = centerIt = rightLine = begUndrLn = endUndrLn = false; frag.clear();}

  bool isEmpty() {return frag.isEmpty();}

  void append(TCchar* tc) {frag += tc;}
  void append(TCchar  ch) {frag += ch;}
  };


// Manage Position, unit is one character

class ArchPos : protected ArchFile {

double     leftMargin;                // Left Margin in characters, eg 2 spaces from physical left
                                      // edge of file
double     rightMargin;               // Right Margin in characters, eg 2 spaces from page width
int        pageWidth;                 // Width of line in characters, default is 99

int        curPos;                    // Current Position in characters

Tabs       tabs;                          ////  ************ Needs Work ***********
ArchULines uLines;

public:

  ArchPos(TCchar* fileName, int mode);

  ArchPos(void* arbObj, int mode) : ArchFile(arbObj, mode), tabs(int(pageWidth - rightMargin)) { }
 ~ArchPos() { }

  void     clear();

  void     setLMargin(double v) {leftMargin   = v;}
  void     setRMargin(double v) {rightMargin  = v;}
  void     setPageWidth(int  v) {pageWidth    = v;}

  void     clrTabs() {tabs.clear();}
  void     setTab(int pos, bool right) {tabs.set(pos, right);}

  void     write(ArchLine& arLine);
  bool     write(Tchar     ch)            {return ArchFile::write(ch);}
  bool     write(TCchar*   tc)            {return ArchFile::write(tc);}

  void     crlf();

// Binary Read/Write

  bool     wrtBin(int           x)        {return ArchFile::wrtBin(x);}
  bool     wrtBin(ulong         x)        {return ArchFile::wrtBin(x);}
  bool     wrtBin(const void* blk, int n) {return ArchFile::wrtBin(blk, n);}
  bool     wrtBin(Byte          v)        {return ArchFile::wrtBin(v);}

  bool     readBin(int&    x)             {return ArchFile::readBin(x);}
  bool     readBin(ulong&  x)             {return ArchFile::readBin(x);}
  bool     readBin(void* blk, int& n)     {return ArchFile::readBin(blk, n);}
  bool     readBin(Byte&   v)             {return ArchFile::readBin(v);}

private:

  bool     tab(int lng);
  };



///-----------------

//  void     setTabSize(int nSpaces);
//  void     tabIndex(int index);


