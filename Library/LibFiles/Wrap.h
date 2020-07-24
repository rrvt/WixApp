// Wrap --


#pragma once
#include "Expandable.h"
#include "IterT.h"


class Wrap;
typedef IterT<Wrap, String> WrapIter;                     // Establish the typename so it can be friended


class Wrap {
bool enabled;
CDC* dc;
int  extent;
bool isItalic;

Expandable<String, 2> lines;

public:

  Wrap()            : enabled(true), dc(0), extent(0), isItalic(false) { }
  Wrap(bool enable) : enabled(enable) { }
  Wrap(Wrap& w) {copy(w);}
 ~Wrap() { }

  Wrap& operator= (Wrap& w) {copy(w); return *this;}

  void initialize(CDC* pDC, int widthLeft, bool isItalic);

  int operator() (TCchar* p);        //, int noChars, int noTabs

  bool isEmpty() {return lines.end() == 0;}
  int  noLines() {return lines.end();}

private:

  // returns number of data items in array
  int nData() {return lines.end();}

  // returns either a pointer to data (or datum) at index i in array or zero
  String* datum(int i) {return 0 <= i && i < lines.end() ? &lines[i] : 0;}

  void copy(Wrap& w)
          {enabled = w.enabled; dc = w.dc; extent = w.extent; isItalic =   w.isItalic; lines = w.lines;}

  int  findLast(Tchar ch, String& s);
  int  findLastChar(String& s);
  int  width(String& s);

  int  findLastWh(String& s, int noChars);

  friend typename WrapIter;
  };
