// Parse a path into an array


#pragma once
#include "Expandable.h"

class PathUnits {
int                     i;
int                     nUnits;                 // No. of units in the path (e.g. D:\, \abc\, etc.)
Expandable <String, 16> units;
String                  tempPath;

public:

  PathUnits() : nUnits(0) {}
  PathUnits(const String& fullPath) {parse(fullPath);}
  PathUnits(PathUnits& p) {copy(p);}
 ~PathUnits() {}

  PathUnits& operator= (PathUnits& p) {return copy(p);}

  void clear() {nUnits = 0;}
  bool empty() {return nUnits == 0;}

  PathUnits& operator= (String& path);
             operator String();
//             operator CString();

//  bool    relativePath(PathUnits& pathUnit, String& rel);
  String& getPath();
  String& getAbsolutePath();

  String* startLoop() {i = -1; return nextUnit();}
  String* nextUnit()  {i++; return i < nUnits ? &units[i] : 0;}
  int     noUnits() {return nUnits;}

  String* operator[] (int i) {return i < nUnits ? &units[i] : 0;}

private:

  void    parse(const String& path);
  PathUnits& copy(PathUnits& p);
  };



