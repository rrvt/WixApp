// Tabs needed during output to a device


#include "pch.h"
#include "DevTabs.h"
#include "Horiz.h"


DevTabs::DevTabs() : Tabs(1) { }


void DevTabs::setTab( Horiz& hz, NoteTab& tab) {setLineWidth(hz);   set(tab.pos, tab.right);}


RealTab DevTabs::findNextTab(Horiz& hz) {
double  dbl = hz.currentPos() / getCharWidth(hz);
int     curPos = int(dbl + 0.5);
NoteTab tab = find(curPos);
RealTab curTab;

  curTab.pos   = tab.pos * getCharWidth(hz) + hz.leftBnd;
  curTab.right = tab.right;

  return curTab;
  }


void     DevTabs::setLineWidth(Horiz& hz) {setWidth(hz.widthInCh());}
double   DevTabs::getCharWidth(Horiz& hz) {return hz.charWidth;}


////----------------------------

#if 1
#else

void DevTabs::findNextTab(Horiz& hz) {
double   pos       = hz.position;
double   lgChWidth = hz.avgLgChWidth();
double   left      = hz.leftBnd;
DvTbIter iter(*this);
NoteTab*  tab;
int      n;
double   newCurPos;

  for (tab = iter(); tab; tab = iter++)
    if (tab->pos - pos >= lgChWidth) {cur = tab; return;}

  // Not found, so go to then next 5th position

  n = int((pos - left + lgChWidth - 1) / lgChWidth);  n = (n + 4) / 5;

  newCurPos = n * 5 * lgChWidth + left;

  if (newCurPos - pos < lgChWidth) newCurPos += 5 * lgChWidth;

  dfltTab.pos = newCurPos;   dfltTab.right = false;   cur = &dfltTab;
  }


NoteTab::~NoteTab() { }
#endif
#if 1
#else
// Insert an approximate character position for the next tab position

void DevTabs::setTab(Horiz& hz, int pos) {NoteTab tab;   set(hz, pos, tab);}


void DevTabs::setRTab(Horiz& hz, int pos) {NoteTab tab;   tab.right = true; set(hz, pos, tab);}


void DevTabs::set(Horiz& hz, int pos, NoteTab& tab) {
double tabPos = pos;

  tab.pos = tabPos > 0 ? tabPos * hz.avgLgChWidth() + hz.leftBnd :
                                                          hz.rightBnd + hz.avgLgChWidth() * tabPos;
  data = tab;               // inserts in sorted order
  }
#endif
// , cur(&dfltTab)
#if 1
#else
  setWidth(int((hz.rightBnd - hz.leftBnd) / getCharWidth(hz) + 0.5));
#endif
#if 1
#else
  DeviceContext& dvx = hz.dvx;   return (dvx.avgLgChWidth + dvx.avgFlChWidth) / 2;
#endif
#if 0


  void DevTabs::setLineWidth(Horiz& hz) {
    setWidth(hz.widthInCh());
    }


  double DevTabs::getCharWidth(Horiz& hz) {
    return hz.charWidth();
    }


//void DevTabs::setTab(Horiz& hz, int pos)  {setLineWidth(hz);   set(pos, false);}
//void DevTabs::setRTab(Horiz& hz, int pos) {setLineWidth(hz);   set(pos, true);}
#endif
