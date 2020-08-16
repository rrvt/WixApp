// A List  Access Table Descriptors


#pragma once
#include "MapData.h"
#include "MapTable.h"


struct TableDsc {
String    accName;
MapTable* mapTable;
Maps*     maps;
bool      selected;

  TableDsc() : mapTable(0), maps(0), selected(false)  {}
  TableDsc(TableDsc& fd) {copy(fd, *this);}

 ~TableDsc() {}

  TableDsc& operator=(TableDsc& fd) {copy(fd, *this); return *this;}

  private:

  void copy(TableDsc& src, TableDsc& dst) {
    dst.accName = src.accName; dst.mapTable = src.mapTable; dst.maps = src.maps;
    dst.selected = src.selected;
    }
  };


class TableDscrs;
typedef IterT<TableDscrs, TableDsc> TDIter;


class TableDscrs {
int                      i;
Expandable<TableDsc, 2> dscrs;

public:

  TableDscrs() : i(0) {}
 ~TableDscrs() {}

  void      add(                     MapTable& table, Maps* mps) {add(table.name, &table, mps);}
  void      add(String& accessName,  MapTable* table, Maps* mps);
  TableDsc* find(String& accessName);

private:

  // returns either a pointer to data (or datum) at index i in array or zero
  TableDsc* datum(int i) {return 0 <= i && i < nData() ? &dscrs[i] : 0;}

  // returns number of data items in array
  int   nData()      {return dscrs.end();}

  friend typename TDIter;

  };


extern TableDscrs tableDscrs;

