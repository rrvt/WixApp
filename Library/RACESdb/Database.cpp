// Database


#include "pch.h"
#include "Database.h"


Database database;
AccessDB accessDB;
AdrTbl   adrTbl;
AsnTbl   asnTbl;
CtyTbl   ctyTbl;
EntTbl   entTbl;
LocTbl   locTbl;
MbrTbl   mbrTbl;
StsTbl   stsTbl;


bool Database::load(TCchar* path) {
  if (!accessDB.open(path)) return false;
  if (!adrTbl.load(path))   return false;
  if (!asnTbl.load(path))   return false;
  if (!ctyTbl.load(path))   return false;
  if (!entTbl.load(path))   return false;
  if (!locTbl.load(path))   return false;
  if (!mbrTbl.load(path))   return false;
  if (!stsTbl.load(path))   return false;

  return true;
  }


Database::~Database() {accessDB.close();}
