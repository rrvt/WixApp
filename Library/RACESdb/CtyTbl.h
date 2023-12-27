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

  CtyRcd() : id(0), dirty(false), remove(false) { }
 ~CtyRcd() { }

  void load(CtySet* set);

  int getId() {return id;}

  void setDirty()  {dirty = true;}
  void setRemove() {dirty = true; remove = true;}

  void store(CtySet& set);
  void add(  CtySet& set);

  void display();

  // Needed for Insertion Sort of Primary Key
  bool operator== (CtyRcd& r) {return id == r.id;}
  bool operator>= (CtyRcd& r) {return id >= r.id;}

  // Needed for Binary Search of Primary Key
  bool operator== (long id) {return this->id == id;}
  bool operator<  (long id) {return this->id <  id;}
  bool operator>  (long id) {return this->id >  id;}

  // Needed for Linear Search with one or more arguments
  bool contains(TCchar* city, TCchar* state, TCchar* zip) {
    return this->city == city && this->state == state && this->zip == zip;
    }

private:

  void copy(CtySet& set);

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

  CtyRcd* find(int id) {return data.bSearch(id);}
  CtyRcd* find(TCchar* city, TCchar* state, TCchar* zip);

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

