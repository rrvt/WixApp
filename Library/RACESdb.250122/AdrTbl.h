// Address Table


#pragma once
#include "AdrSet.h"
#include "ExpandableP.h"
#include "IterT.h"


class AdrRcd {
int   id;
bool  dirty;
bool  remove;

public:

String address1;
String address2;

  AdrRcd() : id(0), dirty(false), remove(false) { }
 ~AdrRcd() { }

  void load(AdrSet* set);

  int getId() {return id;}

  void setDirty()  {dirty = true;}
  void setRemove() {dirty = true; remove = true;}

  void store(AdrSet& set);
  void add(  AdrSet& set);

  void display();

  // Needed for Insertion Sort of Primary Key
  bool operator== (AdrRcd& r) {return id == r.id;}
  bool operator>= (AdrRcd& r) {return id >= r.id;}

  // Needed for Binary Search of Primary Key
  bool operator== (long id) {return this->id == id;}
  bool operator<  (long id) {return this->id <  id;}
  bool operator>  (long id) {return this->id >  id;}

  // Needed for Linear Search with one or more arguments
  bool contains(TCchar* address1, TCchar* address2) {
    return this->address1 == address1 && this->address2 == address2;
    }

private:

  void copy(AdrSet& set);

  friend class AdrTbl;
  };


// Record Pointer Declaration, see ExpandableP.h for details
typedef DatumPtrT<AdrRcd, int> AdrRcdP;

// Iterator Declaration, see IterT.h for details
class AdrTbl;
typedef IterT<AdrTbl, AdrRcd> AdrIter;


class AdrTbl {

ExpandableP<AdrRcd, int, AdrRcdP, 2> data;

int    maxID;
AdrSet adrSet;

public:

String name;

  AdrTbl() : maxID(0) { }
 ~AdrTbl() {clear();}

  void clear() {data.clear();}

  bool load(TCchar* path);      // load database table into memory

  AdrRcd* add(AdrRcd& rcd);     // Add a new record to table and database

  bool store(TCchar* path);     // Store/Del entities marked

  AdrRcd* find(int id) {return data.bSearch(id);}
  AdrRcd* find(TCchar* address1, TCchar* address2);

  virtual void display();

private:

  bool open(TCchar* path) {return adrSet.open(path);}
  void close() {adrSet.close();}

  void setTabs();

  // returns either a pointer to data (or datum) at index i in array or zero

  AdrRcd* datum(int i) {return 0 <= i && i < nData() ? data[i].p : 0;}

  int   nData()      {return data.end();}   // returns number of data items in array

  void  removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

  friend typename AdrIter;
  };


extern AdrTbl adrTbl;

