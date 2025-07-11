// Member Table


#include "pch.h"
#include "MbrTbl.h"
#include "NotePad.h"
#include "Utilities.h"


bool MbrTbl::load(TCchar* path) {
MbrSetIter iter(mbrSet);
MbrSet*    set;
int        i;

  if (!open(path)) return false;

  data.clear();

  for (i = 0, set = iter(); set; i++, set = iter++) {

    MbrRcd* rcd = data.allocate();   rcd->load(set);   data = rcd;

    if (rcd->id > maxID) maxID = rcd->id;
    }

  close();   return true;
  }


void MbrRcd::load(MbrSet* set) {
  id                  = set->memberID;
  badgeNumber         = set->badgeNumber;
  mbrEntityID         = set->mbrEntityID;
  emplEntityID        = set->emplEntityID;
  iCE_EntityID        = set->iCE_EntityID;
  assgnPrefID         = set->assgnPrefID;
  locationPrefID      = set->locationPrefID;
  statusID            = set->statusID;
  callSign            = set->callSign;
  fCCExpiration       = set->fCCExpiration;
  startDate           = set->startDate;
  dSWDate             = set->dSWDate;
  badgeExpDate        = set->badgeExpDate;
  responder           = set->responder;
  secondaryEmail      = set->secondaryEmail;
  textMsgPh1          = set->textMsgPh1;
  textMsgPh2          = set->textMsgPh2;
  handHeld            = set->handHeld;
  portMobile          = set->portMobile;
  portPacket          = set->portPacket;
  otherEquip          = set->otherEquip;
  multilingual        = set->multilingual;
  otherCapabilities   = set->otherCapabilities;
  limitations         = set->limitations;
  comments            = set->comments;
  shirtSize           = set->shirtSize;
  isOfficer           = set->isOfficer;
  skillCertifications = set->skillCertifications;
  eOC_Certifications  = set->eOC_Certifications;
  updateDate          = set->updateDate;
  badgeOK             = set->badgeOK;
  image               = set->image;
  }


bool MbrTbl::store(TCchar* path) {
MbrIter    iter(*this);
MbrRcd*    rcd;
MbrSetIter setIter(mbrSet);
MbrSet*    set;

  if (!open(path)) return false;

  for (rcd = iter(), set = setIter(); rcd; rcd = iter++) if (rcd->dirty) {

    set = setIter.find(rcd->id);   rcd->dirty = false;

    if (!set)        {rcd->add(mbrSet); continue;}

    if (rcd->remove) {set->remove(); iter.remove();  continue;}

    rcd->store(*set);
    }

  close();   return true;
  }


void MbrRcd::store(MbrSet& set) {set.edit();     copy(set);   set.update();}
void MbrRcd::add(  MbrSet& set) {set.addNew();   copy(set);   set.update();}


void MbrRcd::copy(MbrSet& set) {
  set.memberID            = id;
  set.badgeNumber         = badgeNumber;
  set.mbrEntityID         = mbrEntityID;
  set.emplEntityID        = emplEntityID;
  set.iCE_EntityID        = iCE_EntityID;
  set.assgnPrefID         = assgnPrefID;
  set.locationPrefID      = locationPrefID;
  set.statusID            = statusID;
  set.callSign            = callSign;
  set.fCCExpiration       = fCCExpiration;
  set.startDate           = startDate;
  set.dSWDate             = dSWDate;
  set.badgeExpDate        = badgeExpDate;
  set.responder           = responder;
  set.secondaryEmail      = secondaryEmail;
  set.textMsgPh1          = textMsgPh1;
  set.textMsgPh2          = textMsgPh2;
  set.handHeld            = handHeld;
  set.portMobile          = portMobile;
  set.portPacket          = portPacket;
  set.otherEquip          = otherEquip;
  set.multilingual        = multilingual;
  set.otherCapabilities   = otherCapabilities;
  set.limitations         = limitations;
  set.comments            = comments;
  set.shirtSize           = shirtSize;
  set.isOfficer           = isOfficer;
  set.skillCertifications = skillCertifications;
  set.eOC_Certifications  = eOC_Certifications;
  set.updateDate          = updateDate;
  set.badgeOK             = badgeOK;
  set.image               = image;
  }


