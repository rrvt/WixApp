// The one object that describes the current Solution Path


#pragma once
#include "WixDataDlg.h"
#include "PathUnits.h"


class Solution {
PathUnits pathUnits;
String    version;

public:
PUIter    iter;
String    name;

  Solution() : iter(pathUnits) { }
 ~Solution() { }

  String&  getVersion() {return version;}

  operator String() {return (String) pathUnits;}

  void     newFile();
  void     clear() {name.clear(); pathUnits.clear();}
  bool     isEmpty() {return pathUnits.isEmpty();}

  void     readWixData();
  void     writeWixData();

  int      noUnits()    {return pathUnits.nData();}

  String*  operator[](int i) {return pathUnits[i];}
  };


extern Solution solution;
