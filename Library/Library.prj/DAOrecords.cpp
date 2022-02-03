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

