// Tabs needed during output to a device


#pragma once
#include "Tabs.h"

class Horiz;



struct RealTab {
double pos;                                        // Tab position in pixels
bool   right;                                      // right tab when true;

  void clear() {pos = 0; right = false;}
  };



class DevTabs : Tabs {

public:

           DevTabs();
           DevTabs(DevTabs& tm) : Tabs(Tabs::lineWidth) {copy(tm);}
          ~DevTabs() { }

  void     clear()   {Tabs::clear();}             // curTab.clear();

  DevTabs& operator= (DevTabs& tm) {copy(tm); return *this;}

  void     setTab( Horiz& hz, NoteTab& tab);

  RealTab  findNextTab(Horiz& hz);

private:

  void     setLineWidth(Horiz& hz);
  double   getCharWidth(Horiz& hz);

  void     copy(DevTabs& tm) {Tabs::copy(tm);}
  };




//////------------------

#if 0
struct NoteTab {
double pos;                                        // Tab position in pixels
bool   right;                                      // right tab when true;

  NoteTab() {clear();}
 ~NoteTab();
  NoteTab(NoteTab& devTab) {copy(devTab);}

  void  clear() {pos = 0.0; right = false;}

  NoteTab& operator=  (NoteTab& devTab) {copy(devTab); return *this;}

  bool  operator== (NoteTab& tab) {return pos == tab.pos;}
  bool  operator>= (NoteTab& tab) {return pos >= tab.pos;}
private:

  void copy(NoteTab& devTab) {pos = devTab.pos; right = devTab.right;}
  };


class DevTabs;
typedef DatumPtrT<NoteTab, double> DevTabP;
typedef IterT<DevTabs, NoteTab>    DvTbIter;
#endif
//#include "ExpandableP.h"
//#include "IterT.h"
//  void   set(Horiz& hz, int pos, TabPos& tab);
#if 0
private:

  // returns either a pointer to data (or datum) at index i in array or zero

  TabPos* datum(int i) {return 0 <= i && i < nData() ? data[i].p : 0;}

  int     nData()      {return data.end();}     // returns number of data items in array

  void    removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

  friend typename DvTbIter;
#endif

//ExpandableP<NoteTab, double, DevTabP, 2> data;
//TabPos dfltTab;
//TabPos* cur;                      // points to current tab structure
//NoteTab curTab;

  //  void     setTab( Horiz& hz, int pos);         // Insert an character position for the next
  //                                                // tab position
  //  void     setRTab(Horiz& hz, int pos);         // Insert an character position for the next right

