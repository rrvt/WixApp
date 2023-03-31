// Entity Table


#include "pch.h"
#include "EntTbl.h"
#include "NotePad.h"
#include "Utilities.h"


bool EntTbl::load(TCchar* path) {
EntSetIter iter(entSet);
EntSet*    set;
int        i;

  if (!open(path)) return false;

  data.clear();

  for (i = 0, set = iter(); set; i++, set = iter++) {

    EntRcd* rcd = data.allocate();   rcd->load(set);   data = rcd;

    if (rcd->id > maxID) maxID = rcd->id;
    }

  close();   return true;
  }


void EntRcd::load(EntSet* set) {
  id            = set->entityID;
  firstName     = set->firstName;
  middleInitial = set->middleInitial;
  lastName      = set->lastName;
  suffix        = set->suffix;
  addrID        = set->addrID;
  cityStID      = set->cityStID;
  addrIsPO      = set->addrIsPO;
  locationZip   = set->locationZip;
  eMail         = set->eMail;
  phone1        = set->phone1;
  phone2        = set->phone2;
  }


bool EntTbl::store(TCchar* path) {
EntIter    iter(*this);
EntRcd*    rcd;
EntSetIter setIter(entSet);
EntSet*    set;

  if (!open(path)) return false;

  for (rcd = iter(), set = setIter(); rcd; rcd = iter++) if (rcd->dirty) {

    set = setIter.find(rcd->id);   rcd->dirty = false;

    if (!set)        {rcd->add(entSet); continue;}

    if (rcd->remove) {set->remove(); iter.remove();  continue;}

    rcd->store(*set);
    }

  close();   return true;
  }


void EntRcd::store(EntSet& set) {set.edit();     copy(set);   set.update();}
void EntRcd::add(  EntSet& set) {set.addNew();   copy(set);   set.update();}


void EntRcd::copy(EntSet& set) {
  set.entityID      = id;
  set.firstName     = firstName;
  set.middleInitial = middleInitial;
  set.lastName      = lastName;
  set.suffix        = suffix;
  set.addrID        = addrID;
  set.cityStID      = cityStID;
  set.addrIsPO      = addrIsPO;
  set.locationZip   = locationZip;
  set.eMail         = eMail;
  set.phone1        = phone1;
  set.phone2        = phone2;
  }


EntRcd* EntTbl::add(EntRcd& rcd) {rcd.id = ++maxID;  rcd.dirty = true;  return data = rcd;}


EntRcd* EntTbl::find(TCchar* firstName, TCchar* lastName, TCchar* phone1) {
EntIter iter(*this);
EntRcd* rcd;

  for (rcd = iter(); rcd; rcd = iter++) if (rcd->contains(firstName, lastName, phone1)) return rcd;

  return 0;
  }


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


