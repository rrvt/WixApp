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
//#include "DeviceContext.h"
//#include "DevTabs.h"


class Horiz {

int    pgWidth;
double leftBnd;                               // Computed from the attributes
double rightBnd;

double maxPos;

public:

double position;
double charWidth;

  Horiz() {clear();}
 ~Horiz() { }

  void     clear();

  Horiz&   operator= (Horiz& hrz);

  void     setPgWidth(int pgWdth) {pgWidth = pgWdth;}
  void     initialize(double leftMgn, double rightMgn);

  void     setEdges(double leftMgn, double rightMgn);
  void     setLeftEdge( double leftMgn);
  void     setRightEdge(double rightMgn);

  double   pos()           {return position;}
  void     set(double pos) {position = pos;}
  int      currentPos()    {return int(position);}
  int      maxX()          {return int(maxPos);}
  void     move(int width) {position += width;}
  bool     isAtLftBnd()    {return position == leftBnd;}

  int      remaining()  {return int(rightBnd - position);}      // Used in wrapping
  int      maxExtent()  {return int(rightBnd - leftBnd);}       // maximum space for char on line
  void     restorePos(double pos) {position = pos;}

  int      widthInCh();                         // Window int(Width / charWidth)



//  double   avgLgChWidth() {return dvx.avgLgChWidth;}          // avg of large chars

  // Operate on text width to position the text

  void centerText(int width) {position = (rightBnd + leftBnd - width) / 2;}
  void rightText(int width)  {position =  rightBnd - width;}

  void cr();

private:

  friend class  DevTabs;
  };


