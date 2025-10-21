// AssgnPref Table


#include "pch.h"
#include "AsnTbl.h"


AsnRcd::AsnRcd() : id(0), dirty(false), remove(false) { }


void AsnRcd::clear() {
  id = 0;   dirty = false;   remove = false;
  aPKey.clear();   txt.clear();
  }


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


void AsnRcd::copy(AsnRcd& r) {
  id     = r.id;
  dirty  = r.dirty;
  remove = r.remove;
  aPKey  = r.aPKey;
  txt    = r.txt;
  }


AsnRcd* AsnTbl::add(AsnRcd& rcd) {rcd.id = ++maxID;  rcd.dirty = true;  return data = rcd;}


AsnRcd* AsnTbl::find(TCchar* aPKey) {
AsnIter iter(*this);
AsnRcd* rcd;

  for (rcd = iter(); rcd; rcd = iter++) if (rcd->contains(aPKey)) return rcd;

  return 0;
  }