MbrRcd* MbrTbl::add(MbrRcd& rcd) {rcd.id = ++maxID;  rcd.dirty = true;  return data = rcd;}


MbrRcd* MbrTbl::find(TCchar* callSign) {
MbrIter iter(*this);
MbrRcd* rcd;

  for (rcd = iter(); rcd; rcd = iter++) if (rcd->contains(callSign)) return rcd;

  return 0;
  }


void MbrTbl::display() {
MbrIter iter(*this);
MbrRcd* rcd;

  setTabs();

  notePad << _T("Member Table") << nCrlf;

  for (rcd = iter(); rcd; rcd = iter++) rcd->display();
  }


void MbrRcd::display() {
  notePad << nTab << id;

  notePad << nTab << callSign;
  notePad << nTab << fCCExpiration;
  notePad << nTab << startDate;
  notePad << nTab << dSWDate;

  notePad << nTab << badgeExpDate;
  notePad << nTab << responder;
  notePad << nTab << textMsgPh1;
  notePad << nTab << textMsgPh2;


  notePad << nTab << shirtSize;
  notePad << nTab << updateDate;

  notePad << nTab << image;
  notePad << nCrlf;
  }


static const int nTabs = 10;


void MbrTbl::setTabs() {
MbrIter iter(*this);
MbrRcd* rcd;
int     max;
int     nFldsLn;
int     tabs[nTabs];
int     tab;
int     i;
int     n;

  for (max = 0, rcd = iter(); rcd; rcd = iter++) {
    maxLng(rcd->callSign,      max);
    maxLng(rcd->fCCExpiration, max);
    maxLng(rcd->startDate,     max);
    maxLng(rcd->dSWDate,       max);
    maxLng(rcd->badgeExpDate,  max);
    maxLng(rcd->responder,     max);
    maxLng(rcd->textMsgPh1,    max);
    maxLng(rcd->textMsgPh2,    max);
    maxLng(rcd->shirtSize,     max);
    maxLng(rcd->updateDate,    max);
    maxLng(rcd->image,         max);
    }

  n = max ? 90 / max : 1;

  nFldsLn = n < 11 ? n : 11;

  for (i = 0; i < nTabs; i++) tabs[i] = 0;

  for (i = 0, rcd = iter(); rcd; i = 0, rcd = iter++) {
    maxLng(rcd->callSign,      tabs[i]);   i = (i + 1) % nFldsLn;
    maxLng(rcd->fCCExpiration, tabs[i]);   i = (i + 1) % nFldsLn;
    maxLng(rcd->startDate,     tabs[i]);   i = (i + 1) % nFldsLn;
    maxLng(rcd->dSWDate,       tabs[i]);   i = (i + 1) % nFldsLn;
    maxLng(rcd->badgeExpDate,  tabs[i]);   i = (i + 1) % nFldsLn;
    maxLng(rcd->responder,     tabs[i]);   i = (i + 1) % nFldsLn;
    maxLng(rcd->textMsgPh1,    tabs[i]);   i = (i + 1) % nFldsLn;
    maxLng(rcd->textMsgPh2,    tabs[i]);   i = (i + 1) % nFldsLn;
    maxLng(rcd->shirtSize,     tabs[i]);   i = (i + 1) % nFldsLn;
    maxLng(rcd->updateDate,    tabs[i]);   i = (i + 1) % nFldsLn;
    maxLng(rcd->image,         tabs[i]);   i = (i + 1) % nFldsLn;
    }

  tab = 4;
  notePad << nClrTabs << nSetRTab(tab) << nSetTab(tab += 2);

  for (i = 0; i < nTabs && tabs[i]; i++) {
    tab += tabs[i] + 2;   notePad << nSetTab(tab);
    }
  }


