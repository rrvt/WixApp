// Database Access by DAO
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.


#include "stdafx.h"
#include "AceDao.h"
#include "Variant.h"


static void comError(ComError& e);


// The CoCreateInstance helper function provides a convenient shortcut by connecting to the class object
// associated with the specified CLSID, creating an uninitialized instance, and releasing the class
// object.
//
// HRESULT CoCreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter, DWORD dwClsContext,
//                                                                             REFIID riid, LPVOID* ppv);
//   where:
//     HRESULT       - S_OK (success), REGDB_E_CLASSNOTREG, CLASS_E_NOAGGREGATION (both failures)
//     rclsid        - [in] Class identifier associated with the data and code that are used to create
//                          the object
//     pUnkOuter     - [in] If NULL, indicates that the object is not being created as part of an
//                          aggregate.
//                          If non-NULL, pointer to the aggregate object's IUnknown interface (the
//                          controlling IUnknown).
//     dwClsContext  - [in] Specifies the context in which the code that manages the newly created object
//                          will run. The only valid value for this parameter is CLSCTX_INPROC_SERVER.
//                          This is from the enumeration CLSCTX. Any other value results in a return value
//                          of E_NOTIMPL.
//     riid          - [in] Reference to the identifier of the interface to be used to communicate with
//                          the object.
//     ppv           - [out] Address of pointer variable that receives the interface pointer requested in
//                           riid. Upon successful return, ppv contains the requested interface pointer.

//  A DAO workspace, represented in MFC by class CDaoWorkspace, manages a session with the Microsoft Jet
//  database engine.  A workspace can contain one or more open DAO database objects, represented in your
//  program, explicitly or implicitly, by CDaoDatabase objects.  In DAO, workspaces manage a single
//  transaction space in which any transaction applies to all open databases and recordsets.  DAO
//  workspaces also manage database security.

AceDao::AceDao() : engine(0), workspace(0), db(0) {
HRESULT       hr;
WorkspacesPtr workspaces;

  if (CoInitialize(0) != S_OK) return;

  hr = CoCreateInstance(__uuidof(DAO::DBEngine), NULL, CLSCTX_ALL, IID_IDispatch, (LPVOID*) &engine);

  if (hr != S_OK)
    MessageBox(0, _T("Please install an Access Runtime DLL from Microsoft"),
                                                                  _T("Database Engine Error"), MB_OK);
  if (!engine) return;

  workspaces = engine->GetWorkspaces(); workspace = workspaces->GetItem(&variant_t(0));

  if (!workspace) MessageBox(0, _T("Unable to establish a workspace"),
                                                                  _T("Database Engine Error"), MB_OK);
  }


//  DatabasePtr OpenDatabase (bstr_t Name, const variant_t &Options = vtMissing,
//                            const variant_t &ReadOnly = vtMissing,
//                            const variant_t &Connect = vtMissing);
// where:
//  Name       - The name and path (String type) of Microsoft Jet Database file (Microsoft Access file)
//               that you want to open. It can be also the DSN (Data Source Name) of ODBC Data Source.
//  Options    - (Optional) may be either true or false:
//               true - Open the database file in exclusive mode (only one user can access the database)
//               false - (Default) Open the database file in shared mode (multiple users can access the
//               database)
//  ReadOnly   - (Optional) may be either true or false:
//               true - The user can't modify the database.
//               false - (Default) The user can append, edit or delete data in the open database.
//  Connection - (Optional) String that provides additional input to function (e.g. password)

bool AceDao::open(TCchar* path) {

  if (!workspace || !path || *path == 0) return false;

  if (db && this->path == path) return true;

  try {db = workspace->OpenDatabase(bstr_t(path));}
  catch (ComError& e) {comError(e); return false;}

  this->path = path; return db != 0;
  }


void AceDao::close() {if (db) {db->Close(); db = 0;}}



RecordSetP TableDesc::openRecordSet(DaoOptions option) {

  try {return table->OpenRecordset(variant_t(DAO::dbOpenTable), variant_t(option));}
  catch(ComError& e) {comError(e);}

  return 0;
  }



void TableDesc::copy(TableDesc& src)
              {name = src.name; table = src.table; flds = src.flds; nFlds = src.nFlds; nRcds = src.nRcds;}



