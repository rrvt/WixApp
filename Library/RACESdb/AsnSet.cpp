// AsnSet Record


#include "pch.h"
#include "AsnSet.h"
#include "AccessDB.h"


AsnSet::AsnSet() : AccRcdSet(accessDB.db()), assgnPrefID(0), aPKey(), txt() { }


bool AsnSet::open(TCchar* path) {

  opened = false;

  if (!accessDB.isOpen() && !accessDB.open(path)) return false;

  SetState(CRecordset::dynaset, NULL, CRecordset::none);          // Cache state info and allocate hstmt

  if (!AllocHstmt()) return false;

  OnSetOptions(m_hstmt);   AllocStatusArrays();   m_nFields = 3;

  try {if (!Open(CRecordset::dynaset, _T("AssgnPref"), CRecordset::none)) return false;}
  catch(...) {
    try {if (!Open(CRecordset::snapshot, _T("AssgnPref"), CRecordset::none)) return false;}
    catch(...) {close(); return false;}
    }

  AllocRowset();          // Allocate memory and cache info

  return opened = true;
  }


AsnSet* AsnSetIter::find(int id) {
AsnSet* set = &rcd;

  for ( ; set && set->assgnPrefID <= id; set = (*this)++) if (set->assgnPrefID == id) return set;
  for ( ; set && set->assgnPrefID >= id; set = (*this)--) if (set->assgnPrefID == id) return set;

  return 0;
  }


bool AsnSet::edit()
  {if (!opened) return false;   try {Edit(); return true;} catch(...) {return false;}}


bool AsnSet::addNew()
  {if (!opened) return false;   try {AddNew(); return true;} catch(...) {return false;}}


bool AsnSet::update()
  {if (!opened) return false;   try {Update(); movePrev(); return true;} catch(...) {return false;}}


bool AsnSet::remove()
  {if (!opened) return false;   try {Delete(); movePrev(); return true;} catch(...) {return false;}}


void AsnSet::DoFieldExchange(CFieldExchange* pFX) {

  pFX->SetFieldType(CFieldExchange::outputColumn);

  RFX_Int( pFX, _T("AssgnPrefID"), assgnPrefID);
  RFX_Text(pFX, _T("APKey"),       aPKey);
  RFX_Text(pFX, _T("Txt"),         txt);
  }


AsnSet* AsnSet::moveFirst() {
  if (!opened || (IsBOF() && IsEOF())) return 0;

  MoveFirst();  return this;
  }


AsnSet* AsnSet::moveNext() {
  if (!opened || IsEOF()) return 0;

  try {Move(1);} catch (...) {try {Move(1);} catch (...) {return 0;}}

  return IsEOF() ? 0 : this;
  }


AsnSet* AsnSet::moveLast() {
  if (!opened || (IsBOF() && IsEOF())) return 0;

  MoveLast();   return this;
  }


AsnSet* AsnSet::movePrev() {
  if (!opened || IsBOF()) return 0;

  try {Move(-1);} catch (...) {try {Move(-1);} catch (...) {return 0;}}

  return IsBOF() ? 0 : this;
  }
