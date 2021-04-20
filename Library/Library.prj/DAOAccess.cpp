// Database Access by DAO
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.


#include "stdafx.h"
#include "DAOAccess.h"
#include "Variant.h"


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

DAOAccess::DAOAccess() : engine(0), workspace(0), db(0) {
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

bool DAOAccess::open(TCchar* path) {

  if (!workspace || !path || *path == 0) return false;

  if (db && this->path == path) return true;

  try {db = workspace->OpenDatabase(bstr_t(path));}
  catch (ComError& e) {daoError(e); return false;}

  this->path = path; return db != 0;
  }


void DAOAccess::close() {if (db) {db->Close(); db = 0;}}



void daoError(ComError& e) {
HRESULT hrstl = e.Error();
bstr_t  desc  = e.Description();
String  m     = e.ErrorMessage();
bstr_t  src   = e.Source();
String  msg;

  msg.format(_T("Source: %s, Com Error: 0x%lx\n\n%s"), (Tchar*) src, hrstl, (Tchar*) desc);

  MessageBox(0, msg, src, MB_OK | MB_ICONWARNING);
  }



#if 0
DAOtable* DAOtables::find(TCchar* name) {
int n = data.end();
int i;

  if (!name || !_tcslen(name)) return 0;

  for (i = 0; i < n; i++) if (data[i].name == name) return &data[i];

  return 0;
  }
#endif
#if 0
    dsc.name = tableDef->GetName();  dsc.tableDef = tableDef;   dsc.flds = tableDef->GetFields();

    if (dsc.flds) dsc.nFlds = dsc.flds->Count;

    recordSet = dsc.openRecordSet(DaoReadOnly);

    if (recordSet) dsc.nRcds = recordSet->RecordCount;

    recordSet->Close();
#endif


#if 0
bstr_t DAOtable::findPindex() {
static TCchar* primaryKey = _T("PrimaryKey");
IndexesPtr     idxs;
_IndexPtr      idx;
int            n;
int            i;

  if (!tableP) return primaryKey;

  idxs = tableP->Indexes;    if (!idxs) return primaryKey;

  n = idxs->Count;

  for (i = 0; i < n; i++) {

    idx = idxs->GetItem(i);

    if (idx && idx->Primary)
      return idx->Name;
    }

  return primaryKey;
  }
#endif

#if 0
    __declspec(property(get=GetName,put=PutName))
    _bstr_t Name;
    __declspec(property(get=GetForeign))
    VARIANT_BOOL Foreign;
    __declspec(property(get=GetUnique,put=PutUnique))
    VARIANT_BOOL Unique;
    __declspec(property(get=GetClustered,put=PutClustered))
    VARIANT_BOOL Clustered;
    __declspec(property(get=GetRequired,put=PutRequired))
    VARIANT_BOOL Required;
    __declspec(property(get=GetIgnoreNulls,put=PutIgnoreNulls))
    VARIANT_BOOL IgnoreNulls;
    __declspec(property(get=GetPrimary,put=PutPrimary))
    VARIANT_BOOL Primary;
    __declspec(property(get=GetDistinctCount))
    long DistinctCount;
    __declspec(property(get=GetFields,put=PutFields))
    _variant_t Fields;
#endif

