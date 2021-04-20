// Access DAO Records


#include "stdafx.h"
#include "DAOrecords.h"


static TCchar* DefPrimaryKey = _T("PrimaryKey");


DAOrcds::DAOrcds(DAOtable* daoTable) : daoTbl(*daoTable) { }


FieldsP DAOrcds::findRecord(const long id) {
variant_t idV    = id;
bstr_t    eq     = "=";

  if (!recordSet && !open(DAOdenyWrite)) return 0;    // Other users cannot view records

  try {recordSet->Index = findPIndex();   recordSet->Seek(eq, idV);  if (recordSet->NoMatch) return 0;}
  catch(ComError& e) {daoError(e);  return 0;}

  return recordSet->GetFields();
  }


FieldsP DAOrcds::findRecord(const float id) {
variant_t idV = id;
bstr_t    eq  = "=";

  if (!recordSet && !open(DAOdenyWrite)) return 0;

  try {recordSet->Index = findPIndex();   recordSet->Seek(eq, idV);   if (recordSet->NoMatch) return 0;}
  catch(ComError& e) {daoError(e);  return 0;}

  return recordSet->GetFields();
  }


FieldsP DAOrcds::findRecord(const String& id) {
Variant idV = id;
bstr_t  eq  = "=";

  if (!recordSet && !open(DAOdenyWrite)) return 0;

  try {recordSet->Index = findPIndex();   recordSet->Seek(eq, idV);   if (recordSet->NoMatch) return 0;}
  catch(ComError& e) {daoError(e);  return 0;}

  return recordSet->GetFields();
  }


bstr_t DAOrcds::findPIndex() {
TableDefP  tableP   = daoTbl.tableP;    if (!tableP)   return DefPrimaryKey;
IndexesPtr indexesP = tableP->Indexes;  if (!indexesP) return DefPrimaryKey;
int        n        = indexesP->Count;;
int        i;
IndexP     indexP;

  for (i = 0; i < n; i++)
                    {indexP = indexesP->GetItem(i); if (indexP && indexP->Primary) return indexP->Name;}

  return DefPrimaryKey;
  }


bool DAOrcds::delCurRcd() {

  if (!recordSet) return false;

  try {recordSet->Delete(); recordSet->MoveNext(); return true;}
  catch (_com_error& e) {daoError(e); return false;}
  }


long DAOrcds::update() {

  if (!recordSet) return false;

  try {return recordSet->Update(dbUpdateRegular, false);}
  catch (_com_error &e) {daoError(e);}

  return false;
  }


FieldsP DAOrcdsIter::operator() (DAOoptions opt) {

  if (!rcds.open(opt)) return 0;

  nRecords = recordSet->RecordCount;  if (!nRecords)  return 0;

  recordSet->MoveFirst();             if (recordSet->DaoEof) return 0;

  return recordSet->GetFields();
  }


FieldsP DAOrcdsIter::operator++ (int) {

  if (!recordSet) return 0;

  recordSet->MoveNext(); if (recordSet->DaoEof) return 0;

  return recordSet->GetFields();
  }



#if 0
DAOrecords::DAOrecords(DAOtable* dsc, DaoOptions option) : table(0), recordSet(0), nRecords(0) {
  open(dsc, option);
  }
#endif
#if 0
FieldsP DAOrcds::operator() (DaoOptions opt) {

  recordSet = tblDsc.openRecordSet(opt);   if (!recordSet) return 0;

  nRecords = recordSet->RecordCount;       if (!nRecords)  return 0;

  recordSet->MoveFirst();                  if (recordSet->DaoEof) return 0;

  FieldsP fields = recordSet->GetFields(); return fields;
  }



FieldsP DAOrcds::operator++ (int) {

  if (!recordSet) return 0;

  recordSet->MoveNext(); if (recordSet->DaoEof) return 0;

  FieldsP fields = recordSet->GetFields();   return fields;
  }
#endif



#if 0
bool DAOrecords::open(DAOtable* table, DaoOptions option) {

  if (!table) return false;

  tableP = table->tableP; recordSet = table->openRecordSet(option);   if (!recordSet) return false;

  nRecords = recordSet->RecordCount;  return true;
  }


bool DAOrecords::startLoop() {

  if (nRecords && recordSet) {recordSet->MoveFirst(); return !recordSet->DaoEof;}

  return false;
  }


bool DAOrecords::nextRecord() {

  if (recordSet) recordSet->MoveNext(); return !recordSet->DaoEof;

  return false;
  }


bool DAOrecords::findRecord(const long id) {
variant_t idV    = id;
bstr_t    eq     = "=";
int       noRcds = 0;
variant_t x;

  if (!recordSet) return false;

  try {recordSet->Index = findPIndex();   recordSet->Seek(eq, idV);   return !recordSet->NoMatch;}
  catch (...) {}

  return false;
  }



bool DAOrecords::findRecord(const float id) {
variant_t idV = id;
bstr_t    eq  = "=";
int       noRcds = 0;
variant_t x;

  if (!recordSet) return false;

  try {recordSet->Index = findPIndex();   recordSet->Seek(eq, idV);   return !recordSet->NoMatch;}
  catch (...) {}

  return false;
  }


bool DAOrecords::findRecord(const String& id) {
Variant idV    = id;
bstr_t  eq     = "=";
int     noRcds = 0;
Variant x;

  if (!recordSet) return false;

  try {recordSet->Index = findPIndex();   recordSet->Seek(eq, idV);   return !recordSet->NoMatch;}
  catch (...) {}

  return false;
  }


bstr_t DAOrecords::findPIndex() {
static TCchar* primaryKey = _T("PrimaryKey");  return primaryKey;
#if 0
IndexesPtr idxs;
_IndexPtr  idx;
int        count;
int        i;

  if (!table) return primaryKey;

  idxs = table->Indexes;    if (!idxs) return primaryKey;

  count = idxs->Count;

  for (i = 0; i < count; i++) {idx = idxs->GetItem(i);    if (idx && idx->Primary) return idx->Name;}

  return primaryKey;
#endif
  }




long DAOrecords::update() {

  if (!recordSet) return false;

  try {if (recordSet) return recordSet->Update(dbUpdateRegular, false);}
  catch (_com_error &e) {daoError(e);}

  return false;
  }


bool DAOrecords::deleteRecord(const long rcdID) {

  if (!findRecord(rcdID)) return false;

  try {delCurRcd();} catch(...) {return false;}

  return true;
  }


bool DAOrecords::deleteRecord(const float   rcdID) {

  if (!findRecord(rcdID)) return false;

  try {delCurRcd();} catch(...) {return false;}

  return true;
  }


bool DAOrecords::deleteRecord(const String& rcdID) {

  if (!findRecord(rcdID)) return false;

  try {delCurRcd();} catch(...) {return false;}

  return true;
  }



bool DAOrecords::delCurRcd() {
  try {if (recordSet) recordSet->Delete(); recordSet->MoveNext(); return true;}
  catch (_com_error& e) {daoError(e); return false;}
  }
#endif

