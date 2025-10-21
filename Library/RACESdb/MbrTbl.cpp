// Member Table


#include "pch.h"
#include "MbrTbl.h"


MbrRcd::MbrRcd() : id(0), dirty(false), remove(false),
                   badgeNumber(0), mbrEntityID(0), emplEntityID(0),
                   iCE_EntityID(0), assgnPrefID(0), locationPrefID(0),
                   statusID(0), isOfficer(false), badgeOK(false) { }


void MbrRcd::clear() {
  id = 0;   dirty = false;   remove = false;
  badgeNumber = 0;   mbrEntityID = 0;   emplEntityID = 0;   iCE_EntityID = 0;
  assgnPrefID = 0;   locationPrefID = 0;   statusID = 0;   callSign.clear();
  fCCExpiration.clear();   startDate.clear();   dSWDate.clear();   badgeExpDate.clear();
  responder.clear();   secondaryEmail.clear();   textMsgPh1.clear();   textMsgPh2.clear();
  handHeld.clear();   portMobile.clear();   portPacket.clear();   otherEquip.clear();
  multilingual.clear();   otherCapabilities.clear();   limitations.clear();   comments.clear();
  shirtSize.clear();   isOfficer = false;   skillCertifications.clear();   eOC_Certifications.clear();
  updateDate.clear();   badgeOK = false;   image.clear();
  }


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


void MbrRcd::copy(MbrRcd& r) {
  id                  = r.id;
  dirty               = r.dirty;
  remove              = r.remove;
  badgeNumber         = r.badgeNumber;
  mbrEntityID         = r.mbrEntityID;
  emplEntityID        = r.emplEntityID;
  iCE_EntityID        = r.iCE_EntityID;
  assgnPrefID         = r.assgnPrefID;
  locationPrefID      = r.locationPrefID;
  statusID            = r.statusID;
  callSign            = r.callSign;
  fCCExpiration       = r.fCCExpiration;
  startDate           = r.startDate;
  dSWDate             = r.dSWDate;
  badgeExpDate        = r.badgeExpDate;
  responder           = r.responder;
  secondaryEmail      = r.secondaryEmail;
  textMsgPh1          = r.textMsgPh1;
  textMsgPh2          = r.textMsgPh2;
  handHeld            = r.handHeld;
  portMobile          = r.portMobile;
  portPacket          = r.portPacket;
  otherEquip          = r.otherEquip;
  multilingual        = r.multilingual;
  otherCapabilities   = r.otherCapabilities;
  limitations         = r.limitations;
  comments            = r.comments;
  shirtSize           = r.shirtSize;
  isOfficer           = r.isOfficer;
  skillCertifications = r.skillCertifications;
  eOC_Certifications  = r.eOC_Certifications;
  updateDate          = r.updateDate;
  badgeOK             = r.badgeOK;
  image               = r.image;
  }


MbrRcd* MbrTbl::add(MbrRcd& rcd) {rcd.id = ++maxID;  rcd.dirty = true;  return data = rcd;}


MbrRcd* MbrTbl::find(TCchar* callSign) {
MbrIter iter(*this);
MbrRcd* rcd;

  for (rcd = iter(); rcd; rcd = iter++) if (rcd->contains(callSign)) return rcd;

  return 0;
  }


