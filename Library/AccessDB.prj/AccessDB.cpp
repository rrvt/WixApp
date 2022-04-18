

#include "stdafx.h"
#include "AccessDB.h"




AccessDB::AccessDB() : CDatabase(), loginTimeout(60), queryTimeout(60), opened(false) { }


// open a data source:

bool AccessDB::open(TCchar* path) {
String s;
DWORD  options;

  if (opened) close();

  SetLoginTimeout(loginTimeout);   SetQueryTimeout(queryTimeout);

  try {
    options = CDatabase::noOdbcDialog;

    s = _T("DSN=MS Access Database;DBQ=");  s += path;  s += _T(';');

    if (!OpenEx(s, options)) return false;
    }
  catch(CDBException* pDBE) {pDBE->Delete();   return false;}

  opened = true;   return true;
  }


// Closes the open database

void AccessDB::close() {if (opened) {Close();   opened = false;}}



#if 0
//FUNCTION: QueryOpenTables
//JOB:    opens a set which enumerates the tables in a database

bool AccessDB::openTables() {                   // Mark

  if (!opened) return setErrMsg(DbErr, _T("Database Not Opened!"));  //Check open database:

  //Was pointer initialized?
  if (!myTable) myTable = new MyTable(this);                   //Make sure we create it:

  if (myTable->IsOpen()) myTable->Close();                            //close the set if open:

  myTable->open(NULL, NULL, NULL, NULL, CRecordset::dynaset);        //dynaset, Now reopen it:

  return SetSuccessMsg();
  }


//FUNCTION: GetTables
//JOB:    returns (in the pointer) the currently open (MUST BE OPEN!) tables

bool AccessDB::GetTables(MyTable*& table) {

  if (!opened) return setErrMsg(DbErr, _T("Database Not Opened!"));      //Check open database:

  //Check if tables has even been queried:
  if (!myTable)
            return setErrMsg(MsgErr, _T("QueryOpenTables was not called to create the MyTable object!"));

  //Check if tables is open:
  if (!myTable->IsOpen()) return setErrMsg(MsgErr, _T("The tables recordset is not open!"));

  table = NULL;   table = myTable;    return SetSuccessMsg();
  }
#endif



#if 0
RecordSetEx* tempRec;
POSITION     pos;
  if (m_bInTrans) Rollback();                   //In transaction? Rollback changes:

  pos = mySets.GetStartPosition();              //Any open sets:

  for (int h = 0; h < mySets.GetCount(); h++) {

    mySets.GetNextAssoc(pos, tempStr, (void*&)tempRec);       //Close this one:

    if (tempRec->IsOpen()) tempRec->Close();

    delete tempRec;   tempRec = 0;                            //Delete the memory:

    mySets.RemoveKey(tempStr);                                //And remove it:
    }

  //Next, if the tables or columns still exist, get rid of them:

  if (myTable) {

    if (myTable->IsOpen()) myTable->Close();
    delete myTable;   myTable = NULL;
    }
#endif

#if 0
  if (myColumns) {

    if (myColumns->IsOpen()) myColumns->Close();
    delete myColumns;   myColumns = NULL;
    }
#endif

