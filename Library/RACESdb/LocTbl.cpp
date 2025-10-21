// LocationPref Table


#include "pch.h"
#include "LocTbl.h"


LocRcd::LocRcd() : id(0), dirty(false), remove(false) { }


void LocRcd::clear() {
  id = 0;   dirty = false;   remove = false;
  key.clear();   txt.clear();
  }


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


void LocRcd::copy(LocRcd& r) {
  id     = r.id;
  dirty  = r.dirty;
  remove = r.remove;
  key    = r.key;
  txt    = r.txt;
  }


LocRcd* LocTbl::add(LocRcd& rcd) {rcd.id = ++maxID;  rcd.dirty = true;  return data = rcd;}


LocRcd* LocTbl::find(TCchar* key) {
LocIter iter(*this);
LocRcd* rcd;

  for (rcd = iter(); rcd; rcd = iter++) if (rcd->contains(key)) return rcd;

  return 0;
  }


