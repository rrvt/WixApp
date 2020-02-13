// The one object that describes the current Solution Path


#pragma once
#include "WixDataDlg.h"
#include "PathUnits.h"


class Solution {
PathUnits pathUnits;
String    name;

public:

  Solution() {}
 ~Solution() { }

  operator String() {return (String) pathUnits;}

  void newFile();
  void clear() {name.clear(); pathUnits.clear();}
  bool empty() {return pathUnits.empty();}

  void loadEB(WixDataDlg& dialog);

//void getRelSolution(String& path, String& varPath);
  void readWixData();
  void writeWixData();

  String* startLoop()  {return pathUnits.startLoop();}
  String* nextSubDir() {return pathUnits.nextUnit();}
  int     noUnits()    {return pathUnits.noUnits();}

  String* operator[](int i) {return pathUnits[i];}
  };


extern Solution solution;
