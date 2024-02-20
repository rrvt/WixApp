// Provieds an iterator for seeing all the fields in a table and a descriptor of a single field.


#include "pch.h"
#include "AccFldDsc.h"
#include "AccessDB.h"


#include "NotePad.h"


IMPLEMENT_DYNAMIC(AccFldDsc, AccRcdSet)


AccFldDsc::AccFldDsc() : AccRcdSet(accessDB.db()) {
  tblQualifier = _T("");
  tblOwner     = _T("");
  tblName      = _T("");
  name         = _T("");
  type         = 0;
  typeName     = _T("");
  precision    = 0;
  length       = 0;
  scale        = 0;
  radix        = 0;
  nullable     = 0;
  remarks      = _T("");
  m_nFields    = 12;
  }


void AccFldDsc::clear() {
  tblQualifier.clear();    tblOwner.clear();    tblName.clear();    name.clear();    type = 0;
  typeName.clear();        precision = 0;       length = 0;         scale = 0;       radix = 0;
  nullable = 0;            remarks.clear();     m_nFields    = 12;
  }


bool AccFldDsc::open(TCchar* path, TCchar* tableName) {
RETCODE nRetCode;
UWORD   bFunctionExists;

  if (!accessDB.isOpen() && !accessDB.open(path)) return false;

  clear();

  // Make sure SQLColumns is supported
  nRetCode = ::SQLGetFunctions(m_pDatabase->m_hdbc, SQL_API_SQLCOLUMNS, &bFunctionExists);

  if (!Check(nRetCode) || !bFunctionExists) return false;

  SetState(CRecordset::dynaset, NULL, CRecordset::readOnly);    // Cache state info and allocate hstmt

  if (!AllocHstmt()) return FALSE;

  try {OnSetOptions(m_hstmt);}
  catch(...) {Close(); return false;}

  try {AllocStatusArrays();}
  catch(...) {Close(); return false;}

    // Call the ODBC function
  TRY {
    AFX_ODBC_CALL(
      ::SQLColumns(m_hstmt, 0, SQL_NTS, 0, SQL_NTS, (SQLWCHAR*) tableName, SQL_NTS, 0, SQL_NTS)
      );

    if (!Check(nRetCode)) return false;

    AllocAndCacheFieldInfo();   AllocRowset();          // Allocate memory and cache info
    }
  CATCH_ALL(e) {Close(); return false;}

  END_CATCH_ALL

  return true;
  }



void AccFldDsc::DoFieldExchange(CFieldExchange* pFX) {
  pFX->SetFieldType(CFieldExchange::outputColumn);
  RFX_Text(pFX,_T("TABLE_QUALIFIER"), tblQualifier);
  RFX_Text(pFX,_T("TABLE_OWNER"),     tblOwner);
  RFX_Text(pFX,_T("TABLE_NAME"),      tblName);
  RFX_Text(pFX,_T("COLUMN_NAME"),     name);
  RFX_Int( pFX,_T("DATA_TYPE"),       type);
  RFX_Text(pFX,_T("TYPE_NAME"),       typeName);
  RFX_Long(pFX,_T("PRECISION"),       precision);
  RFX_Long(pFX,_T("LENGTH"),          length);
  RFX_Int( pFX,_T("SCALE"),           scale);
  RFX_Int( pFX,_T("RADIX"),           radix);
  RFX_Int( pFX,_T("NULLABLE"),        nullable);
  RFX_Text(pFX,_T("REMARKS"),         remarks);
  }

