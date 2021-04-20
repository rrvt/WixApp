// Base Class for all map table classes
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.

#pragma once
#include "AceDao.h"
#include <map>


extern TCchar* FileSection;


class MapRecord {
bool dirty;
bool delRcd;

public:

  virtual ~MapRecord()    = 0;

  void clearMarks() {dirty  = delRcd = false;}
  void mark()       {dirty  = true;}
  void del()        {delRcd = true;}
  bool isDirty()    {return dirty;}
  bool toDelete()   {return delRcd;}

protected:

  MapRecord()                   : dirty(false),   delRcd(false) {}
  MapRecord(const MapRecord& r) : dirty(r.dirty), delRcd(r.delRcd) {}

  MapRecord& operator= (const MapRecord& r) {copy(r); return *this;}

  void copy(const MapRecord& src) {dirty = src.dirty; delRcd = delRcd;}

  virtual  String getFldVal(int i) {return _T("");}
  };


class MapBase {

long maxKey;

public:

String name;

           MapBase() : maxKey(0) {};
           MapBase(const MapBase& tbl) : maxKey(0), name(tbl.name) {}
  virtual ~MapBase() {};

  virtual void       initialize() = 0;
          void       initialize(TCchar* name) {this->name = name; maxKey = 0;}

          MapBase&  operator= (const MapBase& tbl) {name = tbl.name; return *this;}

  virtual MapRecord* find(const long  id) {return 0;};
  virtual MapRecord* find(const float id) {return 0;};

  virtual long       add(TCchar* key, TCchar* dsc) {return 0;}

  virtual int        curSize() = 0;
  virtual void       setKey(long key) {if (key > maxKey) maxKey = key;}
  virtual long       nextKey() {return maxKey+1;}

  virtual bool       toTable(AceRecordSet& records) = 0;
  virtual bool       toDatabase(MapBase& tbl)      {return false;}
  };

