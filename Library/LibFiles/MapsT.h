// Maps Template


#pragma once
#include "AceDao.h"
#include "MapTable.h"


struct TableDsc;


template <class MapData>
class MapsT {
public:
MapData data;

  MapsT() : data() {}
 ~MapsT() {}

//void initialize() {data.initialize();}

  bool initializeMaps( TCchar* key,   String& path);

  void dspPath(        TCchar* title, String& path);

  void loadAllMaps();
  void loadRecords(String& tableName);
  bool loadRecords(TableDsc* tableDsc);
  bool findTable(  String& tableName, TableDsc*& tableDsc);
  bool openRcdSet(TCchar* name, DaoOptions option, AceRecordSet& rcdSet)
                                                          {return data.openRcdSet(name, option, rcdSet);}
  bool openFldDscs(TCchar* name, AceFieldNames& fldDscs) {return data.openFldDscs(name, fldDscs);}
  };


class MapData;
typedef MapsT<MapData> Maps;
#include "TableDscrs.h"


template <class MapData>
bool MapsT<MapData>::initializeMaps(TCchar* key, String& path) {

  if (!data.openDB(path)) return false;

  data.initializeMaps(this);

  dspPath(key, path);   loadAllMaps();   return true;
  }


template <class MapData>
void MapsT<MapData>::dspPath(TCchar* title, String& path)
                                                {notePad << title << _T(":") << nTab << path << nCrlf;}


template <class MapData>
void MapsT<MapData>::loadAllMaps() {
TDIter    iter(tableDscrs);
TableDsc* dsc;

  for (dsc = iter(); dsc; dsc = iter++) loadRecords(dsc);
  }



template <class MapData>
void MapsT<MapData>::loadRecords(String& tableName) {
TableDsc* tableDsc;

  if (findTable(m, tableName, tableDsc)) loadRecords(m, tableDsc);
  }


template <class MapData>
bool MapsT<MapData>::loadRecords(TableDsc* tblDsc) {
MapTable*    mapTable = tblDsc->mapTable;          if (!mapTable) return false;
AceRecordSet records;
bool         rslt;
int          count;

  if (!data.openRcdSet(tblDsc->accName, DaoReadOnly, records)) return false;

  notePad << nSetRTab(25) << nSetRTab(33) << tblDsc->accName;

  for (rslt = records.startLoop(), count = 0; rslt; rslt = records.nextRecord(), count++)
                                                                              mapTable->toTable(records);
  notePad << nTab << count << nCrlf << nClrTabs; return true;
  }


template <class MapData>
bool MapsT<MapData>::findTable(String& tableName, TableDsc*& tableDsc) {
  nameToTable = data.registerTableClass.find(tableName);      return nameToTable != 0;
  }

