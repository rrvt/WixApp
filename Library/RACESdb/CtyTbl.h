// CityState Table


#pragma once
#include "CtySet.h"
#include "ExpandableP.h"
#include "IterT.h"


class CtyRcd {
int   id;
bool  dirty;
bool  remove;

public:

String city;
String state;
String zip;

  CtyRcd();
  CtyRcd(CtyRcd& r) {copy(r);}
 ~CtyRcd() { }

  void clear();

  void load(CtySet* set);

  int getId() {return id;}

  void setDirty()  {dirty = true;}
  void setRemove() {dirty = true; remove = true;}
  bool isRemoved() {return remove;}

  void store(CtySet& set);
  void add(  CtySet& set);

  void display();

  CtyRcd& operator= (CtyRcd& r) {copy(r); return *this;}

  // Needed for Insertion Sort of Primary Key
  bool operator== (CtyRcd& r) {return id == r.id;}
  bool operator>= (CtyRcd& r) {return id >= r.id;}

  // Needed for Binary Search of Primary Key
  bool operator== (long id) {return this->id == id;}
  bool operator<  (long id) {return this->id <  id;}
  bool operator>  (long id) {return this->id >  id;}

  // Needed for Linear Search with one or more arguments
  bool contains(TCchar* zip) {
    return this->zip == zip;
    }

private:

  void copy(CtySet& set);
  void copy(CtyRcd& r);

  friend class CtyTbl;
  };


// Record Pointer Declaration, see ExpandableP.h for details
typedef DatumPtrT<CtyRcd, int> CtyRcdP;

// Iterator Declaration, see IterT.h for details
class CtyTbl;
typedef IterT<CtyTbl, CtyRcd> CtyIter;


class CtyTbl {

ExpandableP<CtyRcd, int, CtyRcdP, 2> data;

int    maxID;
CtySet ctySet;

public:

String name;

  CtyTbl() : maxID(0) { }
 ~CtyTbl() {clear();}

  void clear() {data.clear();}

  bool load(TCchar* path);      // load database table into memory

  CtyRcd* add(CtyRcd& rcd);     // Add a new record to table and database

  bool store(TCchar* path);     // Store/Del entities marked

  CtyRcd* find(int id) {return id ? data.bSearch(id) : 0;}
  CtyRcd* find(TCchar* zip);

  virtual void display();

private:

  bool open(TCchar* path) {return ctySet.open(path);}
  void close() {ctySet.close();}

  void setTabs();

  // returns either a pointer to data (or datum) at index i in array or zero

  CtyRcd* datum(int i) {return 0 <= i && i < nData() ? data[i].p : 0;}

  int   nData()      {return data.end();}   // returns number of data items in array

  void  removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

  friend typename CtyIter;
  };


extern CtyTbl ctyTbl;

