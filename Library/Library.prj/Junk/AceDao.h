// Database Access by DAO
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.


#pragma once
#include "Expandable.h"
#include "IterT.h"


#import <C:\Program Files (x86)\Microsoft Office\root\Office16\ACEDAO.DLL> rename(_T("EOF"), _T("DaoEof"))
using namespace DAO;


// Global Function

String getDbCppType(DataTypeEnum type);     // Translate Access Database type value into a c++ type


// Normalizing type definitions from ACEDAO.DLL

typedef _DBEnginePtr EngineP;
typedef Workspace*   WorkspaceP;
typedef DatabasePtr  DatabaseP;
typedef TableDefsPtr TableDefsP;
typedef _TableDefPtr TableDefP;
typedef RecordsetPtr RecordSetP;
typedef FieldsPtr    FieldsP;
typedef _FieldPtr    FieldP;
typedef _com_error   ComError;


enum DaoOptions {DaoDenyWrite      = DAO::dbDenyWrite,
                 DaoDenyRead       = DAO::dbDenyRead,
                 DaoReadOnly       = DAO::dbReadOnly,
                 DaoAppendOnly     = DAO::dbAppendOnly,
                 DaoInconsistent   = DAO::dbInconsistent,
                 DaoConsistent     = DAO::dbConsistent,
                 DaoSQLPassThrough = DAO::dbSQLPassThrough,
                 DaoFailOnError    = DAO::dbFailOnError,
                 DaoForwardOnly    = DAO::dbForwardOnly,
                 DaoSeeChanges     = DAO::dbSeeChanges,
                 DaoRunAsync       = DAO::dbRunAsync,
                 DaoExecDirect     = DAO::dbExecDirect
                 };


class AceDao {
EngineP    engine;
WorkspaceP workspace;
String     path;
DatabaseP  db;

public:

  AceDao();
 ~AceDao() {close();}

  bool    open(TCchar* path);             // Database
  void    close();                        // Close Database
  String& getPath() {return path;}

  friend class AceTables;
  };


struct TableDesc {
String    name;
TableDefP table;
FieldsP   flds;
int       nFlds;
int       nRcds;

  TableDesc() : table(0), flds(0), nFlds(0), nRcds(0) {}
  TableDesc(TableDesc& dsc) {copy(dsc);}

  TableDesc& operator= (TableDesc& dsc) {copy(dsc); return *this;}

  RecordSetP openRecordSet(DaoOptions option);

private:

  void copy(TableDesc& src);    // Error:  Fix
  };




// Define the iterator used to look at the data in the datastore.  It is here so that it can be friended

class AceTables;
typedef IterT<AceTables, TableDesc> ATIter;                       // Iterator for the Store



class AceTables {
AceDao&                  dao;
int                      i;                        // Loop index
bool                     autoOpen;
int                      nTables;
Expandable<TableDesc, 4> tables;

public:

  AceTables(AceDao& acedao);
 ~AceTables() {if (autoOpen) dao.close();}

  int        count() {return nTables;}
  TableDesc* find(TCchar* name);

private:

  // returns either a pointer to data (or datum) at index i in array or zero
  TableDesc* datum(int i) {return 0 <= i && i < nData() ? &tables[i] : 0;}

  // returns number of data items in array
  int   nData()      {return tables.end();}

  friend typename ATIter;
  };


class AceRecordSet {
TableDefP  table;
RecordSetP recordSet;
int        nRecords;

public:

  AceRecordSet() : table(0), recordSet(0), nRecords(0) {}
  AceRecordSet(TableDesc* dsc, DaoOptions option);
  AceRecordSet(const AceRecordSet& rs) : table(rs.table), recordSet(rs.recordSet),
                                                                                 nRecords(rs.nRecords) {}
 ~AceRecordSet() {close();}

  bool       open(TableDesc* dsc, DaoOptions option);
  bool       isOpen() {return recordSet != 0;}
  void       close()  {if (recordSet) recordSet->Close(); recordSet = 0;}

  bool       startLoop();
  bool       nextRecord();
  RecordSetP getCurRcd() {return recordSet;}

