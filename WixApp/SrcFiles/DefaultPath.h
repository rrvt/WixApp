// Manage Default (i.e. last used) Paths


#pragma once
#include "Expandable.h"


enum DefPathDscType {NilType, StgType, IntType};


struct DefPathDsc {
DefPathDscType dscType;
String         key;
String         path;
int            val;

  DefPathDsc() : val(0), dscType(NilType) {}
 ~DefPathDsc() { }
  };


class DefaultPath {

int                        nPaths;
Expandable <DefPathDsc, 1> paths;

public:

  DefaultPath() : nPaths(0) {}
 ~DefaultPath() { }

  TCchar* add(   TCchar* key, String& fullPath);
  int     add(   TCchar* key, int     v);
  TCchar* get(   TCchar* key);
  int     getInt(TCchar* key);

  void    writeWixData();

private:

  DefPathDsc* find(TCchar* key);
  };


extern DefaultPath defaultPath;

