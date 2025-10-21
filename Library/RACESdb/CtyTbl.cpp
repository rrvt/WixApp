// CityState Table


#include "pch.h"
#include "CtyTbl.h"


CtyRcd::CtyRcd() : id(0), dirty(false), remove(false) { }


void CtyRcd::clear() {
  id = 0;   dirty = false;   remove = false;
  city.clear();   state.clear();   zip.clear();
  }


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


void CtyRcd::copy(CtyRcd& r) {
  id     = r.id;
  dirty  = r.dirty;
  remove = r.remove;
  city   = r.city;
  state  = r.state;
  zip    = r.zip;
  }


CtyRcd* CtyTbl::add(CtyRcd& rcd) {rcd.id = ++maxID;  rcd.dirty = true;  return data = rcd;}


CtyRcd* CtyTbl::find(TCchar* zip) {
CtyIter iter(*this);
CtyRcd* rcd;

  for (rcd = iter(); rcd; rcd = iter++) if (rcd->contains(zip)) return rcd;

  return 0;
  }


