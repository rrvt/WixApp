// Manage Tabs in pixels or characters depending on where utilized


#include "pch.h"
#include "Tabs.h"




// Insert an approximate character position for the next tab position

void Tabs::set(int charPos, bool right) {
NoteTab xTab;

  xTab.pos  = charPos > 0 ? charPos : lineWidth + charPos;

  xTab.right = right;   data = xTab;
  }


NoteTab Tabs::find(int curPos) {
TabsIter iter(*this);
NoteTab*  dtm;
int      n;
NoteTab   tabPos;

  for (dtm = iter(); dtm; dtm = iter++) if (dtm->pos > curPos) return *dtm;

  n = (curPos + 4) / 5;   n = n * 5;

  if (n < curPos) n += 5;    tabPos.pos = n;   return tabPos;
  }



////--------------

#if 1
#else
  n = (cursorPos - leftEdge + width - 1) / width;  n = (n + 4) / 5;

  newCurPos = n * 5 * width + leftEdge;

  if (newCurPos - cursorPos < width) newCurPos += 5 * width;

  curPos.pos = newCurPos; curPos.right = false; return curPos;
#endif

