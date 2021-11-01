// Report Base


#pragma once
#include "Expandable.h"
#include "IterT.h"


class CScrView;
class Device;
class NotePad;


struct RBtab {
int  pos;
bool right;

public:

  RBtab() : pos(0), right(false) { }
  RBtab(RBtab& tb) {copy(tb);}
 ~RBtab() { }

  RBtab& operator= (RBtab& tb) {copy(tb); return *this;}

  bool  operator== (RBtab& tab) {return pos == tab.pos;}
  bool  operator>= (RBtab& tab) {return pos >= tab.pos;}

private:

  void copy(RBtab& tb) {pos = tb.pos; right = tb.right;}
  };


class ReportBase;
typedef IterT<ReportBase, RBtab> RbTbIter;


class ReportBase {
protected:

NotePad& np;

String   title;
int      noLines;
int      maxLines;
int      maxPages;

bool     printing;

Expandable<RBtab, 8> tabs;

public:

  ReportBase(NotePad& notePad) : np(notePad), noLines(0), maxLines(0),
                                                            maxPages(0), printing(false) {tabs.clear();}
 ~ReportBase() { }

          void setTitle(TCchar* t) {title = t;}

  virtual void display(CScrView& vw);
  virtual void print(CScrView& vw);
  virtual void footer(Device& dev, int pageNo);

protected:

  void detNoPages(CScrView& vw);

  virtual void create(CScrView& vw)               = 0;
  virtual int  header(NotePad& np, bool printing) = 0;

private:

  // returns either a pointer to data (or datum) at index i in array or zero

  RBtab* datum(int i) {return 0 <= i && i < nData() ? &tabs[i] : 0;}       // or data[i].p

  int    nData()      {return tabs.end();}                       // returns number of data items in array

  void   removeDatum(int i) {if (0 <= i && i < nData()) tabs.del(i);}

  friend typename RbTbIter;

  ReportBase() : np(*(NotePad*)0) { }
  };

