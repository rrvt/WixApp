// DBtablesB -- Base class with a List of Data Base Table Objects used in the App


#pragma once
#include "DAOtables.h"
#include "Expandable.h"
#include "IterT.h"


class DBrcd {
bool  dirty;
bool  delRcd;

public:

long  id;

  DBrcd() : id(0), dirty(false), delRcd(false)  { }
  DBrcd(DBrcd& r) {copy(r);}
  virtual ~DBrcd() = 0;

  void mark()      {dirty = true;}
  void clearMark() {dirty = false;}

  void del()  {delRcd = true;}

  bool isDirty()   {return dirty;}
  bool isDeleted() {return delRcd;}

  DBrcd& operator= (DBrcd& r) {copy(r); return *this;}

private:

  void copy(DBrcd& r);
  };



class DBtable {
public:

String name;

  DBtable(TCchar* nm) : name(nm) { }
  virtual ~DBtable() = 0;

  virtual void clear() { }

  virtual bool load(DAOtable* daoTable) {return false;}
  virtual void store() { }

  virtual DBrcd* add(DBrcd& rcd) {return 0;}            // Add a new record to table and database
  virtual DBrcd* add(TCchar* key, TCchar* dsc) {return 0;}

  virtual void display() = 0;

  virtual bool operator== (TCchar* name) {return false;}

private:

  DBtable() { }
  };



inline bool operator== (DBtable& n, DBtable* t) {return n.name == t->name;}
inline bool operator>= (DBtable& n, DBtable* t) {return n.name >= t->name;}


class DBtablesB;
typedef DBtable* DBtableP;
typedef IterT<DBtablesB, DBtableP> DBTiter;



class DBtablesB {
protected:

Expandable<DBtableP, 2> data;

public:

           DBtablesB() { }
  virtual ~DBtablesB() = 0;

  virtual DBtablesB& operator+= (DBtable* tbl);
  virtual DBtablesB& operator=  (DBtable* tbl);

  virtual void       clear();

  virtual bool       load(TCchar* path);

  virtual DBtable*   find(TCchar* name);
//virtual String*    primaryKey(TCchar* tableName)
//                                {DBtable* tbl = find(tableName);   return tbl ? &tbl->primaryKey : 0;}
virtual   void       display();

private:

  // returns either a pointer to data (or datum) at index i in array or zero

  DBtableP* datum(int i) {return 0 <= i && i < nData() ? &data[i] : 0;}       // or data[i].p

  int   nData()      {return data.end();}                       // returns number of data items in array

  void  removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

  friend typename DBTiter;
  };

