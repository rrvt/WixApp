// A List Table Descriptors


#pragma once
#include "MapData.h"
#include "MapTable.h"


struct TableDsc {
String    name;
MapTable* mapTable;
Maps*     maps;
bool      selected;

  TableDsc() : mapTable(0), maps(0), selected(false)  {}
  TableDsc(TableDsc& fd) {copy(fd, *this);}

 ~TableDsc() {}

  TableDsc& operator=(TableDsc& fd) {copy(fd, *this); return *this;}

  private:

  void copy(TableDsc& src, TableDsc& dst)
    {dst.name = src.name; dst.mapTable = src.mapTable; dst.maps = src.maps; dst.selected = src.selected;}
  };


class TableDscrs {
int                      i;
Expandable<TableDsc, 64> dscrs;

public:

  TableDscrs() : i(0) {}
 ~TableDscrs() {}

  void      add(                 MapTable& table, Maps* mps) {add(table.name, &table, mps);}
  void      add(String&   name,  MapTable* table, Maps* mps);
  TableDsc* find(String& name);

  TableDsc* startLoop()  {i = -1; return nextEntry();}
  TableDsc* nextEntry()  {i++; return i < dscrs.end() ? &dscrs[i] : 0;}
  };


extern TableDscrs tableDscrs;
