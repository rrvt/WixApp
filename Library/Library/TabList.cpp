// List of tab in any medium


#include "pch.h"
#include "TabList.h"
#include "CDCex.h"


void TabList::set(int pos, bool right = false) {
Tab*       tab = data.allocate();

  tab->set(pos, right);   data = tab;   getAvgCharWidth();
  }


// Find next tab after curPos

int TabList::find(int curPos) {
TLIter iter(*this);
Tab*   tab;
int    pos;
int    avgWidth = getAvgCharWidth();
int    n;
static Tab dflt;

  if (!avgWidth) avgWidth = 20;

  pos = (curPos + avgWidth -1) / avgWidth;

  for (tab = iter(); tab; tab = iter++)
    if (*tab > pos) return tab->val * avgWidth;

  n = (pos + 6) / 5;   n = n * 5;   return n * avgWidth;
  }


int TabList::getAvgCharWidth() {
TEXTMETRIC metrics;

  if (!avgCharWidth && cdcEx.getMetrics(metrics)) avgCharWidth = metrics.tmAveCharWidth;

  return avgCharWidth;
  }

