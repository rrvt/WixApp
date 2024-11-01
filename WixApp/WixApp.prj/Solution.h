// The one object that describes the current Solution Path


#pragma once
#include "WixAppDlg.h"
#include "PathUnits.h"

class Archive;

extern TCchar* IniSection;
extern TCchar* RootPathKey;


class Solution {
String    rootPath;
PathUnits pathUnits;
double    ver;

public:
PUIter    iter;
String    name;

  Solution() : iter(pathUnits) { }
 ~Solution() { }

  double   getVer()     {return ver;}

  operator String() {return (String) pathUnits;}

  bool     newProject(bool isNew);
  void     clear() {name.clear(); pathUnits.clear();}
  bool     isEmpty() {return pathUnits.isEmpty();}

  void     readWixData();
  void     writeWixData();

  bool     getRootPath(String& path);
  bool     getSolutionPath(String& path);

  String&  getRootPath() {return rootPath;}

  int      noUnits()    {return pathUnits.nData();}

  String*  operator[](int i) {return pathUnits[i];}

  void     saveData(Archive& ar);

private:

  bool     readRootPath(String& path);
  void     saveRootPath(TCchar* path);

  void     getSolutionPath(TCchar* fullPath = 0);
  };


extern Solution solution;
