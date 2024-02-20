// Managing the Text Position on a Screen or in the output to a file
// The unit of horizontal position is one character width (width).
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.

#pragma once
#include "Expandable.h"


struct Tab {
int  pos;                         // Tab position in pixels
bool right;                       // right tab when true;

  Tab() {pos = 0; right = false;}

  bool operator== (Tab& tab) {return pos == tab.pos;}
  bool operator>= (Tab& tab) {return pos >= tab.pos;}
  Tab& operator=  (Tab  tab) {pos = tab.pos; right = tab.right; return *this;}
  };


class TextPosition {
public:

int cursorPos;                    // Current Horizonal Position
int maxCursorPos;

int width;                        // Width of one character
int lastWidth;

private:

int leftEdge;                     // X offset from left edge to first output pixel and right
                                  // edge to last output pixel (for printer at least)
int rightEdge;                    // right edge of output window
int leftOdd;


Expandable<Tab, 0> tabs;          // Tab positions/Left/Right

public:

  TextPosition() {initialize();}

  void initialize() {
    width = 1; leftEdge = rightEdge = leftOdd = cursorPos = maxCursorPos = lastWidth = 0;
    tabs.clear();
    }

  void setLeftMargin( int lm)
                      {leftOdd = lm >= 0 ? lm : 0;  maxCursorPos = cursorPos = getInitialCursorPos();}
  void iPos(int left, int right)
                  {leftEdge = left; rightEdge = right; maxCursorPos = cursorPos = getInitialCursorPos();}

  void pos(int x)  {cursorPos  = x;}         // Change position to x
  bool exceedsRtMargin(int width);           // return true when new position would exceed right margin
  int  remaining();
  int  widthCh()   {return width;}
  void move(int x) {cursorPos += x;}         // Move Cursor by the number of characters specified
  int  get()       {return cursorPos;}       // Returns the current cursor position in pixels
  int  getCharPos(){return width ? (cursorPos - leftEdge)/width - leftOdd: 0;}
                                             // Returns the current cursor position in characters

  void clrTabs()   {tabs.clear();}           // Clear Tabs
  void setTab( int charPos, bool right);     // Insert an character position for the next
                                             // tab position
  Tab tab();                                 // return position of next tab and sets cursorPos
  Tab findNextTab();
  void centerPos(int width) {pos((rightEdge - leftEdge - width) / 2 + leftEdge);}
  void rightPos( int width) {pos(rightEdge - width);}
  void rightTabPos(Tab& tab, int width) {pos(tab.pos - width);}

  void doCR() {
    if (cursorPos > maxCursorPos) maxCursorPos = cursorPos;

    cursorPos = getInitialCursorPos(); width = lastWidth ? lastWidth : 1;
    }
   int getPortWidth() {return rightEdge - leftEdge;}

private:

  int charsPerLine() {return (rightEdge - leftEdge) / width;}
  int getInitialCursorPos() {return leftEdge + leftOdd * width;}
  };

