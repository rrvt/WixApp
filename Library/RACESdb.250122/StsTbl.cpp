// Status Table


#include "pch.h"
#include "StsTbl.h"
#include "NotePad.h"
#include "Utilities.h"


bool StsTbl::load(TCchar* path) {
StsSetIter iter(stsSet);
StsSet*    set;
int        i;

  if (!open(path)) return false;

  data.clear();

  for (i = 0, set = iter(); set; i++, set = iter++) {

    StsRcd* rcd = data.allocate();   rcd->load(set);   data = rcd;

    if (rcd->id > maxID) maxID = rcd->id;
    }

  close();   return true;
  }


void StsRcd::load(StsSet* set) {
  id           = set->stsID;
  abbreviation = set->abbreviation;
  description  = set->description;
  }


bool StsTbl::store(TCchar* path) {
StsIter    iter(*this);
StsRcd*    rcd;
StsSetIter setIter(stsSet);
StsSet*    set;

  if (!open(path)) return false;

  for (rcd = iter(), set = setIter(); rcd; rcd = iter++) if (rcd->dirty) {

    set = setIter.find(rcd->id);   rcd->dirty = false;

    if (!set)        {rcd->add(stsSet); continue;}

    if (rcd->remove) {set->remove(); iter.remove();  continue;}

    rcd->store(*set);
    }

  close();   return true;
  }


void StsRcd::store(StsSet& set) {set.edit();     copy(set);   set.update();}
void StsRcd::add(  StsSet& set) {set.addNew();   copy(set);   set.update();}


void StsRcd::copy(StsSet& set) {
  set.stsID        = id;
  set.abbreviation = abbreviation;
  set.description  = description;
  }


StsRcd* StsTbl::add(StsRcd& rcd) {rcd.id = ++maxID;  rcd.dirty = true;  return data = rcd;}


StsRcd* StsTbl::find(TCchar* abbreviation) {
StsIter iter(*this);
StsRcd* rcd;

  for (rcd = iter(); rcd; rcd = iter++) if (rcd->contains(abbreviation)) return rcd;

  return 0;
  }


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


