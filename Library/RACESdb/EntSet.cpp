// EntSet Record


#include "pch.h"
#include "EntSet.h"
#include "AccessDB.h"


EntSet::EntSet() : AccRcdSet(accessDB.db()), entityID(0), firstName(), middleInitial(),
          lastName(), suffix(),
          addrID(0),
          cityStID(0),
          addrIsPO(false),
          locationZip(),
          eMail(),
          phone1(),
          phone2() { }


bool EntSet::open(TCchar* path) {

  opened = false;

  if (!accessDB.isOpen() && !accessDB.open(path)) return false;

  SetState(CRecordset::dynaset, NULL, CRecordset::none);          // Cache state info and allocate hstmt

  if (!AllocHstmt()) return false;

  OnSetOptions(m_hstmt);   AllocStatusArrays();   m_nFields = 12;

  try {if (!Open(CRecordset::dynaset, _T("Entity"), CRecordset::none)) return false;}
  catch(...) {
    try {if (!Open(CRecordset::snapshot, _T("Entity"), CRecordset::none)) return false;}
    catch(...) {close(); return false;}
    }

  return opened = true;
  }


EntSet* EntSetIter::find(int id) {
EntSet* set = &rcd;

  for ( ; set && set->entityID <= id; set = (*this)++) if (set->entityID == id) return set;
  for ( ; set && set->entityID >= id; set = (*this)--) if (set->entityID == id) return set;

  return 0;
  }


bool EntSet::edit()
  {if (!opened) return false;   try {Edit(); return true;} catch(...) {return false;}}


bool EntSet::addNew()
  {if (!opened) return false;   try {AddNew(); return true;} catch(...) {return false;}}


bool EntSet::update()
  {if (!opened) return false;   try {Update(); movePrev(); return true;} catch(...) {return false;}}


bool EntSet::remove()
  {if (!opened) return false;   try {Delete(); movePrev(); return true;} catch(...) {return false;}}


void EntSet::DoFieldExchange(CFieldExchange* pFX) {

  pFX->SetFieldType(CFieldExchange::outputColumn);

  RFX_Int( pFX, _T("EntityID"),      entityID);
  RFX_Text(pFX, _T("FirstName"),     firstName);
  RFX_Text(pFX, _T("MiddleInitial"), middleInitial);
  RFX_Text(pFX, _T("LastName"),      lastName);
  RFX_Text(pFX, _T("Suffix"),        suffix);
  RFX_Int( pFX, _T("AddrID"),        addrID);
  RFX_Int( pFX, _T("CityStID"),      cityStID);
  RFX_Bool(pFX, _T("AddrIsPO"),      addrIsPO);
  RFX_Text(pFX, _T("LocationZip"),   locationZip);
  RFX_Text(pFX, _T("eMail"),         eMail, 1024);
  RFX_Text(pFX, _T("Phone1"),        phone1);
  RFX_Text(pFX, _T("Phone2"),        phone2);
  }


EntSet* EntSet::moveFirst() {
  if (!opened || (IsBOF() && IsEOF())) return 0;

  MoveFirst();  return this;
  }


EntSet* EntSet::moveNext() {
  if (!opened || IsEOF()) return 0;

  try {Move(1);} catch (...) {try {Move(1);} catch (...) {return 0;}}

  return IsEOF() ? 0 : this;
  }


EntSet* EntSet::moveLast() {
  if (!opened || (IsBOF() && IsEOF())) return 0;

  MoveLast();   return this;
  }


EntSet* EntSet::movePrev() {
  if (!opened || IsBOF()) return 0;

  try {Move(-1);} catch (...) {try {Move(-1);} catch (...) {return 0;}}

  return IsBOF() ? 0 : this;
  }
