// Tabs needed during output to a device


#include "pch.h"
#include "DevTabs.h"
#include "Horiz.h"


// Insert an approximate character position for the next tab position

void DevTabs::setTab(Horiz& hz, int pos) {DevTab tab;   set(hz, pos, tab);}


void DevTabs::setRTab(Horiz& hz, int pos) {DevTab tab;   tab.right = true; set(hz, pos, tab);}


void DevTabs::set(Horiz& hz, int pos, DevTab& tab) {
double tabPos = pos;

  tab.pos = tabPos > 0 ? tabPos * hz.avgLgChWidth() + hz.leftBnd :
                                                                hz.rightBnd + hz.avgLgChWidth() * tabPos;
  data = tab;               // inserts in sorted order
  }


void DevTabs::findNextTab(Horiz& hz) {
double   pos       = hz.position;
double   lgChWidth = hz.avgLgChWidth();
double   left      = hz.leftBnd;
DvTbIter iter(*this);
DevTab*  tab;
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



DevTab::~DevTab() { }

