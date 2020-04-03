// A List Table Descriptors


#include "stdafx.h"
#include "TableDscrs.h"


TableDscrs tableDscrs;





void TableDscrs::add(String& name, MapTable* table, Maps* mps) {
TableDsc* p;

  if (find(name)) return;

  p = &dscrs[dscrs.end()];  p->name = name; p->mapTable = table;  p->maps = mps;
  }


TableDsc* TableDscrs::find(String& name) {
int       i;
TableDsc* p;

  for (i = 0; i < dscrs.end(); i++) {p = &dscrs[i];  if (p->name == name) return p;}

  return 0;
  }

