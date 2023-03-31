// Managing the Text Position on a Screen or in the output to a file
// The unit of horizontal position is one character width (width).


#include "pch.h"
#include "TextPosition.h"
#include "InsertSorted.h"


// return true when new position would exceed right margin

bool TextPosition::exceedsRtMargin(int width) {return cursorPos + width > rightEdge;}


int TextPosition::remaining() {
  return rightEdge - cursorPos;
  }


// Insert an approximate character position for the next tab position

void TextPosition::setTab(int charPos, bool right) {
Tab xTab;

  xTab.pos = (charPos > 0 ? charPos : (charsPerLine() + charPos)) * width + leftEdge;

  xTab.right = right;   tabs = xTab;
  }



// set cursorPos to next tab position and return new position and type

Tab TextPosition::tab() {
Tab tab = findNextTab();
  cursorPos = tab.pos; return tab;
  }


Tab TextPosition::findNextTab() {
int i;
int n, newCurPos;
Tab curPos;

  for (i = 0; i < tabs.end(); i++) if (tabs[i].pos > cursorPos) return tabs[i];

  n = (cursorPos - leftEdge + width - 1) / width;  n = (n + 4) / 5;

  newCurPos = n * 5 * width + leftEdge;

  if (newCurPos - cursorPos < width) newCurPos += 5 * width;

  curPos.pos = newCurPos; curPos.right = false; return curPos;
  }


