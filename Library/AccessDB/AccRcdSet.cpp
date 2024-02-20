// Part of the infastructure of examining tables and fields.


#include "pch.h"
#include "AccRcdSet.h"

IMPLEMENT_DYNAMIC(AccRcdSet, CRecordset)


void AccRcdSet::AllocAndCacheFieldInfo() {                    // Mark
RETCODE nRetCode;
SWORD   nActualLen;    // Cache the number of result columns.
LPSTR   fieldName;

  m_nResultCols = -1;

  ASSERT(GetODBCFieldCount() < 0);

  if (m_rgODBCFieldInfos != NULL) {delete [] m_rgODBCFieldInfos;   m_rgODBCFieldInfos = NULL;}

  AFX_ODBC_CALL(::SQLNumResultCols(m_hstmt, &m_nResultCols));

  if (!Check(nRetCode)) {
    TRACE(_T("Error: Can't get field info.\n"));
    AfxThrowDBException(nRetCode, m_pDatabase,m_hstmt);
    }

  // If there are no fields, quit now

  if (m_nResultCols == 0) return;

  // Allocate buffer and get the ODBC meta data.
  m_rgODBCFieldInfos = new CODBCFieldInfo[m_nResultCols];

  fieldName = new char[MAX_FNAME_LEN + 1];              // Temporarary storage for non-UNICODE name.

  for (WORD i = 0; i < GetODBCFieldCount(); i++) {      // Get the field info for each field.

    AFX_ODBC_CALL(
      ::SQLDescribeCol(m_hstmt, i + 1,
                       (SQLWCHAR*) fieldName, MAX_FNAME_LEN, &nActualLen,
                       &m_rgODBCFieldInfos[i].m_nSQLType,
                       &m_rgODBCFieldInfos[i].m_nPrecision,
                       &m_rgODBCFieldInfos[i].m_nScale,
                       &m_rgODBCFieldInfos[i].m_nNullability
                       )
      );

    m_rgODBCFieldInfos[i].m_strName = fieldName;

    if (!Check(nRetCode)) {
      TRACE1("Error: ODBC failure getting field #%d info.\n", i + 1);
        AfxThrowDBException(nRetCode, m_pDatabase, m_hstmt);
      }
    }

  delete[] fieldName;
  }


void AccRcdSet::CheckRowsetError(RETCODE nRetCode) {
  switch (nRetCode) {
    case SQL_SUCCESS          : result = SuccessSet;           break;
    case SQL_SUCCESS_WITH_INFO: result = SuccesWithInfoSet;    break;
    case SQL_NO_DATA_FOUND    : result = NoDataSet;            break;
    case SQL_ERROR            : result = ErrorSet;             break;
    case SQL_INVALID_HANDLE   : result = InvalidHandleSet;     break;
    case SQL_STILL_EXECUTING  : result = StillExecutingSet;    break;
    default                   : result = (SetResult) nRetCode; break;
    }
  }


void AccRcdSet::close() {

  if (m_rgODBCFieldInfos) {delete[] m_rgODBCFieldInfos;   m_rgODBCFieldInfos = 0;}
  if (m_rgRowStatus)      {delete[] m_rgRowStatus;        m_rgRowStatus      = 0;}

  try {Close();} catch (...) { }

  if (!opened) return;
  try {if (m_rgRowStatus) FreeRowset();   if (m_rgFieldInfos) FreeDataCache();} catch (...) { }

  opened = false;
  }

