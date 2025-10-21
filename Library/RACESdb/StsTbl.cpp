// Status Table


#include "pch.h"
#include "StsTbl.h"


StsRcd::StsRcd() : id(0), dirty(false), remove(false) { }


void StsRcd::clear() {
  id = 0;   dirty = false;   remove = false;
  abbreviation.clear();   description.clear();
  }


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


void StsRcd::copy(StsRcd& r) {
  id           = r.id;
  dirty        = r.dirty;
  remove       = r.remove;
  abbreviation = r.abbreviation;
  description  = r.description;
  }


StsRcd* StsTbl::add(StsRcd& rcd) {rcd.id = ++maxID;  rcd.dirty = true;  return data = rcd;}


StsRcd* StsTbl::find(TCchar* abbreviation) {
StsIter iter(*this);
StsRcd* rcd;

  for (rcd = iter(); rcd; rcd = iter++) if (rcd->contains(abbreviation)) return rcd;

  return 0;
  }


