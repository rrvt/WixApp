// Base Class for all map table classes
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.

#pragma once
#include "AceDao.h"
#include <map>


extern TCchar* FileSection;


class MapRecord {
bool dirty;
bool delRcd;
//bool newRcd;

public:
           MapRecord()                   : dirty(false),   delRcd(false) {};  // ,    newRcd(false)
           MapRecord(const MapRecord& r) : dirty(r.dirty), delRcd(r.delRcd) {}  // , newRcd(r.newRcd)

  virtual ~MapRecord()    = 0;

      void clearMarks() {dirty  = delRcd = false;}
      void mark()       {dirty  = true;}
      void del()        {delRcd = true;}
      bool isDirty()    {return dirty;}
      bool toDelete()   {return delRcd;}

  MapRecord& operator= (const MapRecord& r) {copy(r, *this); return *this;}

  void copy(const MapRecord& src, MapRecord& dst)
                            {dst.dirty = src.dirty; dst.delRcd = src.delRcd;} // dst.newRcd = src.newRcd;

  virtual  String getFldVal(int i) {return _T("");}


//  virtual  void write(ArchiveIO& ario) {}
//  virtual  void read( ArchiveIO& ario) {}
  };


class MapTable {

public:

String name;

           MapTable() {};
           MapTable(const MapTable& tbl) : name(tbl.name) {}
  virtual ~MapTable() {};

  virtual void       initialize() = 0;
          void       initialize(TCchar* name) {this->name = name;}

          MapTable&  operator= (const MapTable& tbl) {name = tbl.name; return *this;}

  virtual MapRecord* find(const long  id) {return 0;};
  virtual MapRecord* find(const float id) {return 0;};

  virtual MapRecord* startLoop() {return 0;}

  virtual MapRecord* nextRecord() {return 0;}

  virtual MapRecord* prevRecord() {return 0;}

  virtual MapRecord* curRcd()  {return 0;}
  virtual long       curKey()  {return 0;}
  virtual int        curSize() {return 0;}
  virtual long       nextKey() {return 0;}


  virtual bool       toTable(AceRecordSet& records) = 0;
  virtual bool       toDatabase(MapTable& tbl)      {return false;}
  };

