// CtySet Record


#include "pch.h"
#include "CtySet.h"
#include "AccessDB.h"


CtySet::CtySet() : AccRcdSet(accessDB.db()), cityStateID(0), city(), state(),
          zip() { }


bool CtySet::open(TCchar* path) {

  opened = false;

  if (!accessDB.isOpen() && !accessDB.open(path)) return false;

  SetState(CRecordset::dynaset, NULL, CRecordset::none);          // Cache state info and allocate hstmt

  if (!AllocHstmt()) return false;

  OnSetOptions(m_hstmt);   AllocStatusArrays();   m_nFields = 4;

  try {if (!Open(CRecordset::dynaset, _T("CityState"), CRecordset::none)) return false;}
  catch(...) {
    try {if (!Open(CRecordset::snapshot, _T("CityState"), CRecordset::none)) return false;}
    catch(...) {close(); return false;}
    }

  AllocRowset();          // Allocate memory and cache info

  return opened = true;
  }


CtySet* CtySetIter::find(int id) {
CtySet* set = &rcd;

  for ( ; set && set->cityStateID <= id; set = (*this)++) if (set->cityStateID == id) return set;
  for ( ; set && set->cityStateID >= id; set = (*this)--) if (set->cityStateID == id) return set;

  return 0;
  }


bool CtySet::edit()
  {if (!opened) return false;   try {Edit(); return true;} catch(...) {return false;}}


bool CtySet::addNew()
  {if (!opened) return false;   try {AddNew(); return true;} catch(...) {return false;}}


bool CtySet::update()
  {if (!opened) return false;   try {Update(); movePrev(); return true;} catch(...) {return false;}}


bool CtySet::remove()
  {if (!opened) return false;   try {Delete(); movePrev(); return true;} catch(...) {return false;}}


void CtySet::DoFieldExchange(CFieldExchange* pFX) {

  pFX->SetFieldType(CFieldExchange::outputColumn);

  RFX_Int( pFX, _T("CityStateID"), cityStateID);
  RFX_Text(pFX, _T("City"),        city);
  RFX_Text(pFX, _T("State"),       state);
  RFX_Text(pFX, _T("Zip"),         zip);
  }


CtySet* CtySet::moveFirst() {
  if (!opened || (IsBOF() && IsEOF())) return 0;

  MoveFirst();  return this;
  }


CtySet* CtySet::moveNext() {
  if (!opened || IsEOF()) return 0;

  try {Move(1);} catch (...) {try {Move(1);} catch (...) {return 0;}}

  return IsEOF() ? 0 : this;
  }


CtySet* CtySet::moveLast() {
  if (!opened || (IsBOF() && IsEOF())) return 0;

  MoveLast();   return this;
  }


CtySet* CtySet::movePrev() {
  if (!opened || IsBOF()) return 0;

  try {Move(-1);} catch (...) {try {Move(-1);} catch (...) {return 0;}}

  return IsBOF() ? 0 : this;
  }