AceTables::AceTables(AceDao& aceDao) : dao(aceDao), i(0), autoOpen(false), nTables(0)  {
TableDefsP tableDefs;
TableDefP  table;
RecordSetP recordSet;

int        n;
int        i;

  if (!dao.db) {if (!aceDao.open(aceDao.path)) return;   autoOpen = true;}

  tableDefs = dao.db->GetTableDefs();    n = tableDefs->Count;

  for (i = 0; i < n; i++) {

    table = tableDefs->GetItem(i);   if (!table) continue;

    if (table->GetAttributes()) continue;           // Non zero attribute is a hidden table

    TableDesc& dsc = tables.nextData();

    dsc.name = table->GetName();  dsc.table = table;   dsc.flds = table->GetFields();

    if (dsc.flds) dsc.nFlds = dsc.flds->Count;

    recordSet = dsc.openRecordSet(DaoReadOnly);

    if (recordSet) dsc.nRcds = recordSet->RecordCount;

    recordSet->Close();
    }

  nTables = tables.end();
  }



TableDesc* AceTables::find(TCchar* name) {
int i;

  if (!name || !_tcslen(name)) return 0;

  for (i = 0; i < nTables; i++) if (tables[i].name == name) return &tables[i];

  return 0;
  }





void comError(ComError& e) {
HRESULT hrstl = e.Error();
bstr_t  desc  = e.Description();
String  m     = e.ErrorMessage();
bstr_t  src   = e.Source();
String  msg;

  msg.format(_T("Source: %s, Com Error: 0x%lx\n\n%s"), (Tchar*) src, hrstl, (Tchar*) desc);

  MessageBox(0, msg, src, MB_OK | MB_ICONWARNING);
  }



// -----------------------------------



AceRecordSet::AceRecordSet(TableDesc* dsc, DaoOptions option) : table(0), recordSet(0), nRecords(0) {
  open(dsc, option);
  }


bool AceRecordSet::open(TableDesc* dsc, DaoOptions option) {

  if (!dsc) return false;

  table = dsc->table; nRecords = dsc->nRcds;  recordSet = dsc->openRecordSet(option); return isOpen();
  }


//#if 0
bool AceRecordSet::startLoop() {

  if (nRecords && recordSet) {recordSet->MoveFirst(); return !recordSet->DaoEof;}

  return false;
  }


bool AceRecordSet::nextRecord() {

  if (recordSet) recordSet->MoveNext(); return !recordSet->DaoEof;

  return false;
  }
//#endif


bool AceRecordSet::findRecord(const long id) {
variant_t idV    = id;
bstr_t    eq     = "=";
int       noRcds = 0;
variant_t x;

  if (!recordSet) return false;

  try {recordSet->Index = findPIndex();   recordSet->Seek(eq, idV);   return !recordSet->NoMatch;}
  catch (...) {}

  return false;
  }



bool AceRecordSet::findRecord(const float id) {
variant_t idV = id;
bstr_t    eq  = "=";
int       noRcds = 0;
variant_t x;

  if (!recordSet) return false;

  try {recordSet->Index = findPIndex();   recordSet->Seek(eq, idV);   return !recordSet->NoMatch;}
  catch (...) {}

  return false;
  }


bool AceRecordSet::findRecord(const String& id) {
Variant idV    = id;
bstr_t  eq     = "=";
int     noRcds = 0;
Variant x;

  if (!recordSet) return false;

  try {recordSet->Index = findPIndex();   recordSet->Seek(eq, idV);   return !recordSet->NoMatch;}
  catch (...) {}

  return false;
  }


bstr_t AceRecordSet::findPIndex() {
static TCchar* primaryKey = _T("PrimaryKey");
IndexesPtr idxs;
_IndexPtr  idx;
int        count;
int        i;

  if (!table) return primaryKey;

  idxs = table->Indexes;    if (!idxs) return primaryKey;

  count = idxs->Count;

  for (i = 0; i < count; i++) {idx = idxs->GetItem(i);    if (idx && idx->Primary) return idx->Name;}

  return primaryKey;
  }




long AceRecordSet::update() {

  if (!recordSet) return false;

  try {if (recordSet) return recordSet->Update(dbUpdateRegular, false);}
  catch (_com_error &e) {comError(e);}

  return false;
  }


bool AceRecordSet::deleteRecord(const long rcdID) {

  if (!findRecord(rcdID)) return false;

  try {deleteCurrentRecord();} catch(...) {return false;}

  return true;
  }


bool AceRecordSet::deleteRecord(const float   rcdID) {

  if (!findRecord(rcdID)) return false;

  try {deleteCurrentRecord();} catch(...) {return false;}

  return true;
  }


