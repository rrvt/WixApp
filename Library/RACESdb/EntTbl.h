// Entity Table


#pragma once
#include "EntSet.h"
#include "ExpandableP.h"
#include "IterT.h"


class EntRcd {
int   id;
bool  dirty;
bool  remove;

public:

String firstName;
String middleInitial;
String lastName;
String suffix;
int    addrID;
int    cityStID;
BOOL   addrIsPO;
String locationZip;
String eMail;
String phone1;
String phone2;

  EntRcd() : id(0), dirty(false), remove(false) { }
 ~EntRcd() { }

  void load(EntSet* set);

  int getId() {return id;}

  void setDirty()  {dirty = true;}
  void setRemove() {dirty = true; remove = true;}

  void store(EntSet& set);
  void add(  EntSet& set);

  void display();

  // Needed for Insertion Sort of Primary Key
  bool operator== (EntRcd& r) {return id == r.id;}
  bool operator>= (EntRcd& r) {return id >= r.id;}

  // Needed for Binary Search of Primary Key
  bool operator== (long id) {return this->id == id;}
  bool operator<  (long id) {return this->id <  id;}
  bool operator>  (long id) {return this->id >  id;}

  // Needed for Linear Search with one or more arguments
  bool contains(TCchar* firstName, TCchar* lastName, TCchar* phone1) {
    return this->firstName == firstName && this->lastName == lastName && this->phone1 == phone1;
    }

private:

  void copy(EntSet& set);

  friend class EntTbl;
  };


// Record Pointer Declaration, see ExpandableP.h for details
typedef DatumPtrT<EntRcd, int> EntRcdP;

// Iterator Declaration, see IterT.h for details
class EntTbl;
typedef IterT<EntTbl, EntRcd> EntIter;


class EntTbl {

ExpandableP<EntRcd, int, EntRcdP, 2> data;

int    maxID;
EntSet entSet;

public:

String name;

  EntTbl() : maxID(0) { }
 ~EntTbl() {clear();}

  void clear() {data.clear();}

  bool load(TCchar* path);      // load database table into memory

  EntRcd* add(EntRcd& rcd);     // Add a new record to table and database

  bool store(TCchar* path);     // Store/Del entities marked

  EntRcd* find(int id) {return data.bSearch(id);}
  EntRcd* find(TCchar* firstName, TCchar* lastName, TCchar* phone1);

  virtual void display();

private:

  bool open(TCchar* path) {return entSet.open(path);}
  void close() {entSet.close();}

  void setTabs();

  // returns either a pointer to data (or datum) at index i in array or zero

  EntRcd* datum(int i) {return 0 <= i && i < nData() ? data[i].p : 0;}

  int   nData()      {return data.end();}   // returns number of data items in array

  void  removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

  friend typename EntIter;
  };


extern EntTbl entTbl;