  bool       addNew() {return recordSet ? recordSet->AddNew() == S_OK : false;}
  long       edit()   {return recordSet ? recordSet->Edit()   : 0;}
  long       update();

  bool       findRecord(const long    id);
  bool       findRecord(const float   id);
  bool       findRecord(const String& id);

  bool       deleteRecord(const long    rcdID);
  bool       deleteRecord(const float   rcdID);
  bool       deleteRecord(const String& rcdID);
  bool       deleteCurrentRecord();

  AceRecordSet& operator= (const AceRecordSet& rs)
                                    {table = rs.table; recordSet = rs.recordSet; nRecords = rs.nRecords;}
private:

  bstr_t     findPIndex();
  void       openRecordFlds(int rcdType, int rcdOption);
  };


struct AceFldNameDsc {
String       name;
DataTypeEnum type;
long         attr;

  AceFldNameDsc() : type((DataTypeEnum) 0), attr(0) {}
  };


class AceFieldNames;
typedef ObjIterT<AceFieldNames, AceFldNameDsc> AFNIter;


// The field names are stored in the table definition

class AceFieldNames {
TableDefP table;
FieldsP   fields;

int       i;                                // Loop index
int       n;                                // Number of Fields

public:

  AceFieldNames() : table(0), fields(0) , i(0), n(0) {}
  AceFieldNames(TableDesc* dsc) : table(0), fields(0), i(0), n(0)
      {if (!dsc) return; table = dsc->table; fields = table->GetFields(); if (fields) n = fields->Count;}

  bool open(TableDesc* dsc);

  bool isOpen() {return fields != 0;}

  int  noFields() {return n;}

private:

  // returns either a pointer to data (or datum) at index i in array or zero
  AceFldNameDsc* getDatum(int i, AceFldNameDsc& datum);

  // returns number of data items in array
  int   nData() {return fields ? fields->Count : 0;}

  friend typename AFNIter;
  };


class AceFieldDsc {
FieldP    field;

public:

variant_t value;
long      attr;

  AceFieldDsc() : field(0), value(0), attr(0) {}

  void write(variant_t v);

  friend class AceFields;
  friend class FieldsIter;
  };


class AceFields;
typedef ObjIterT<AceFields, AceFieldDsc> AFIter;


// Manage Fields in Current Record

class AceFields {
RecordSetP curRcd;
FieldsP    fields;
//FieldP     field;


public:

  AceFields() { }

  AceFields(AceRecordSet& currentRcd) : curRcd(currentRcd.getCurRcd()), fields(0)
                                                              {if (curRcd) fields = curRcd->GetFields();}

  AceFields(AceRecordSet* currentRcd) : curRcd(currentRcd->getCurRcd()), fields(0)
                                                              {if (curRcd) fields = curRcd->GetFields();}

  void initialize(AceRecordSet& currentRcd)
                             {curRcd = currentRcd.getCurRcd(); if (curRcd) fields = curRcd->GetFields();}

private:

  // returns either a pointer to data (or datum) at index i in array or zero
  AceFieldDsc* getDatum(int i, AceFieldDsc& datum);

  // returns number of data items in array
  int   nData() {return fields ? fields->Count : 0;}

  friend typename AFIter;
  };



class RcdsIter {
TableDesc&   tblDsc;
AceRecordSet rcdSet;
RecordSetP   curRcd;
FieldsP      fields;

public:

  RcdsIter(TableDesc& tblDsc);
 ~RcdsIter() { }

  FieldsP operator() (DaoOptions opt = DaoDenyWrite);
  FieldsP operator++ (int);

private:

  RcdsIter() : tblDsc(*(TableDesc*)0) { }
  };


class FieldsIter {

int         index;
FieldsP     fields;
AceFieldDsc fieldDsc;

public:

  FieldsIter(FieldsP fieldsP) : fields(fieldsP) { }

  AceFieldDsc* operator() ();
  AceFieldDsc* operator++ (int);

private:

  AceFieldDsc* getDatum();

  FieldsIter() : fields(*(FieldsP*)0) { }
  };


