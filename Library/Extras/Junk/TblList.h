// A List  Access Table Descriptors


#pragma once
#include "MapData.h"
#include "MapBase.h"


struct TblDsc {
String   accName;
MapBase* mapTable;
bool     selected;

  TblDsc() : mapTable(0), selected(false)  {}
  TblDsc(TblDsc& fd) {copy(fd, *this);}

 ~TblDsc() {}

  TblDsc& operator=(TblDsc& fd) {copy(fd, *this); return *this;}

  private:

  void copy(TblDsc& src, TblDsc& dst)
                  {dst.accName = src.accName; dst.mapTable = src.mapTable; dst.selected = src.selected;}
  };


class TblList;
typedef IterT<TblList, TblDsc> TLIter;


class TblList {
Expandable<TblDsc, 2> data;

public:

  TblList() {}
 ~TblList() {}

  void    add(                    MapBase& table) {add(table.name, &table);}
  void    add(String& accessName, MapBase* table);
  TblDsc* find(String& accessName);

private:

  // returns either a pointer to data (or datum) at index i in array or zero
  TblDsc* datum(int i) {return 0 <= i && i < nData() ? &data[i] : 0;}

  // returns number of data items in array
  int   nData()      {return data.end();}

  friend typename TLIter;
  };


extern TblList tblList;

