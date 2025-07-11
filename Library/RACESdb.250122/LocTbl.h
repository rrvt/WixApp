// LocationPref Table


#pragma once
#include "LocSet.h"
#include "ExpandableP.h"
#include "IterT.h"


class LocRcd {
int   id;
bool  dirty;
bool  remove;

public:

String key;
String txt;

  LocRcd() : id(0), dirty(false), remove(false) { }
 ~LocRcd() { }

  void load(LocSet* set);

  int getId() {return id;}

  void setDirty()  {dirty = true;}
  void setRemove() {dirty = true; remove = true;}

  void store(LocSet& set);
  void add(  LocSet& set);

  void display();

  // Needed for Insertion Sort of Primary Key
  bool operator== (LocRcd& r) {return id == r.id;}
  bool operator>= (LocRcd& r) {return id >= r.id;}

  // Needed for Binary Search of Primary Key
  bool operator== (long id) {return this->id == id;}
  bool operator<  (long id) {return this->id <  id;}
  bool operator>  (long id) {return this->id >  id;}

  // Needed for Linear Search with one or more arguments
  bool contains(TCchar* key) {
    return this->key == key;
    }

private:

  void copy(LocSet& set);

  friend class LocTbl;
  };


// Record Pointer Declaration, see ExpandableP.h for details
typedef DatumPtrT<LocRcd, int> LocRcdP;

// Iterator Declaration, see IterT.h for details
class LocTbl;
typedef IterT<LocTbl, LocRcd> LocIter;


class LocTbl {

ExpandableP<LocRcd, int, LocRcdP, 2> data;

int    maxID;
LocSet locSet;

public:

String name;

  LocTbl() : maxID(0) { }
 ~LocTbl() {clear();}

  void clear() {data.clear();}

  bool load(TCchar* path);      // load database table into memory

  LocRcd* add(LocRcd& rcd);     // Add a new record to table and database

  bool store(TCchar* path);     // Store/Del entities marked

  LocRcd* find(int id) {return data.bSearch(id);}
  LocRcd* find(TCchar* key);

  virtual void display();

private:

  bool open(TCchar* path) {return locSet.open(path);}
  void close() {locSet.close();}

  void setTabs();

  // returns either a pointer to data (or datum) at index i in array or zero

  LocRcd* datum(int i) {return 0 <= i && i < nData() ? data[i].p : 0;}

  int   nData()      {return data.end();}   // returns number of data items in array

  void  removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

  friend typename LocIter;
  };


extern LocTbl locTbl;

