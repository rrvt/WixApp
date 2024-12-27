// Manage Tabs in characters
// Tab values reported are relative to whatever boundaries and width of a character that are
// maintained outside of the class.  The tab value is put into the data structure is returned
// unchanged.
//
// The left edge of the window/text/file for the purposes of Tabs is considered to be zero.
// The width of the window/text/file is initialized when the class starts.  Tabs are always
// computed in reference to that width.  The width does not change ever once intialized.

#pragma once
#include "Expandable.h"
#include "IterT.h"
#include "NoteTab.h"


class Tabs;
typedef IterT<Tabs, NoteTab> TabsIter;


class Tabs {

Expandable<NoteTab, 0> data;          // Tab positions/Left/Right

int lineWidth;                        // lineWidth (i.e. left edge to right edge) in characters

public:

  Tabs(int width) : lineWidth(width) { }
 ~Tabs() { }

  void    clear() {data.clear();}
  Tabs&   operator= (Tabs& tabs) {copy(tabs);   return *this;}

  void    setWidth(int nChars) {lineWidth = nChars;}  // Number of characters in line
  void    set(int charPos, bool right);               // Insert an character position for the next
                                                      // tab position
  NoteTab* get(int i) {return i < nData() ? &data[i] : 0;}
  NoteTab  find(int curPos);                          // find the next tab after the current
                                                      // Position
private:

  void copy(Tabs& tabs) {data = tabs.data;   lineWidth = tabs.lineWidth;}

  // returns either a pointer to data (or datum) at index i in array or zero

  NoteTab* datum(int i) {return 0 <= i && i < nData() ? &data[i] : 0;}

  int     nData()      {return data.end();}           // returns number of data items in array

  void    removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

  Tabs() : lineWidth(0) { }

  friend typename TabsIter;
  friend class    DevTabs;
  };




///-------------------

#if 0
struct NoteTab {
int  pos;
bool right;

  NoteTab() {pos = 0; right = false;}

  void    clear() {pos = 0;   right = false;}

  bool    operator== (NoteTab& tab) {return pos == tab.pos;}
  bool    operator>= (NoteTab& tab) {return pos >= tab.pos;}
  NoteTab& operator=  (NoteTab  tab) {pos = tab.pos; right = tab.right; return *this;}
  };
#endif

