// A List Table Descriptors


#include "stdafx.h"
#include "TblList.h"


TblList tblList;


void TblList::add(String& accessName, MapBase* table) {

  if (find(accessName)) return;

  TblDsc& p = data.nextData();  p.accName = accessName; p.mapTable = table;
  }


TblDsc* TblList::find(String& accessName) {
TLIter  iter(*this);
TblDsc* p;

  for (p = iter(); p; p = iter++) {if (p->accName == accessName) return p;}

  return 0;
  }

