// Member Table Display Functions (Doc/View)


#include "pch.h"
#include "MbrTbl.h"
#include "NotePad.h"
#include "Utilities.h"


void MbrTbl::display() {
MbrIter iter(*this);
MbrRcd* rcd;

  setTabs();

  notePad << _T("Member Table") << nCrlf;

  for (rcd = iter(); rcd; rcd = iter++) rcd->display();
  }


void MbrRcd::display() {
  notePad << nTab << id;

  notePad << nTab << callSign;
  notePad << nTab << fCCExpiration;
  notePad << nTab << startDate;
  notePad << nTab << dSWDate;

  notePad << nTab << badgeExpDate;
  notePad << nTab << responder;
  notePad << nTab << textMsgPh1;
  notePad << nTab << textMsgPh2;


  notePad << nTab << shirtSize;
  notePad << nTab << updateDate;

  notePad << nTab << image;
  notePad << nCrlf;
  }


static const int nTabs = 10;


void MbrTbl::setTabs() {
MbrIter iter(*this);
MbrRcd* rcd;
int     max;
int     nFldsLn;
int     tabs[nTabs];
int     tab;
int     i;
int     n;

  for (max = 0, rcd = iter(); rcd; rcd = iter++) {
    maxLng(rcd->callSign,      max);
    maxLng(rcd->fCCExpiration, max);
    maxLng(rcd->startDate,     max);
    maxLng(rcd->dSWDate,       max);
    maxLng(rcd->badgeExpDate,  max);
    maxLng(rcd->responder,     max);
    maxLng(rcd->textMsgPh1,    max);
    maxLng(rcd->textMsgPh2,    max);
    maxLng(rcd->shirtSize,     max);
    maxLng(rcd->updateDate,    max);
    maxLng(rcd->image,         max);
    }

  n = max ? 90 / max : 1;

  nFldsLn = n < 11 ? n : 11;

  for (i = 0; i < nTabs; i++) tabs[i] = 0;

  for (i = 0, rcd = iter(); rcd; i = 0, rcd = iter++) {
    maxLng(rcd->callSign,      tabs[i]);   i = (i + 1) % nFldsLn;
    maxLng(rcd->fCCExpiration, tabs[i]);   i = (i + 1) % nFldsLn;
    maxLng(rcd->startDate,     tabs[i]);   i = (i + 1) % nFldsLn;
    maxLng(rcd->dSWDate,       tabs[i]);   i = (i + 1) % nFldsLn;
    maxLng(rcd->badgeExpDate,  tabs[i]);   i = (i + 1) % nFldsLn;
    maxLng(rcd->responder,     tabs[i]);   i = (i + 1) % nFldsLn;
    maxLng(rcd->textMsgPh1,    tabs[i]);   i = (i + 1) % nFldsLn;
    maxLng(rcd->textMsgPh2,    tabs[i]);   i = (i + 1) % nFldsLn;
    maxLng(rcd->shirtSize,     tabs[i]);   i = (i + 1) % nFldsLn;
    maxLng(rcd->updateDate,    tabs[i]);   i = (i + 1) % nFldsLn;
    maxLng(rcd->image,         tabs[i]);   i = (i + 1) % nFldsLn;
    }

  tab = 4;
  notePad << nClrTabs << nSetRTab(tab) << nSetTab(tab += 2);

  for (i = 0; i < nTabs && tabs[i]; i++) {
    tab += tabs[i] + 2;   notePad << nSetTab(tab);
    }
  }


