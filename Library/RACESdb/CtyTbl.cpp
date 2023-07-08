// CityState Table


#include "pch.h"
#include "CtyTbl.h"
#include "NotePad.h"
#include "Utilities.h"


bool CtyTbl::load(TCchar* path) {
CtySetIter iter(ctySet);
CtySet*    set;
int        i;

  if (!open(path)) return false;

  data.clear();

  for (i = 0, set = iter(); set; i++, set = iter++) {

    CtyRcd* rcd = data.allocate();   rcd->load(set);   data = rcd;

    if (rcd->id > maxID) maxID = rcd->id;
    }

  close();   return true;
  }


void CtyRcd::load(CtySet* set) {
  id    = set->cityStateID;
  city  = set->city;
  state = set->state;
  zip   = set->zip;
  }


bool CtyTbl::store(TCchar* path) {
CtyIter    iter(*this);
CtyRcd*    rcd;
CtySetIter setIter(ctySet);
CtySet*    set;

  if (!open(path)) return false;

  for (rcd = iter(), set = setIter(); rcd; rcd = iter++) if (rcd->dirty) {

    set = setIter.find(rcd->id);   rcd->dirty = false;

    if (!set)        {rcd->add(ctySet); continue;}

    if (rcd->remove) {set->remove(); iter.remove();  continue;}

    rcd->store(*set);
    }

  close();   return true;
  }


void CtyRcd::store(CtySet& set) {set.edit();     copy(set);   set.update();}
void CtyRcd::add(  CtySet& set) {set.addNew();   copy(set);   set.update();}


void CtyRcd::copy(CtySet& set) {
  set.cityStateID = id;
  set.city        = city;
  set.state       = state;
  set.zip         = zip;
  }


CtyRcd* CtyTbl::add(CtyRcd& rcd) {rcd.id = ++maxID;  rcd.dirty = true;  return data = rcd;}


CtyRcd* CtyTbl::find(TCchar* city, TCchar* state, TCchar* zip) {
CtyIter iter(*this);
CtyRcd* rcd;

  for (rcd = iter(); rcd; rcd = iter++) if (rcd->contains(city, state, zip)) return rcd;

  return 0;
  }


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