bool AceRecordSet::deleteRecord(const String& rcdID) {

  if (!findRecord(rcdID)) return false;

  try {deleteCurrentRecord();} catch(...) {return false;}

  return true;
  }



bool AceRecordSet::deleteCurrentRecord() {
  try {if (recordSet) recordSet->Delete(); recordSet->MoveNext(); return true;}
  catch (_com_error& e) {comError(e); return false;}
  }


bool AceFieldNames::open(TableDesc* dsc) {
                                 if (!dsc)    return false;
  table  = dsc->table;           if (!table)  return false;
  fields = table->GetFields();   if (!fields) return false;
  n      = fields->Count;                     return true;
  }


AceFldNameDsc* AceFieldNames::getDatum(int i, AceFldNameDsc& datum) {
FieldP field;

  if (i < 0 || i >= nData()) return 0;

  field = fields->GetItem(i);   if (!field) return 0;

  datum.name = field->GetName();
  datum.type = (DataTypeEnum) field->Type;
  datum.attr = field->Attributes;

  return &datum;
  }


AceFieldDsc* AceFields::getDatum(int i, AceFieldDsc& datum) {
FieldP field;

  if (i < 0 || i >= nData()) return 0;

  field = fields->GetItem(i);   if (!field) return 0;

  datum.field = field;
  datum.value = field->GetValue();   notNull(datum.value);
  datum.attr  = field->Attributes;

  return &datum;
  }



void AceFieldDsc::write(variant_t v) {
  value = v;

  try {if (field) field->Value = value;}
  catch(ComError& e) {comError(e);}
  }


// Translate Access Database type value into a c++ type

String getDbCppType(DataTypeEnum type) {

  switch (type) {
    case dbBoolean        : return _T("bool");
    case dbByte           : return _T("Byte");
    case dbInteger        : return _T("int");
    case dbLong           : return _T("long");
    case dbCurrency       : return _T("Currency");
    case dbSingle         : return _T("float");
    case dbDouble         : return _T("double");
    case dbDate           : return _T("Date");
    case dbBinary         : return _T("dbBinary");
    case dbText           : return _T("String");
    case dbLongBinary     : return _T("dbLongBinary");
    case dbMemo           : return _T("String");
    case dbGUID           : return _T("dbGUID");
    case dbBigInt         : return _T("dbBigInt");
    case dbVarBinary      : return _T("dbVarBinary");
    case dbChar           : return _T("char");
    case dbNumeric        : return _T("dbNumeric");
    case dbDecimal        : return _T("dbDecimal");
    case dbFloat          : return _T("float");
    case dbTime           : return _T("dbTime");
    case dbTimeStamp      : return _T("dbTimeStamp");
    case dbAttachment     : return _T("dbAttachment");
    case dbComplexByte    : return _T("dbComplexByte");
    case dbComplexInteger : return _T("dbComplexInteger");
    case dbComplexLong    : return _T("dbComplexLong");
    case dbComplexSingle  : return _T("dbComplexSingle");
    case dbComplexDouble  : return _T("dbComplexDouble");
    case dbComplexGUID    : return _T("dbComplexGUID");
    case dbComplexDecimal : return _T("dbComplexDecimal");
    case dbComplexText    : return _T("dbComplexText");
    default               : break;
    }
  String t = _T("<"); t += type; t += _T(">");  return t;
  }



#if 0
bool AceFieldNames::startLoop(AceFldNameDsc& fldDsc) {

  if (!fields) return false;

  n = fields->Count; i = -1;  return nextDesc(fldDsc);
  }


bool AceFieldNames::nextDesc( AceFldNameDsc& fldDsc) {
FieldP field;

  if (++i >= n) return false;

  field = fields->GetItem(i); if (!field)   return false;

  fldDsc.name = field->GetName();
  fldDsc.type = (DataTypeEnum) field->Type;
  fldDsc.attr = field->Attributes;            return true;
  }
#endif


#if 0
bool AceFields::startLoop(AceFieldDsc& dsc) {

  if (!fields) {i = n = 0; return false;}

  n = fields->Count; i = -1;  return nextField(dsc);
  }


bool AceFields::nextField(AceFieldDsc& dsc) {
FieldP field;

  if (++i >= n) return false;

  field = fields->GetItem(i);    if (!field) return false;

  dsc.field = field;
  dsc.value = field->GetValue();   notNull(dsc.value);
  dsc.attr  = field->Attributes;   return true;
  }
#endif

