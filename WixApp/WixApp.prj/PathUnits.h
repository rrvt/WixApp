// Parse a path into an array


#pragma once
#include "Expandable.h"
#include "IterT.h"


class PathUnits;
typedef IterT<PathUnits, String> PUIter;


class PathUnits {
Expandable <String, 16> units;
String                  tempPath;

public:

  PathUnits() { }
  PathUnits(const String& fullPath) {parse(fullPath);}
  PathUnits(PathUnits& p) {copy(p);}
 ~PathUnits() { }

  PathUnits& operator= (PathUnits& p) {return copy(p);}

  void       clear()    {units.clear();}
  bool       isEmpty() {return units.end() == 0;}

  PathUnits& operator= (String& path);
             operator String();

  String&    getPath();
  String&    getAbsolutePath();

  String*    operator[] (int i) {return datum(i);}

private:

  void       parse(const String& path);
  PathUnits& copy(PathUnits& p);

  // returns either a pointer to data (or datum) at index i in array or zero
  String* datum(int i) {return 0 <= i && i < nData() ? &units[i] : 0;}

public:

  // returns number of data items in array
  int   nData()      {return units.end();}

  friend typename PUIter;
  };



