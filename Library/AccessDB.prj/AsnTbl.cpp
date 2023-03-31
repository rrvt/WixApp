// AssgnPref Table


#include "pch.h"
#include "AsnTbl.h"
#include "NotePad.h"
#include "Utilities.h"


bool AsnTbl::load(TCchar* path) {
AsnSetIter iter(asnSet);
AsnSet*    set;
int        i;

  if (!open(path)) return false;

  data.clear();

  for (i = 0, set = iter(); set; i++, set = iter++) {

    AsnRcd* rcd = data.allocate();   rcd->load(set);   data = rcd;

    if (rcd->id > maxID) maxID = rcd->id;
    }

  close();   return true;
  }


void AsnRcd::load(AsnSet* set) {
  id    = set->assgnPrefID;
  aPKey = set->aPKey;
  txt   = set->txt;
  }


bool AsnTbl::store(TCchar* path) {
AsnIter    iter(*this);
AsnRcd*    rcd;
AsnSetIter setIter(asnSet);
AsnSet*    set;

  if (!open(path)) return false;

  for (rcd = iter(), set = setIter(); rcd; rcd = iter++) if (rcd->dirty) {

    set = setIter.find(rcd->id);   rcd->dirty = false;

    if (!set)        {rcd->add(asnSet); continue;}

    if (rcd->remove) {set->remove(); iter.remove();  continue;}

    rcd->store(*set);
    }

  close();   return true;
  }


void AsnRcd::store(AsnSet& set) {set.edit();     copy(set);   set.update();}
void AsnRcd::add(  AsnSet& set) {set.addNew();   copy(set);   set.update();}


void AsnRcd::copy(AsnSet& set) {
  set.assgnPrefID = id;
  set.aPKey       = aPKey;
  set.txt         = txt;
  }


AsnRcd* AsnTbl::add(AsnRcd& rcd) {rcd.id = ++maxID;  rcd.dirty = true;  return data = rcd;}


AsnRcd* AsnTbl::find(TCchar* aPKey) {
AsnIter iter(*this);
AsnRcd* rcd;

  for (rcd = iter(); rcd; rcd = iter++) if (rcd->contains(aPKey)) return rcd;

  return 0;
  }


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


