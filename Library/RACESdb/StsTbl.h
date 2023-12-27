// Status Table


#pragma once
#include "StsSet.h"
#include "ExpandableP.h"
#include "IterT.h"


class StsRcd {
int   id;
bool  dirty;
bool  remove;

public:

String abbreviation;
String description;

  StsRcd() : id(0), dirty(false), remove(false) { }
 ~StsRcd() { }

  void load(StsSet* set);

  int getId() {return id;}

  void setDirty()  {dirty = true;}
  void setRemove() {dirty = true; remove = true;}

  void store(StsSet& set);
  void add(  StsSet& set);

  void display();

  // Needed for Insertion Sort of Primary Key
  bool operator== (StsRcd& r) {return id == r.id;}
  bool operator>= (StsRcd& r) {return id >= r.id;}

  // Needed for Binary Search of Primary Key
  bool operator== (long id) {return this->id == id;}
  bool operator<  (long id) {return this->id <  id;}
  bool operator>  (long id) {return this->id >  id;}

  // Needed for Linear Search with one or more arguments
  bool contains(TCchar* abbreviation) {
    return this->abbreviation == abbreviation;
    }

private:

  void copy(StsSet& set);

  friend class StsTbl;
  };


// Record Pointer Declaration, see ExpandableP.h for details
typedef DatumPtrT<StsRcd, int> StsRcdP;

// Iterator Declaration, see IterT.h for details
class StsTbl;
typedef IterT<StsTbl, StsRcd> StsIter;


class StsTbl {

ExpandableP<StsRcd, int, StsRcdP, 2> data;

int    maxID;
StsSet stsSet;

public:

String name;

  StsTbl() : maxID(0) { }
 ~StsTbl() {clear();}

  void clear() {data.clear();}

  bool load(TCchar* path);      // load database table into memory

  StsRcd* add(StsRcd& rcd);     // Add a new record to table and database

  bool store(TCchar* path);     // Store/Del entities marked

  StsRcd* find(int id) {return data.bSearch(id);}
  StsRcd* find(TCchar* abbreviation);

  virtual void display();

private:

  bool open(TCchar* path) {return stsSet.open(path);}
  void close() {stsSet.close();}

  void setTabs();

  // returns either a pointer to data (or datum) at index i in array or zero

  StsRcd* datum(int i) {return 0 <= i && i < nData() ? data[i].p : 0;}

  int   nData()      {return data.end();}   // returns number of data items in array

  void  removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

  friend typename StsIter;
  };


extern StsTbl stsTbl;

