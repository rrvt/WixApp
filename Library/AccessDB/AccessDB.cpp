// Open an Access Database for the purposes of determining the table names and fields within the
// tables


#include "pch.h"
#include "AccessDB.h"
#include "MessageBox.h"


AccessDB::AccessDB() : CDatabase(), loginTimeout(60), queryTimeout(60), opened(false) { }


// open a data source:

bool AccessDB::open(TCchar* path) {
String s;
DWORD  options;

  if (opened) close();

  SetLoginTimeout(loginTimeout);   SetQueryTimeout(queryTimeout);

  try {
    options = CDatabase::noOdbcDialog;

    //s = _T("DSN=MS Access Database;DBQ=");  s += path;  s += _T(';');

    //“Driver={Microsoft Access Driver (*.mdb, *.accdb)};DBQ=path to mdb/accdb file"
    s = _T("Driver={Microsoft Access Driver (*.mdb, *.accdb)};DBQ=");   s += path;  s += _T(';');

    if (!OpenEx(s, options)) {messageBox(_T("OpenEx(s, options) failed"));   return false;}
    }
  catch(CDBException* pDBE) {
    s = _T("OpenEx(s, options) bounced: ");   s += pDBE->m_strError;    messageBox(s);
    pDBE->Delete();   return false;
    }

  opened = true;   return true;
  }


// Closes the open database

void AccessDB::close() {if (opened) {Free();   Close();}   opened = false;}


