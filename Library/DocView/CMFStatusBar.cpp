


#include "pch.h"
#include "CMFStatusBar.h"


static const int DefaultInd[] = {ID_INDICATOR_CAPS, ID_INDICATOR_NUM};
static const int NDefaultInd  = noElements(DefaultInd);
static const int DefaultPct   =  4;


CMFStatusBar::CMFStatusBar(int nUser) : CMFCStatusBar(), n(0), nFree(nUser) {
int smallPct = NDefaultInd * DefaultPct;
int bigPct   = (100 - smallPct) / (nUser + 1);
int i;

  for (i = 0, n = 0; i < nUser+1 && n < MaxStsBarInd; i++, n++) indicators[n] = ID_SEPARATOR;

  for (i = 0; i < NDefaultInd && n < MaxStsBarInd; i++, n++) indicators[n] = DefaultInd[i];

  for (i = 0; i < nFree + 1; i++) percents[i] = bigPct;
  for (     ; i < n; i++)         percents[i] = DefaultPct;
  }


bool CMFStatusBar::create(CWnd* mainFrame)
                                   {int rslt = Create((CWnd*)mainFrame);   setup();   return rslt;}


void CMFStatusBar::setup() {
CRect rect;
int   width;
int   i;

  GetClientRect(&rect);   width = rect.right-40;

  SetIndicators((uint*)indicators, n);

  for (i = 0; i < n; i++) SetPaneWidth(i, width * percents[i] / 100);

  for (i = 0; i < nFree; i++) SetPaneText(i+1, text[i]);
  }

