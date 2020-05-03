// The one object that describes the current Solution Path


#pragma once
#include "WixDataDlg.h"
#include "PathUnits.h"


class Solution {
PathUnits pathUnits;
String    version;

public:

String    name;

  Solution() { }
 ~Solution() { }

  String&  getVersion() {return version;}

  operator String() {return (String) pathUnits;}

  void     newFile();
  void     clear() {name.clear(); pathUnits.clear();}
  bool     isEmpty() {return pathUnits.isEmpty();}

//  void     loadEB(WixDataDlg& dialog);

//void getRelSolution(String& path, String& varPath);
  void     readWixData();
  void     writeWixData();

  String*  startLoop()  {return pathUnits.startLoop();}
  String*  nextSubDir() {return pathUnits.nextUnit();}
  int      noUnits()    {return pathUnits.noUnits();}

  String*  operator[](int i) {return pathUnits[i];}
  };


extern Solution solution;
