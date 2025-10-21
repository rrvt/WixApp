// Entity Table Display Functions (Doc/View)


#include "pch.h"
#include "EntTbl.h"
#include "NotePad.h"
#include "Utilities.h"


void EntTbl::display() {
EntIter iter(*this);
EntRcd* rcd;

  setTabs();

  notePad << _T("Entity Table") << nCrlf;

  for (rcd = iter(); rcd; rcd = iter++) rcd->display();
  }


void EntRcd::display() {
  notePad << nTab << id;
  notePad << nTab << firstName;
  notePad << nTab << middleInitial;
  notePad << nTab << lastName;
  notePad << nTab << suffix;

  notePad << nTab << locationZip;
  notePad << nTab << phone1;
  notePad << nTab << phone2;

  notePad << nCrlf;
  }


static const int nTabs = 10;


void EntTbl::setTabs() {
EntIter iter(*this);
EntRcd* rcd;
int     max;
int     nFldsLn;
int     tabs[nTabs];
int     tab;
int     i;
int     n;

  for (max = 0, rcd = iter(); rcd; rcd = iter++) {
    maxLng(rcd->firstName,     max);
    maxLng(rcd->middleInitial, max);
    maxLng(rcd->lastName,      max);
    maxLng(rcd->suffix,        max);
    maxLng(rcd->locationZip,   max);
    maxLng(rcd->phone1,        max);
    maxLng(rcd->phone2,        max);
    }

  n = max ? 90 / max : 1;

  nFldsLn = n < 7 ? n : 7;

  for (i = 0; i < nTabs; i++) tabs[i] = 0;

  for (i = 0, rcd = iter(); rcd; i = 0, rcd = iter++) {
    maxLng(rcd->firstName,     tabs[i]);   i = (i + 1) % nFldsLn;
    maxLng(rcd->middleInitial, tabs[i]);   i = (i + 1) % nFldsLn;
    maxLng(rcd->lastName,      tabs[i]);   i = (i + 1) % nFldsLn;
    maxLng(rcd->suffix,        tabs[i]);   i = (i + 1) % nFldsLn;
    maxLng(rcd->locationZip,   tabs[i]);   i = (i + 1) % nFldsLn;
    maxLng(rcd->phone1,        tabs[i]);   i = (i + 1) % nFldsLn;
    maxLng(rcd->phone2,        tabs[i]);   i = (i + 1) % nFldsLn;
    }

  tab = 4;
  notePad << nClrTabs << nSetRTab(tab) << nSetTab(tab += 2);

  for (i = 0; i < nTabs && tabs[i]; i++) {
    tab += tabs[i] + 2;   notePad << nSetTab(tab);
    }
  }


