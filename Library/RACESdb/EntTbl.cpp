// Entity Table


#include "pch.h"
#include "EntTbl.h"


EntRcd::EntRcd() : id(0), dirty(false), remove(false), addrID(0), cityStID(0),
                   addrIsPO(false) { }


void EntRcd::clear() {
  id = 0;   dirty = false;   remove = false;
  firstName.clear();   middleInitial.clear();   lastName.clear();   suffix.clear();
  addrID = 0;   cityStID = 0;   addrIsPO = false;   locationZip.clear();
  eMail.clear();   phone1.clear();   phone2.clear();
  }


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


void EntRcd::copy(EntRcd& r) {
  id            = r.id;
  dirty         = r.dirty;
  remove        = r.remove;
  firstName     = r.firstName;
  middleInitial = r.middleInitial;
  lastName      = r.lastName;
  suffix        = r.suffix;
  addrID        = r.addrID;
  cityStID      = r.cityStID;
  addrIsPO      = r.addrIsPO;
  locationZip   = r.locationZip;
  eMail         = r.eMail;
  phone1        = r.phone1;
  phone2        = r.phone2;
  }


EntRcd* EntTbl::add(EntRcd& rcd) {rcd.id = ++maxID;  rcd.dirty = true;  return data = rcd;}


EntRcd* EntTbl::find(TCchar* firstName, TCchar* lastName, TCchar* phone2) {
EntIter iter(*this);
EntRcd* rcd;

  for (rcd = iter(); rcd; rcd = iter++) if (rcd->contains(firstName, lastName, phone2)) return rcd;

  return 0;
  }


