// Horizonal line management for display/printer output


/*
The window (display or printer) has an outer boundary and a output boundary:
leftEdge          leftBnd                                    rightBnd           rightEdge
    |<--leftMargin-->|<- - - - - - - output area - - - - - - - ->|<-- rightMargin-->|
                     |<-Tab1->|<--Tab2-->|...
    |<- - - - - - - - - - - - - - - - - - - position ->|

  Left Edge is always zero.
  Tabs are measured in average character widths (e.g. 2.3 * avgLgChWidth
  All attributes are kept as floating point (double) and converted to integer for use on output
   -- avgLgChWidth
   -- leftMargin
   -- rightMargin
   -- position
*/

#pragma once
#include "Expandable.h"


class Horiz;


struct TabX {
double pos;                                        // Tab position in pixels
bool   right;                                      // right tab when true;

  TabX() {clear();}
  TabX(TabX& tabX) {copy(tabX);}

  void  clear() {pos = 0.0; right = false;}

  TabX& operator=  (TabX& tabX) {copy(tabX); return *this;}

  bool  operator== (TabX& tab) {return pos == tab.pos;}
  bool  operator>= (TabX& tab) {return pos >= tab.pos;}
private:

  void copy(TabX& tabX) {pos = tabX.pos; right = tabX.right;}
  };


class TabMgmt {

Expandable<TabX, 2> tabs;

public:

  TabMgmt() { }
 ~TabMgmt() { }

  void   clear()   {tabs.clear();}

  void   setTab( Horiz& hz, int pos);           // Insert an character position for the next
                                                // tab position
  void   setRTab(Horiz& hz, int pos);           // Insert an character position for the next right

  void   findNextTab(Horiz& hz, TabX& tabX);

private:

  void   set(Horiz& hz, int pos, TabX& tab);
  };


class Horiz {
double   position;
double   maxPos;

double   rightEdge;               // Attributes set from outside
double   leftMgn;
double   leftBase;
double   rightMgn;

double   leftBnd;                 // Computed from the attributes
double   rightBnd;
double   avgLgChWidth;
double   avgFlChWidth;

TabMgmt  tabMgmt;
double   savePos;                 // used to save and restore the position (1 layer stack?)

public:

  Horiz() {clear();}
 ~Horiz() { }

  void clear();

  void setAttributes(int width, double leftMargin, double rightMargin);
  void setLeftMargin( double v);
  void setRightMargin(double v);
  void setAvgLgChWidth(CDC* dc);

  void clrTabs()       {tabMgmt.clear();}
  void setTab(int  v)  {tabMgmt.setTab( *this, v);}
  void setRTab(int v)  {tabMgmt.setRTab(*this, v);}
  void tab(TabX& tabX) {tabMgmt.findNextTab(*this, tabX);  position = tabX.pos;}

  int  currentPos() {return int(position);}
  int  maxX()       {return int(maxPos);}
  void move(int width) {position += width;}
  bool isAtLftBnd() {return position == leftBnd;}

  int  remaining()  {return int(rightBnd - position);}      // Used in wrapping
  int  maxExtent()  {return int(rightBnd - leftBnd);}       // maximum space for char on line
  void saveCurPos() {savePos = position;}                   // Save and restore current position
  void restorePos() {position = savePos;}                   // Used for wrapping text...

  int  charIndex() {return int((position - leftBnd) / avgLgChWidth);}    // index of char in line (approx)
  int  lgChWidth() {return int(avgLgChWidth + 0.5);}
  int  flChWidth() {return int(avgFlChWidth + 0.5);
  }

  // Operate on text width to position the text

  void centerText(int width) {             position = (rightBnd - leftBnd - width) / 2 + leftBnd;}
  void rightText(int width) {              position =  rightBnd - width;}
  void rightTabText(TabX& tab, int width) {position =  tab.pos  - width;}

  void cr();

private:

  void initPos()   {maxPos = position = leftBnd = leftMgn  * avgLgChWidth;}
  void setRtEdge() {       rightBnd = rightEdge - rightMgn * avgLgChWidth;}

  friend class TabMgmt;
  };

