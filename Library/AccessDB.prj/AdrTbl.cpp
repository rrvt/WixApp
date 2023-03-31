// Address Table


#include "pch.h"
#include "AdrTbl.h"
#include "NotePad.h"
#include "Utilities.h"


bool AdrTbl::load(TCchar* path) {
AdrSetIter iter(adrSet);
AdrSet*    set;
int        i;

  if (!open(path)) return false;

  data.clear();

  for (i = 0, set = iter(); set; i++, set = iter++) {

    AdrRcd* rcd = data.allocate();   rcd->load(set);   data = rcd;

    if (rcd->id > maxID) maxID = rcd->id;
    }

  close();   return true;
  }


void AdrRcd::load(AdrSet* set) {
  id       = set->addressID;
  address1 = set->address1;
  address2 = set->address2;
  }


bool AdrTbl::store(TCchar* path) {
AdrIter    iter(*this);
AdrRcd*    rcd;
AdrSetIter setIter(adrSet);
AdrSet*    set;

  if (!open(path)) return false;

  for (rcd = iter(), set = setIter(); rcd; rcd = iter++) if (rcd->dirty) {

    set = setIter.find(rcd->id);   rcd->dirty = false;

    if (!set)        {rcd->add(adrSet); continue;}

    if (rcd->remove) {set->remove(); iter.remove();  continue;}

    rcd->store(*set);
    }

  close();   return true;
  }


void AdrRcd::store(AdrSet& set) {set.edit();     copy(set);   set.update();}
void AdrRcd::add(  AdrSet& set) {set.addNew();   copy(set);   set.update();}


void AdrRcd::copy(AdrSet& set) {
  set.addressID = id;
  set.address1  = address1;
  set.address2  = address2;
  }


AdrRcd* AdrTbl::add(AdrRcd& rcd) {rcd.id = ++maxID;  rcd.dirty = true;  return data = rcd;}


AdrRcd* AdrTbl::find(TCchar* address1, TCchar* address2) {
AdrIter iter(*this);
AdrRcd* rcd;

  for (rcd = iter(); rcd; rcd = iter++) if (rcd->contains(address1, address2)) return rcd;

  return 0;
  }


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


