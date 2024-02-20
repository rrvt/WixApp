// Database


#pragma once
#include "AccessDB.h"
#include "AdrTbl.h"
#include "AsnTbl.h"
#include "CtyTbl.h"
#include "EntTbl.h"
#include "LocTbl.h"
#include "MbrTbl.h"
#include "StsTbl.h"


class Database {
public:

  Database() { }
 ~Database();

  bool load(TCchar* path);
  };


extern Database database;

