// A List Table Descriptors


#include "stdafx.h"
#include "TableDscrs.h"


TableDscrs tableDscrs;


void TableDscrs::add(String& accessName, MapTable* table, Maps* mps) {

  if (find(accessName)) return;

  TableDsc& p = dscrs.nextData();  p.accName = accessName; p.mapTable = table;  p.maps = mps;
  }


TableDsc* TableDscrs::find(String& accessName) {
int       i;
TableDsc* p;

  for (i = 0; i < dscrs.end(); i++) {p = &dscrs[i];  if (p->accName == accessName) return p;}

  return 0;
  }

