// Address Table Display Functions (Doc/View)


#include "pch.h"
#include "AdrTbl.h"
#include "NotePad.h"
#include "Utilities.h"


void AdrTbl::display() {
AdrIter iter(*this);
AdrRcd* rcd;

  setTabs();

  notePad << _T("Address Table") << nCrlf;

  for (rcd = iter(); rcd; rcd = iter++) rcd->display();
  }


void AdrRcd::display() {
  notePad << nTab << id;
  notePad << nTab << address1;
  notePad << nTab << address2;
  notePad << nCrlf;
  }


static const int nTabs = 10;


void AdrTbl::setTabs() {
AdrIter iter(*this);
AdrRcd* rcd;
int     max;
int     nFldsLn;
int     tabs[nTabs];
int     tab;
int     i;
int     n;

  for (max = 0, rcd = iter(); rcd; rcd = iter++) {
    maxLng(rcd->address1, max);
    maxLng(rcd->address2, max);
    }

  n = max ? 90 / max : 1;

  nFldsLn = n < 2 ? n : 2;

  for (i = 0; i < nTabs; i++) tabs[i] = 0;

  for (i = 0, rcd = iter(); rcd; i = 0, rcd = iter++) {
    maxLng(rcd->address1, tabs[i]);   i = (i + 1) % nFldsLn;
    maxLng(rcd->address2, tabs[i]);   i = (i + 1) % nFldsLn;
    }

  tab = 4;
  notePad << nClrTabs << nSetRTab(tab) << nSetTab(tab += 2);

  for (i = 0; i < nTabs && tabs[i]; i++) {
    tab += tabs[i] + 2;   notePad << nSetTab(tab);
    }
  }


