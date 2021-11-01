// DBtablesB -- Base class with a List of Data Base Table Objects used in the App


#include "stdafx.h"
#include "DBtablesB.h"
#include "NotePad.h"



DBtablesB& DBtablesB::operator+= (DBtable* tbl) {data += tbl; return *this;}
DBtablesB& DBtablesB::operator=  (DBtable* tbl) {data  = tbl; return *this;}


DBtablesB::~DBtablesB() { }
DBtable::~DBtable() { }
DBrcd::~DBrcd()   { }


void DBtablesB::clear() {
DBTiter   iter(*this);
DBtableP* dsc;

  for (dsc = iter(); dsc; dsc = iter++) {DBtable* dbTable = *dsc;   dbTable->clear();}
  }


bool DBtablesB::load(TCchar* path) {
DBTiter   iter(*this);
DBtableP* dsc;

  if (!daoTables.open(path))
                          {notePad << _T("Unable to Open Database at: ") << path << nCrlf; return false;}

  notePad << path << nCrlf;

  notePad << nClrTabs << nSetRTab(14);

  clear();

  for (dsc = iter(); dsc; dsc = iter++) {
    DBtable* dbTable = *dsc;

    notePad << dbTable->name << nTab;

    DAOtable* daoTbl = daoTables.bSearch(dbTable->name);

    if (daoTbl) dbTable->load(daoTbl);
    }

  return true;
  }



DBtable* DBtablesB::find(TCchar* name) {
DBTiter   iter(*this);
DBtableP* tblP;

  for (tblP = iter(); tblP; tblP = iter++) {
    DBtable* tbl = *tblP;

    if (tbl->name == name) return tbl;
    }

  return 0;
  }



void DBtablesB::display() {
int n = data.end();
int i;

  for (i = 0; i < n; i++) {
    DBtable* tbl = data[i];

    tbl->display();
    }
  }


void DBrcd::copy(DBrcd& r) {dirty = r.dirty;   delRcd = r.delRcd;   id = r.id;}

