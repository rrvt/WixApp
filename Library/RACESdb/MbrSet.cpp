// MbrSet Record


#include "pch.h"
#include "MbrSet.h"
#include "AccessDB.h"


MbrSet::MbrSet() : AccRcdSet(accessDB.db()), memberID(0), badgeNumber(0), mbrEntityID(0),
          emplEntityID(0), iCE_EntityID(0),
          assgnPrefID(0),
          locationPrefID(0),
          statusID(0),
          callSign(),
          fCCExpiration(),
          startDate(),
          dSWDate(),
          badgeExpDate(),
          responder(),
          secondaryEmail(),
          textMsgPh1(),
          textMsgPh2(),
          handHeld(),
          portMobile(),
          portPacket(),
          otherEquip(),
          multilingual(),
          otherCapabilities(),
          limitations(),
          comments(),
          shirtSize(),
          isOfficer(false),
          skillCertifications(),
          eOC_Certifications(),
          updateDate(),
          badgeOK(false),
          image() { }


bool MbrSet::open(TCchar* path) {

  opened = false;

  if (!accessDB.isOpen() && !accessDB.open(path)) return false;

  SetState(CRecordset::dynaset, NULL, CRecordset::none);          // Cache state info and allocate hstmt

  if (!AllocHstmt()) return false;

  OnSetOptions(m_hstmt);   AllocStatusArrays();   m_nFields = 32;

  try {if (!Open(CRecordset::dynaset, _T("Member"), CRecordset::none)) return false;}
  catch(...) {
    try {if (!Open(CRecordset::snapshot, _T("Member"), CRecordset::none)) return false;}
    catch(...) {close(); return false;}
    }

  return opened = true;
  }


MbrSet* MbrSetIter::find(int id) {
MbrSet* set = &rcd;

  for ( ; set && set->memberID <= id; set = (*this)++) if (set->memberID == id) return set;
  for ( ; set && set->memberID >= id; set = (*this)--) if (set->memberID == id) return set;

  return 0;
  }


bool MbrSet::edit()
  {if (!opened) return false;   try {Edit(); return true;} catch(...) {return false;}}


bool MbrSet::addNew()
  {if (!opened) return false;   try {AddNew(); return true;} catch(...) {return false;}}


bool MbrSet::update()
  {if (!opened) return false;   try {Update(); movePrev(); return true;} catch(...) {return false;}}


bool MbrSet::remove()
  {if (!opened) return false;   try {Delete(); movePrev(); return true;} catch(...) {return false;}}


void MbrSet::DoFieldExchange(CFieldExchange* pFX) {

  pFX->SetFieldType(CFieldExchange::outputColumn);

  RFX_Int( pFX, _T("MemberID"),            memberID);
  RFX_Int( pFX, _T("BadgeNumber"),         badgeNumber);
  RFX_Int( pFX, _T("MbrEntityID"),         mbrEntityID);
  RFX_Int( pFX, _T("EmplEntityID"),        emplEntityID);
  RFX_Int( pFX, _T("ICE_EntityID"),        iCE_EntityID);
  RFX_Int( pFX, _T("AssgnPrefID"),         assgnPrefID);
  RFX_Int( pFX, _T("LocationPrefID"),      locationPrefID);
  RFX_Int( pFX, _T("StatusID"),            statusID);
  RFX_Text(pFX, _T("CallSign"),            callSign);
  RFX_Text(pFX, _T("FCCExpiration"),       fCCExpiration);
  RFX_Text(pFX, _T("StartDate"),           startDate);
  RFX_Text(pFX, _T("DSWDate"),             dSWDate);
  RFX_Text(pFX, _T("BadgeExpDate"),        badgeExpDate);
  RFX_Text(pFX, _T("Responder"),           responder);
  RFX_Text(pFX, _T("SecondaryEmail"),      secondaryEmail, 1024);
  RFX_Text(pFX, _T("TextMsgPh1"),          textMsgPh1);
  RFX_Text(pFX, _T("TextMsgPh2"),          textMsgPh2);
  RFX_Text(pFX, _T("HandHeld"),            handHeld, 1024);
  RFX_Text(pFX, _T("PortMobile"),          portMobile, 1024);
  RFX_Text(pFX, _T("PortPacket"),          portPacket, 1024);
  RFX_Text(pFX, _T("OtherEquip"),          otherEquip, 1024);
  RFX_Text(pFX, _T("Multilingual"),        multilingual, 1024);
  RFX_Text(pFX, _T("OtherCapabilities"),   otherCapabilities, 1024);
  RFX_Text(pFX, _T("Limitations"),         limitations, 1024);
  RFX_Text(pFX, _T("Comments"),            comments, 1024);
  RFX_Text(pFX, _T("ShirtSize"),           shirtSize);
  RFX_Bool(pFX, _T("IsOfficer"),           isOfficer);
  RFX_Text(pFX, _T("SkillCertifications"), skillCertifications, 1024);
  RFX_Text(pFX, _T("EOC_Certifications"),  eOC_Certifications, 1024);
  RFX_Text(pFX, _T("UpdateDate"),          updateDate);
  RFX_Bool(pFX, _T("BadgeOK"),             badgeOK);
  RFX_Text(pFX, _T("Image"),               image);
  }


MbrSet* MbrSet::moveFirst() {
  if (!opened || (IsBOF() && IsEOF())) return 0;

  MoveFirst();  return this;
  }


MbrSet* MbrSet::moveNext() {
  if (!opened || IsEOF()) return 0;

  try {Move(1);} catch (...) {try {Move(1);} catch (...) {return 0;}}

  return IsEOF() ? 0 : this;
  }


MbrSet* MbrSet::moveLast() {
  if (!opened || (IsBOF() && IsEOF())) return 0;

  MoveLast();   return this;
  }


MbrSet* MbrSet::movePrev() {
  if (!opened || IsBOF()) return 0;

  try {Move(-1);} catch (...) {try {Move(-1);} catch (...) {return 0;}}

  return IsBOF() ? 0 : this;
  }
