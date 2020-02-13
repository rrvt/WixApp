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

int leftEdge;                     // X offset from left edge to first output pixel and right
                                  // edge to last output pixel (for printer at least)
int rightEdge;                    // right edge of output window
int leftMargin;

int threshold;                    // Used to decide on automatic crlf in line

Expandable<Tab, 0> tabs;         // Tab positions/Left/Right

public:

int cursorPos;                    // Current Horizonal Position
                                  //
int width;                        // Width of one character
int lastWidth;
int maxCursorPos;

  TextPosition() {initialize();}

  void initialize()
    {width = 1; leftMargin = leftEdge = rightEdge = lastWidth = cursorPos = maxCursorPos = threshold = 0;}

  void iPos(int left, int right) {
    maxCursorPos = cursorPos = leftEdge = left; rightEdge = right; threshold = (right - left)/2 + left;
    }

  void pos(int x)  {cursorPos  = x;}         // Change position to x
  bool exceedsRtMargin(int width);           // return true when new position would exceed right margin
  void move(int x) {cursorPos += x;}         // Move Cursor by the number of characters specified
  int  get()       {return cursorPos;}       // Returns the current cursor position in pixels
  bool pastThreshold() {return cursorPos >= threshold;}
  int  getCharPos(){return width ? (cursorPos - leftEdge)/width - leftMargin: 0;}
                                             // Returns the current cursor position in characters
  void setLeftMargin(int lm) {leftMargin = lm >= 0 ? lm : 0;}
  void clrTabs()   {tabs.clr();}             // Clear Tabs
  void setTab( int charPos);                 // Insert an character position for the next
                                             // tab position
  void setRTab(int charPos);                 // Insert an character position for the next right
                                             // tab position
  Tab tab();                                 // return position of next tab and sets cursorPos
  Tab findNextTab();
  void centerPos(int width) {pos((rightEdge - leftEdge - width) / 2 + leftEdge);}
  void rightPos(int width)  {pos(rightEdge - width);}
  void rightTabPos(Tab& tab, int width) {pos(tab.pos - width);}

  void crlf() {
    if (cursorPos > maxCursorPos) maxCursorPos = cursorPos;

    cursorPos = leftEdge + leftMargin * width; width = lastWidth ? lastWidth : 1;
    }

private:

  int charsPerLine() {return (rightEdge - leftEdge) / width;}
  };
