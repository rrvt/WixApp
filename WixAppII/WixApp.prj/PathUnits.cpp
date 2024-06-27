// Parse a path into an array


#include "pch.h"
#include "PathUnits.h"


PathUnits& PathUnits::operator= (String& path) {parse(path);   return *this;}


// disect the path to a file into bite size chunks, mostly directories (e.g. D:\, \abc\, etc.)

void PathUnits::parse(const String& path) {
String t = path;
int    end;

  units.clear();

  while ((end = t.find(_T('\\'))) > 0)
                                {String s = t.substr(0, end);   units += s;   t = t.substr(end+1);}
  }


PathUnits::operator String() {
int n = units.end();
int i;

  tempPath.clear();

  for (i = 0; i < n; i++) tempPath += units[i] + _T('\\');

  return tempPath;
  }


PathUnits& PathUnits::copy(PathUnits& p) {
int n = units.end();
int j;

  tempPath = p.tempPath;

  for (j = 0; j < n; j++) units[j] = p.units[j];

  return *this;
  }




