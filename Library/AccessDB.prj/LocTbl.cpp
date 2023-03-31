// LocationPref Table


#include "pch.h"
#include "LocTbl.h"
#include "NotePad.h"
#include "Utilities.h"


bool LocTbl::load(TCchar* path) {
LocSetIter iter(locSet);
LocSet*    set;
int        i;

  if (!open(path)) return false;

  data.clear();

  for (i = 0, set = iter(); set; i++, set = iter++) {

    LocRcd* rcd = data.allocate();   rcd->load(set);   data = rcd;

    if (rcd->id > maxID) maxID = rcd->id;
    }

  close();   return true;
  }


void LocRcd::load(LocSet* set) {
  id  = set->locationPrefID;
  key = set->key;
  txt = set->txt;
  }


bool LocTbl::store(TCchar* path) {
LocIter    iter(*this);
LocRcd*    rcd;
LocSetIter setIter(locSet);
LocSet*    set;

  if (!open(path)) return false;

  for (rcd = iter(), set = setIter(); rcd; rcd = iter++) if (rcd->dirty) {

    set = setIter.find(rcd->id);   rcd->dirty = false;

    if (!set)        {rcd->add(locSet); continue;}

    if (rcd->remove) {set->remove(); iter.remove();  continue;}

    rcd->store(*set);
    }

  close();   return true;
  }


void LocRcd::store(LocSet& set) {set.edit();     copy(set);   set.update();}
void LocRcd::add(  LocSet& set) {set.addNew();   copy(set);   set.update();}


void LocRcd::copy(LocSet& set) {
  set.locationPrefID = id;
  set.key            = key;
  set.txt            = txt;
  }


LocRcd* LocTbl::add(LocRcd& rcd) {rcd.id = ++maxID;  rcd.dirty = true;  return data = rcd;}


LocRcd* LocTbl::find(TCchar* key) {
LocIter iter(*this);
LocRcd* rcd;

  for (rcd = iter(); rcd; rcd = iter++) if (rcd->contains(key)) return rcd;

  return 0;
  }


void LocTbl::display() {
LocIter iter(*this);
LocRcd* rcd;

  setTabs();

  notePad << _T("LocationPref Table") << nCrlf;

  for (rcd = iter(); rcd; rcd = iter++) rcd->display();
  }


void LocRcd::display() {
  notePad << nTab << id;
  notePad << nTab << key;
  notePad << nTab << txt;
  notePad << nCrlf;
  }


static const int nTabs = 10;


void LocTbl::setTabs() {
LocIter iter(*this);
LocRcd* rcd;
int     max;
int     nFldsLn;
int     tabs[nTabs];
int     tab;
int     i;
int     n;

  for (max = 0, rcd = iter(); rcd; rcd = iter++) {
    maxLng(rcd->key, max);
    maxLng(rcd->txt, max);
    }

  n = max ? 90 / max : 1;

  nFldsLn = n < 2 ? n : 2;

  for (i = 0; i < nTabs; i++) tabs[i] = 0;

  for (i = 0, rcd = iter(); rcd; i = 0, rcd = iter++) {
    maxLng(rcd->key, tabs[i]);   i = (i + 1) % nFldsLn;
    maxLng(rcd->txt, tabs[i]);   i = (i + 1) % nFldsLn;
    }

  tab = 4;
  notePad << nClrTabs << nSetRTab(tab) << nSetTab(tab += 2);

  for (i = 0; i < nTabs && tabs[i]; i++) {
    tab += tabs[i] + 2;   notePad << nSetTab(tab);
    }
  }


