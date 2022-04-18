// Database Access by DAO
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.


#pragma once
#include "Expandable.h"
#include "IterT.h"
//#import <C:\Program Files\Microsoft Office\root\Office16\ACEDAO.DLL> rename(_T("EOF"), _T("DaoEof"))
//       C:\Program Files (x86)\Microsoft Office\root\Office16
#import <C:\Program Files (x86)\Microsoft Office\root\Office16\ACEDAO.DLL> rename(_T("EOF"), _T("DaoEof"))
using namespace DAO;


// Normalizing type definitions from ACEDAO.DLL

typedef _DBEnginePtr EngineP;
typedef Workspace*   WorkspaceP;
typedef DatabasePtr  DatabaseP;
typedef TableDefsPtr TableDefsP;
typedef _TableDefPtr TableDefP;
typedef RecordsetPtr RecordSetP;
typedef FieldsPtr    FieldsP;
typedef _FieldPtr    FieldP;
typedef _IndexPtr    IndexP;
typedef _com_error   ComError;


enum DAOoptions {DAOdenyWrite      = DAO::dbDenyWrite,      // Other users cannot view records
                 DAOdenyRead       = DAO::dbDenyRead,       // Other users cannot view records
                 DAOreadOnly       = DAO::dbReadOnly,       // You can only view records;
                                                            // other users can modify them.
                 DAOappendOnly     = DAO::dbAppendOnly,     // You can only append new records
                 DAOinconsistent   = DAO::dbInconsistent,   //  Inconsistent updates are allowed
                                                            // (dynaset-type recordset only).
                 DAOconsistent     = DAO::dbConsistent,     // Only consistent updates are allowed
                                                            // (dynaset-type recordset only).
                 DAOsqlPassThrough = DAO::dbSQLPassThrough, // Executes an SQL pass-through query.
                                                            // Setting this option passes the SQL
                                                            // statement to an ODBC database for
                                                            // processing (Microsoft Access workspaces
                                                            // only).
                 DAOfailOnError    = DAO::dbFailOnError,    // Rolls back updates if an error occurs
                                                            // (Microsoft Access workspaces only)
                 DAOforwardOnly    = DAO::dbForwardOnly,    // Creates a forward-only scrolling
                                                            // snapshot-type Recordset (snapshot-type
                                                            // only).
                 DAOseeChanges     = DAO::dbSeeChanges,     // Generates a run-time error if another
                                                            // user is changing data you are editing
                                                            // (dynaset-type only).
                 DAOrunAsync       = DAO::dbRunAsync,       // Executes the query asynchronously
                 DAOexecDirect     = DAO::dbExecDirect      // Executes the query without first calling
                 };                                         // the SQLPrepare ODBC function.



// Global Function

void   daoError(ComError& e);


class DAOAccess {
EngineP    engine;
WorkspaceP workspace;
String     path;
DatabaseP  db;

public:

  DAOAccess();
 ~DAOAccess() {close();}

  bool    open(TCchar* path);             // Database
  void    close();                        // Close Database
  String& getPath() {return path;}

  friend class DAOtables;
  };

