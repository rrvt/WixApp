// AssgnPref Table


#pragma once
#include "AsnSet.h"
#include "ExpandableP.h"
#include "IterT.h"


class AsnRcd {
int   id;
bool  dirty;
bool  remove;

public:

String aPKey;
String txt;

  AsnRcd() : id(0), dirty(false), remove(false) { }
 ~AsnRcd() { }

  void load(AsnSet* set);

  int getId() {return id;}

  void setDirty()  {dirty = true;}
  void setRemove() {dirty = true; remove = true;}

  void store(AsnSet& set);
  void add(  AsnSet& set);

  void display();

  // Needed for Insertion Sort of Primary Key
  bool operator== (AsnRcd& r) {return id == r.id;}
  bool operator>= (AsnRcd& r) {return id >= r.id;}

  // Needed for Binary Search of Primary Key
  bool operator== (long id) {return this->id == id;}
  bool operator<  (long id) {return this->id <  id;}
  bool operator>  (long id) {return this->id >  id;}

  // Needed for Linear Search with one or more arguments
  bool contains(TCchar* aPKey) {
    return this->aPKey == aPKey;
    }

private:

  void copy(AsnSet& set);

  friend class AsnTbl;
  };


// Record Pointer Declaration, see ExpandableP.h for details
typedef DatumPtrT<AsnRcd, int> AsnRcdP;

// Iterator Declaration, see IterT.h for details
class AsnTbl;
typedef IterT<AsnTbl, AsnRcd> AsnIter;


class AsnTbl {

ExpandableP<AsnRcd, int, AsnRcdP, 2> data;

int    maxID;
AsnSet asnSet;

public:

String name;

  AsnTbl() : maxID(0) { }
 ~AsnTbl() {clear();}

  void clear() {data.clear();}

  bool load(TCchar* path);      // load database table into memory

  AsnRcd* add(AsnRcd& rcd);     // Add a new record to table and database

  bool store(TCchar* path);     // Store/Del entities marked

  AsnRcd* find(int id) {return data.bSearch(id);}
  AsnRcd* find(TCchar* aPKey);

  virtual void display();

private:

  bool open(TCchar* path) {return asnSet.open(path);}
  void close() {asnSet.close();}

  void setTabs();

  // returns either a pointer to data (or datum) at index i in array or zero

  AsnRcd* datum(int i) {return 0 <= i && i < nData() ? data[i].p : 0;}

  int   nData()      {return data.end();}   // returns number of data items in array

  void  removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

  friend typename AsnIter;
  };


extern AsnTbl asnTbl;

