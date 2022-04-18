// DAO Tables -- Access to database


#pragma once
#include "DAOAccess.h"


struct DAOtable {
String    name;
TableDefP tableP;

  DAOtable() : tableP(0) {}
  DAOtable(DAOtable& daoTable) {copy(daoTable);}

  void      load(TableDefP tableDef);

  DAOtable& operator= (DAOtable& daoTable) {copy(daoTable); return *this;}

  RecordSetP openRecordSet(DAOoptions option);

  bool       operator== (DAOtable& tbl) {return name == tbl.name;}
  bool       operator>= (DAOtable& tbl) {return name >= tbl.name;}

  bool       operator== (TCchar* name) {return this->name == name;}
  bool       operator>  (TCchar* name) {return this->name >  name;}
  bool       operator<  (TCchar* name) {return this->name <  name;}

  private:

  void   copy(DAOtable& daoTable);
  };




// Define the iterator used to look at the data in the datastore.  It is here so that it can be friended

class DAOtables;
typedef IterT<DAOtables, DAOtable> DAOiter;                       // Iterator for the Store



class DAOtables {
DAOAccess               dao;
Expandable<DAOtable, 4> data;

public:

  DAOtables() { }
 ~DAOtables() { }

  bool       open(TCchar* path) {return dao.open(path) ? load() : false;}

  bool       load();

  int        count() {return data.end();}

  DAOtable*  find(   TCchar* name) {return data.find(name);}
  DAOtable*  bSearch(TCchar* name) {return data.bSearch(name);}

private:

  // returns either a pointer to data (or datum) at index i in array or zero
  DAOtable* datum(int i) {return 0 <= i && i < nData() ? &data[i] : 0;}

  // returns number of data items in array
  int   nData()      {return data.end();}

  friend typename DAOiter;
  };



extern DAOtables daoTables;


