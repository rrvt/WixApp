// Manage Archive Output Position


#include "pch.h"
#include "ArchPos.h"


ArchPos::ArchPos(String& fileName, int mode) : ArchFile(fileName, mode), leftMargin(0),
                                               pageWidth(99), curPos(0), tabs(pageWidth) { }


void ArchPos::clear() {
  leftMargin = rightMargin = -1;   curPos = 0; pageWidth = 99;   tabs.clear();   uLines.clear();
  }


void ArchPos::write(ArchLine& arLine) {
int lng = arLine.frag.length();
int t;

  if (curPos < leftMargin) curPos = spaces(int(leftMargin - curPos));

  if      (arLine.tabSeen) arLine.tabSeen = !tab(lng);

  else if (arLine.centerIt && lng) {
    t = (pageWidth + int(leftMargin - rightMargin) - lng) / 2 - curPos;  curPos += spaces(t);
    arLine.centerIt = false;
    }

  else if (arLine.rightLine && lng) {
    t = pageWidth - int(rightMargin) - lng - curPos;

    curPos += spaces(t);   arLine.rightLine = false;
    }

  if (arLine.begUndrLn) {uLines.setBeg(curPos);   arLine.begUndrLn = false;}

  ArchFile::write(arLine.frag);    arLine.frag.clear();   curPos += lng;

  if (arLine.endUndrLn) {uLines.setEnd(curPos);   arLine.endUndrLn = false;}
  }


void ArchPos::crlf() {

  curPos = 0;   ArchFile::crlf();   if (uLines.isEmpty()) return;

  ArULiter   iter(uLines);
  ArchULine* uLine;
  int        i;

  for (uLine = iter(); uLine; uLine = iter++) {
    curPos += spaces(uLine->beg - curPos);

    for (i = uLine->beg; i < uLine->end; i++) {ArchFile::write(_T('-'));   curPos++;}
    }

  uLines.clear();   crlf();
  }


bool ArchPos::tab(int lng) {
NoteTab tabPos  = tabs.find(curPos - int(leftMargin));
int    nSpaces = tabPos.pos + int(leftMargin) - curPos;

  if (!tabPos.right) {curPos += spaces(nSpaces);         return true;}
  else if (lng)      {curPos += spaces(nSpaces - lng);   return true;}

  return false;
  }



//////////------------------------

#if 0
void ArchPos::setTabSize(int nSpaces) {
int i;

  tabs.clear();

  for (i = nSpaces; i < rightMargin; i += nSpaces) setTab(i, false);
  }


void ArchPos::tabIndex(int index) {
NoteTab* tab = tabs.get(index);

  if (tab) curPos += spaces(tab->pos - curPos);
  }
#endif

