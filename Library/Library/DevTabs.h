// Tabs needed during output to a device


#pragma once
#include "ExpandableP.h"
#include "IterT.h"

class Horiz;


struct DevTab {
double pos;                                        // Tab position in pixels
bool   right;                                      // right tab when true;

  DevTab() {clear();}
 ~DevTab();
  DevTab(DevTab& devTab) {copy(devTab);}

  void  clear() {pos = 0.0; right = false;}

  DevTab& operator=  (DevTab& devTab) {copy(devTab); return *this;}

  bool  operator== (DevTab& tab) {return pos == tab.pos;}
  bool  operator>= (DevTab& tab) {return pos >= tab.pos;}
private:

  void copy(DevTab& devTab) {pos = devTab.pos; right = devTab.right;}
  };


class DevTabs;
typedef DatumPtrT<DevTab, double> DevTabP;
typedef IterT<DevTabs, DevTab>    DvTbIter;


class DevTabs {

ExpandableP<DevTab, double, DevTabP, 2> data;
DevTab                                  dfltTab;

public:

DevTab* cur;                  // points to current tab structure

  DevTabs() : cur(&dfltTab) { }
  DevTabs(DevTabs& tm) {copy(tm);}
 ~DevTabs() { }

  void   clear()   {data.clear();  reset();}
  void   reset()   {cur = &dfltTab;  dfltTab.clear();}

  DevTabs& operator= (DevTabs& tm) {copy(tm); return *this;}

  void   setTab( Horiz& hz, int pos);           // Insert an character position for the next
                                                // tab position
  void   setRTab(Horiz& hz, int pos);           // Insert an character position for the next right

  void   findNextTab(Horiz& hz);

private:

  void   copy(DevTabs& tm) {cur = tm.cur; data = tm.data; dfltTab = tm.dfltTab;}

  void   set(Horiz& hz, int pos, DevTab& tab);

private:

  // returns either a pointer to data (or datum) at index i in array or zero

  DevTab* datum(int i) {return 0 <= i && i < nData() ? data[i].p : 0;}

  int     nData()      {return data.end();}                      // returns number of data items in array

  void    removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

  friend typename DvTbIter;
  };

