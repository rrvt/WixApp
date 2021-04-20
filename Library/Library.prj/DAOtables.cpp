// DAO Tables -- Access to database


#include "stdafx.h"
#include "DAOtables.h"





bool DAOtables::load()  {
TableDefsP tableDefs = dao.db->GetTableDefs();   if (!tableDefs) return false;
int        n         = tableDefs->Count;
int        i;
TableDefP  tableDef;
//FieldsP    flds;
//RecordSetP recordSet;

  data.clear();

  for (i = 0; i < n; i++) {

    tableDef = tableDefs->GetItem(i);   if (!tableDef) continue;

    if (tableDef->GetAttributes()) continue;           // Non zero attribute is a hidden tableDef

    DAOtable doaTbl;  doaTbl.load(tableDef);                                 ;

    data = doaTbl;
    }

  return data.end() > 0;
  }


void DAOtable::load(TableDefP tableDef) {
  tableP = tableDef;   name = tableP->GetName();
  }


RecordSetP DAOtable::openRecordSet(DAOoptions option) {

  try {return tableP->OpenRecordset(variant_t(DAO::dbOpenTable), variant_t(option));}
  catch(ComError& e) {daoError(e);}

  return 0;
  }


void DAOtable::copy(DAOtable& src) {
  name   = src.name;
  tableP = src.tableP;
  }

