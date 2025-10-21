// Status Table Display Functions (Doc/View)


#include "pch.h"
#include "StsTbl.h"
#include "NotePad.h"
#include "Utilities.h"


void StsTbl::display() {
StsIter iter(*this);
StsRcd* rcd;

  setTabs();

  notePad << _T("Status Table") << nCrlf;

  for (rcd = iter(); rcd; rcd = iter++) rcd->display();
  }


void StsRcd::display() {
  notePad << nTab << id;
  notePad << nTab << abbreviation;
  notePad << nCrlf;
  }


static const int nTabs = 10;


void StsTbl::setTabs() {
StsIter iter(*this);
StsRcd* rcd;
int     max;
int     nFldsLn;
int     tabs[nTabs];
int     tab;
int     i;
int     n;

  for (max = 0, rcd = iter(); rcd; rcd = iter++) {
    maxLng(rcd->abbreviation, max);
    }

  n = max ? 90 / max : 1;

  nFldsLn = n < 1 ? n : 1;

  for (i = 0; i < nTabs; i++) tabs[i] = 0;

  for (i = 0, rcd = iter(); rcd; i = 0, rcd = iter++) {
    maxLng(rcd->abbreviation, tabs[i]);   i = (i + 1) % nFldsLn;
    }

  tab = 4;
  notePad << nClrTabs << nSetRTab(tab) << nSetTab(tab += 2);

  for (i = 0; i < nTabs && tabs[i]; i++) {
    tab += tabs[i] + 2;   notePad << nSetTab(tab);
    }
  }


