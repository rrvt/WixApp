// Access DAO Records


#pragma once
#include "DAOAccess.h"
#include "DAOtables.h"


class DAOrcds {
DAOtable&  daoTbl;
RecordSetP recordSet;
int        nRecords;

public:

  DAOrcds(DAOtable* daoTable);
 ~DAOrcds() {close();}                                    //rcdSet.

  FieldsP findRecord(const long    id);
  FieldsP findRecord(const float   id);
  FieldsP findRecord(const String& id);

  FieldsP addNew()
                 {return recordSet && recordSet->AddNew() == S_OK ? recordSet->GetFields() : (FieldsP)0;}
  long    edit()   {return recordSet ? recordSet->Edit()   : 0;}

  bool    delCurRcd();

  long    update();

private:

  RecordSetP open(DAOoptions opt) {return recordSet = daoTbl.openRecordSet(opt);}

  void       close()  {if (recordSet) recordSet->Close(); recordSet = 0;}

  bstr_t     findPIndex();

  DAOrcds() : daoTbl(*(DAOtable*)0) { }  //, rcdSet(0, DaoDenyWrite)

  friend class DAOrcdsIter;
  };


class DAOrcdsIter {
DAOrcds     rcds;
RecordSetP& recordSet;
int&        nRecords;

public:

  DAOrcdsIter(DAOtable* daoTable) : rcds(daoTable), recordSet(rcds.recordSet),
                                                                              nRecords(rcds.nRecords) { }
  FieldsP operator() (DAOoptions opt = DAOdenyWrite);
  FieldsP operator++ (int);

private:

  DAOrcdsIter() : rcds(0), recordSet(*(RecordSetP*)0), nRecords(*(int*)0) { }
  };



