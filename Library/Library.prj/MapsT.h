// Maps Template


#pragma once
#include "AceDao.h"
#include "MapBase.h"


struct TblDsc;


template <class MapData>
class MapsT {
MapData data;
public:

  MapsT() : data() {}
 ~MapsT() {}

 bool openDB(TCchar* path) {return data.openDB(path);}

 bool initializeMaps(TCchar* key, String& path);
 void loadMaps()           {data.initializeMaps();   loadAllMaps();}

 bool openRcdSet(TCchar* name, DaoOptions option, AceRecordSet& rcdSet)
                                                         {return data.openRcdSet(name, option, rcdSet);}

 bool openFldDscs(TCchar* name, AceFieldNames& fldDscs) {return data.openFldDscs(name, fldDscs);}

private:

  void dspPath(TCchar* title, String& path);

  void loadAllMaps();
  void loadRecords(String& tableName);
  bool loadRecords(TblDsc* tableDsc);
  bool findTable(  String& tableName, TblDsc*& tableDsc);
  };


class MapData;
typedef MapsT<MapData> Maps;


#include "TblList.h"


template <class MapData>
bool MapsT<MapData>::initializeMaps(TCchar* key, String& path) {

  if (!data.openDB(path)) return false;

  data.initializeMaps();

  dspPath(key, path);   loadAllMaps();   return true;
  }



template <class MapData>
void MapsT<MapData>::dspPath(TCchar* title, String& path)
                                                {notePad << title << _T(":") << nTab << path << nCrlf;}


template <class MapData>
void MapsT<MapData>::loadAllMaps() {
TLIter  iter(tblList);
TblDsc* dsc;

  for (dsc = iter(); dsc; dsc = iter++) loadRecords(dsc);
  }



template <class MapData>
void MapsT<MapData>::loadRecords(String& tableName) {
TblDsc* tableDsc;

  if (findTable(m, tableName, tableDsc)) loadRecords(m, tableDsc);
  }


template <class MapData>
bool MapsT<MapData>::loadRecords(TblDsc* tblDsc) {
MapBase*     mapTable = tblDsc->mapTable;          if (!mapTable) return false;
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
bool MapsT<MapData>::findTable(String& tableName, TblDsc*& tableDsc) {
  nameToTable = data.registerTableClass.find(tableName);      return nameToTable != 0;
  }

