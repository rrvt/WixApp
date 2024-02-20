// LocSet Record


#include "pch.h"
#include "LocSet.h"
#include "AccessDB.h"


LocSet::LocSet() : AccRcdSet(accessDB.db()), locationPrefID(0), key(), txt() { }


bool LocSet::open(TCchar* path) {

  opened = false;

  if (!accessDB.isOpen() && !accessDB.open(path)) return false;

  SetState(CRecordset::dynaset, NULL, CRecordset::none);          // Cache state info and allocate hstmt

  if (!AllocHstmt()) return false;

  OnSetOptions(m_hstmt);   AllocStatusArrays();   m_nFields = 3;

  try {if (!Open(CRecordset::dynaset, _T("LocationPref"), CRecordset::none)) return false;}
  catch(...) {
    try {if (!Open(CRecordset::snapshot, _T("LocationPref"), CRecordset::none)) return false;}
    catch(...) {close(); return false;}
    }

  return opened = true;
  }


LocSet* LocSetIter::find(int id) {
LocSet* set = &rcd;

  for ( ; set && set->locationPrefID <= id; set = (*this)++) if (set->locationPrefID == id) return set;
  for ( ; set && set->locationPrefID >= id; set = (*this)--) if (set->locationPrefID == id) return set;

  return 0;
  }


bool LocSet::edit()
  {if (!opened) return false;   try {Edit(); return true;} catch(...) {return false;}}


bool LocSet::addNew()
  {if (!opened) return false;   try {AddNew(); return true;} catch(...) {return false;}}


bool LocSet::update()
  {if (!opened) return false;   try {Update(); movePrev(); return true;} catch(...) {return false;}}


bool LocSet::remove()
  {if (!opened) return false;   try {Delete(); movePrev(); return true;} catch(...) {return false;}}


void LocSet::DoFieldExchange(CFieldExchange* pFX) {

  pFX->SetFieldType(CFieldExchange::outputColumn);

  RFX_Int( pFX, _T("LocationPrefID"), locationPrefID);
  RFX_Text(pFX, _T("Key"),            key);
  RFX_Text(pFX, _T("Txt"),            txt);
  }


LocSet* LocSet::moveFirst() {
  if (!opened || (IsBOF() && IsEOF())) return 0;

  MoveFirst();  return this;
  }


LocSet* LocSet::moveNext() {
  if (!opened || IsEOF()) return 0;

  try {Move(1);} catch (...) {try {Move(1);} catch (...) {return 0;}}

  return IsEOF() ? 0 : this;
  }


LocSet* LocSet::moveLast() {
  if (!opened || (IsBOF() && IsEOF())) return 0;

  MoveLast();   return this;
  }


LocSet* LocSet::movePrev() {
  if (!opened || IsBOF()) return 0;

  try {Move(-1);} catch (...) {try {Move(-1);} catch (...) {return 0;}}

  return IsBOF() ? 0 : this;
  }
