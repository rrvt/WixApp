// StsSet Record


#include "pch.h"
#include "StsSet.h"
#include "AccessDB.h"


StsSet::StsSet() : AccRcdSet(accessDB.db()), stsID(0), abbreviation(), description() { }


bool StsSet::open(TCchar* path) {

  opened = false;

  if (!accessDB.isOpen() && !accessDB.open(path)) return false;

  SetState(CRecordset::dynaset, NULL, CRecordset::none);          // Cache state info and allocate hstmt

  if (!AllocHstmt()) return false;

  OnSetOptions(m_hstmt);   AllocStatusArrays();   m_nFields = 3;

  try {if (!Open(CRecordset::dynaset, _T("Status"), CRecordset::none)) return false;}
  catch(...) {
    try {if (!Open(CRecordset::snapshot, _T("Status"), CRecordset::none)) return false;}
    catch(...) {close(); return false;}
    }

  AllocRowset();          // Allocate memory and cache info

  return opened = true;
  }


StsSet* StsSetIter::find(int id) {
StsSet* set = &rcd;

  for ( ; set && set->stsID <= id; set = (*this)++) if (set->stsID == id) return set;
  for ( ; set && set->stsID >= id; set = (*this)--) if (set->stsID == id) return set;

  return 0;
  }


bool StsSet::edit()
  {if (!opened) return false;   try {Edit(); return true;} catch(...) {return false;}}


bool StsSet::addNew()
  {if (!opened) return false;   try {AddNew(); return true;} catch(...) {return false;}}


bool StsSet::update()
  {if (!opened) return false;   try {Update(); movePrev(); return true;} catch(...) {return false;}}


bool StsSet::remove()
  {if (!opened) return false;   try {Delete(); movePrev(); return true;} catch(...) {return false;}}


void StsSet::DoFieldExchange(CFieldExchange* pFX) {

  pFX->SetFieldType(CFieldExchange::outputColumn);

  RFX_Int( pFX, _T("StsID"),        stsID);
  RFX_Text(pFX, _T("Abbreviation"), abbreviation);
  RFX_Text(pFX, _T("Description"),  description, 1024);
  }


StsSet* StsSet::moveFirst() {
  if (!opened || (IsBOF() && IsEOF())) return 0;

  MoveFirst();  return this;
  }


StsSet* StsSet::moveNext() {
  if (!opened || IsEOF()) return 0;

  try {Move(1);} catch (...) {try {Move(1);} catch (...) {return 0;}}

  return IsEOF() ? 0 : this;
  }


StsSet* StsSet::moveLast() {
  if (!opened || (IsBOF() && IsEOF())) return 0;

  MoveLast();   return this;
  }


StsSet* StsSet::movePrev() {
  if (!opened || IsBOF()) return 0;

  try {Move(-1);} catch (...) {try {Move(-1);} catch (...) {return 0;}}

  return IsBOF() ? 0 : this;
  }
