// The one object that describes the current Solution Path


#pragma once
#include "WixAppDlg.h"
#include "PathUnits.h"

class Archive;

extern TCchar* CurSection;
extern TCchar* RootPathKey;

enum BaseUnit {NilBase, BaseBase, RootBase, SolBase};


class Solution {

PathUnits baseUnits;                // Directory one below App Root directory
PathUnits rootUnits;                // App Root Directory stored as units
PathUnits solUnits;                 // Solution Directory stored as units
double    ver;

public:
PUIter    iter;
String    name;

  Solution() : iter(solUnits) { }
 ~Solution() { }

  double   getVer()     {return ver;}

  operator String() {return (String) solUnits;}

  bool     newProject(bool isNew);
  void     clear() {name.clear(); solUnits.clear();}
  bool     isEmpty() {return solUnits.isEmpty();}

  void     readWixData();
  void     writeWixData();

  bool     getRootPath(String& path);
  bool     getSolutionPath(String& path);

  String   getBasePath() {return baseUnits;}
  String   getRootPath() {return rootUnits;}

  int      noUnits()    {return solUnits.nData();}

  String*  operator[](int i) {return solUnits[i];}

  void     saveData(Archive& ar);

private:

  bool     readRootPath(String& path);
  void     saveRootPath(TCchar* path);

  void     getSolutionPath(TCchar* fullPath = 0);
  };


extern Solution solution;
