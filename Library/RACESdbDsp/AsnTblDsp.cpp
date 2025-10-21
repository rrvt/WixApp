// AssgnPref Table Display Functions (Doc/View)


#include "pch.h"
#include "AsnTbl.h"
#include "NotePad.h"
#include "Utilities.h"


void AsnTbl::display() {
AsnIter iter(*this);
AsnRcd* rcd;

  setTabs();

  notePad << _T("AssgnPref Table") << nCrlf;

  for (rcd = iter(); rcd; rcd = iter++) rcd->display();
  }


void AsnRcd::display() {
  notePad << nTab << id;
  notePad << nTab << aPKey;
  notePad << nTab << txt;
  notePad << nCrlf;
  }


static const int nTabs = 10;


void AsnTbl::setTabs() {
AsnIter iter(*this);
AsnRcd* rcd;
int     max;
int     nFldsLn;
int     tabs[nTabs];
int     tab;
int     i;
int     n;

  for (max = 0, rcd = iter(); rcd; rcd = iter++) {
    maxLng(rcd->aPKey, max);
    maxLng(rcd->txt,   max);
    }

  n = max ? 90 / max : 1;

  nFldsLn = n < 2 ? n : 2;

  for (i = 0; i < nTabs; i++) tabs[i] = 0;

  for (i = 0, rcd = iter(); rcd; i = 0, rcd = iter++) {
    maxLng(rcd->aPKey, tabs[i]);   i = (i + 1) % nFldsLn;
    maxLng(rcd->txt,   tabs[i]);   i = (i + 1) % nFldsLn;
    }

  tab = 4;
  notePad << nClrTabs << nSetRTab(tab) << nSetTab(tab += 2);

  for (i = 0; i < nTabs && tabs[i]; i++) {
    tab += tabs[i] + 2;   notePad << nSetTab(tab);
    }
  }


