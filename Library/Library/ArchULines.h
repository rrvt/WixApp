// Archive Underline Module


#pragma once
#include "Expandable.h"
#include "IterT.h"


class ArchULine {
public:
int beg;
int end;

  ArchULine() : beg(0), end(0) { }
 ~ArchULine() { }

  void clear() {beg = end = 0;}
  };


class ArchULines;
typedef IterT<ArchULines, ArchULine> ArULiter;


class ArchULines {

Expandable<ArchULine, 4> data;
ArchULine uLine;

public:

  ArchULines() { }
 ~ArchULines() { }

  void clear() {data.clear();}

  bool isEmpty() {return nData() == 0;}

  void setBeg(int pos) {uLine.beg = pos;}
  void setEnd(int pos) {uLine.end = pos;   data += uLine;   uLine.clear();}

private:

  // returns either a pointer to datum at index i in array or zero

  ArchULine* datum(int i) {return 0 <= i && i < nData() ? &data[i] : 0;}

  int        nData()      {return data.end();}            // Returns number of data items in array,
                                                          // not necessarily private
  void       removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

  friend typename ArULiter;
  };

