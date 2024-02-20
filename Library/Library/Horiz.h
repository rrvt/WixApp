// Horizonal line management for display/printer output


/*
The window (display or printer) has an outer boundary and a output boundary:
leftEdge          leftBnd                                    rightBnd           rightEdge
    |<--leftOdd-->|<- - - - - - - output area - - - - - - - ->|<-- rightMargin-->|
                     |<-Tab1->|<--Tab2-->|...
    |<- - - - - - - - - - - - - - - - - - - position ->|

  Left Edge is always zero.
  Tabs are measured in average character widths (e.g. 2.3 * avgLgChWidth
  All attributes are kept as floating point (double) and converted to integer for use on output
   -- avgLgChWidth
   -- leftOdd
   -- rightMargin
   -- position
*/

#pragma once
#include "DevCtx.h"
#include "DevTabs.h"


class Horiz {
public:

double    position;

private:

DevCtx&   dvx;

double    leftBnd;                  // Computed from the attributes
double    rightBnd;

double    maxPos;

public:

  Horiz(DevCtx& devCtx) : dvx(devCtx) {clear();}
  Horiz() : dvx(*(DevCtx*)0) { }
 ~Horiz() { }

  void     clear();

  void     initialize() {setLftEdge();  setRtEdge();  maxPos = position = leftBnd;}
  void     setEdges()   {setLftEdge();  setRtEdge();}
  void     setLftEdge() {leftBnd  = dvx.leftMgn * dvx.edgeChWidth;}
  void     setRtEdge()  {rightBnd = dvx.pgWidth - dvx.rightMgn * dvx.edgeChWidth;}

  double   pos()           {return position;}
  void     set(double pos) {position = pos;}
  int      currentPos()    {return int(position);}
  int      maxX()          {return int(maxPos);}
  void     move(int width) {position += width;}
  bool     isAtLftBnd()    {return position == leftBnd;}

  int      remaining()  {return int(rightBnd - position);}      // Used in wrapping
  int      maxExtent()  {return int(rightBnd - leftBnd);}       // maximum space for char on line
  void     restorePos(double pos) {position = pos;}

  double   avgLgChWidth() {return dvx.avgLgChWidth;}

  // Operate on text width to position the text

  void centerText(int width) {             position = (rightBnd - leftBnd - width) / 2 + leftBnd;}
  void rightText(int width) {              position =  rightBnd - width;}
  void rightTabText(double tabPos, int width) {position =  tabPos  - width;}

  void cr();

private:

  friend class  DevTabs;
  friend class  TxtOut;
  friend struct TxtOps;
  };


