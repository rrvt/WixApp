// Defines an iterator to see all the tables in the database and an descriptor that shows the details
// of a single table in the database



#include "pch.h"
#include "AccTblDsc.h"
#include "AccessDB.h"

IMPLEMENT_DYNAMIC(AccTblDsc, AccRcdSet)


AccTblDsc::AccTblDsc() : AccRcdSet(accessDB.db()) {    // Mark
  qualifier = _T("");
  owner     = _T("");
  name      = _T("");
  type      = _T("");
  remarks        = _T("");
  m_nFields = 5;
  }


bool AccTblDsc::open(TCchar* path) {
RETCODE nRetCode;
UWORD   bFunctionExists;

  if (!accessDB.isOpen() && !accessDB.open(path)) return false;

  // Make sure SQLTables exists
  nRetCode = ::SQLGetFunctions(m_pDatabase->m_hdbc, SQL_API_SQLTABLES,&bFunctionExists);

  if (!Check(nRetCode)) return false;

  if (!bFunctionExists) return false;

  SetState(CRecordset::dynaset, NULL, readOnly);                 // Cache state info and allocate hstmt

  if (!AllocHstmt()) return false;

  TRY {
    OnSetOptions(m_hstmt);   AllocStatusArrays();

    // Call the ODBC function
    AFX_ODBC_CALL(::SQLTables(m_hstmt, 0, SQL_NTS, 0, SQL_NTS, 0, SQL_NTS, 0, SQL_NTS));

    if (!Check(nRetCode)) return false;

    AllocAndCacheFieldInfo();   AllocRowset();              // Allocate memory and cache info
    }

  CATCH_ALL(e) {Close(); return false;}

  END_CATCH_ALL

  return true;
  }


void AccTblDsc::DoFieldExchange(CFieldExchange* pFX) {     // Mark

  pFX->SetFieldType(CFieldExchange::outputColumn);

  RFX_Text(pFX, _T("TABLE_QUALIFIER"), qualifier);
  RFX_Text(pFX, _T("TABLE_OWNER"),     owner);
  RFX_Text(pFX, _T("TABLE_NAME"),      name);
  RFX_Text(pFX, _T("TABLE_TYPE"),      type);
  RFX_Text(pFX, _T("REMARKS"),         remarks);
  }



