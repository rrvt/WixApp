// List of tab in any medium


#pragma once
#include "ExpandableP.h"
#include "IterT.h"

class CDCex;


class Tab {
public:

int  val{0};
bool right{false};

  Tab() { }
 ~Tab() { }

 void  set(int val, bool r) {this->val = val; right = r;}

  bool operator>= (Tab& t)     {return val >= t.val;}
  bool operator>  (int curPos) {return val >  curPos;}
  };


class TabList;
typedef DatumPtrT<Tab, int> TabP;
typedef IterT<TabList, Tab> TLIter;                         // Iterator for the TabList


class TabList {

CDCex& cdcEx;
int    avgCharWidth{0};

ExpandableP<Tab, int, TabP, 2> data;

public:

  TabList(CDCex& cdcEX) : cdcEx(cdcEX) { }
 ~TabList() {clear();}

  void    clear() {data.clear();   avgCharWidth = 0;}
  void    set(int pos, bool right);
  int     find(int curPos);         // Find next tab position after curPos at saved avg char width

private:

  int    getAvgCharWidth();

  // returns either a pointer to data (or datum) at index i in array or zero

  Tab*   datum(int i) {return 0 <= i && i < nData() ? data[i] : 0;}

  int    nData()      {return data.end();}                // returns number of data items in array
                                                          // not necessarily private
  void   removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

  TabList() : cdcEx(*(CDCex*)0) { }

  friend typename TLIter;
  };

