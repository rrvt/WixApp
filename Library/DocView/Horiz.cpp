// Horizonal line management for display/printer output


#include "pch.h"
#include "Horiz.h"


void Horiz::clear() {pgWidth = 0; leftBnd = rightBnd = maxPos = position = charWidth = 0;}


Horiz& Horiz::operator= (Horiz& hrz) {
    pgWidth  = hrz.pgWidth;   leftBnd  = hrz.leftBnd;    rightBnd  = hrz.rightBnd;
    maxPos   = hrz.maxPos;    position = hrz.position;   charWidth = hrz.charWidth;   return *this;
    }


void Horiz::initialize(double leftMgn, double rightMgn)
                                       {setEdges(leftMgn, rightMgn);  maxPos = position = leftBnd;}

void Horiz::setEdges(double leftMgn, double rightMgn)
                                                   {setLeftEdge(leftMgn);  setRightEdge(rightMgn);}

void Horiz::setLeftEdge(double leftMgn) {
double lastLeft = leftBnd;

  leftBnd  = leftMgn * charWidth;   if (position == lastLeft) position = leftBnd;
  }

void Horiz::setRightEdge(double rightMgn) {rightBnd = pgWidth - rightMgn * charWidth;}


void Horiz::cr() {if (position > maxPos) maxPos = position;  position = leftBnd;}


// Window Width / charWidth

int Horiz::widthInCh() {return int((rightBnd - leftBnd) / charWidth + 0.5);}



////-----------

// Midway between char set avg and avg of large chars

//double Horiz::charWidth() {return (dvx.avgLgChWidth + dvx.avgFlChWidth) / 2;}

