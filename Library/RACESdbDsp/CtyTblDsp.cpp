// CityState Table Display Functions (Doc/View)


#include "pch.h"
#include "CtyTbl.h"
#include "NotePad.h"
#include "Utilities.h"


void CtyTbl::display() {
CtyIter iter(*this);
CtyRcd* rcd;

  setTabs();

  notePad << _T("CityState Table") << nCrlf;

  for (rcd = iter(); rcd; rcd = iter++) rcd->display();
  }


void CtyRcd::display() {
  notePad << nTab << id;
  notePad << nTab << city;
  notePad << nTab << state;
  notePad << nTab << zip;
  notePad << nCrlf;
  }


static const int nTabs = 10;


void CtyTbl::setTabs() {
CtyIter iter(*this);
CtyRcd* rcd;
int     max;
int     nFldsLn;
int     tabs[nTabs];
int     tab;
int     i;
int     n;

  for (max = 0, rcd = iter(); rcd; rcd = iter++) {
    maxLng(rcd->city,  max);
    maxLng(rcd->state, max);
    maxLng(rcd->zip,   max);
    }

  n = max ? 90 / max : 1;

  nFldsLn = n < 3 ? n : 3;

  for (i = 0; i < nTabs; i++) tabs[i] = 0;

  for (i = 0, rcd = iter(); rcd; i = 0, rcd = iter++) {
    maxLng(rcd->city,  tabs[i]);   i = (i + 1) % nFldsLn;
    maxLng(rcd->state, tabs[i]);   i = (i + 1) % nFldsLn;
    maxLng(rcd->zip,   tabs[i]);   i = (i + 1) % nFldsLn;
    }

  tab = 4;
  notePad << nClrTabs << nSetRTab(tab) << nSetTab(tab += 2);

  for (i = 0; i < nTabs && tabs[i]; i++) {
    tab += tabs[i] + 2;   notePad << nSetTab(tab);
    }
  }


