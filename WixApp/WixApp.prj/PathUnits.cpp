// Parse a path into an array


#include "stdafx.h"
#include "PathUnits.h"


PathUnits& PathUnits::operator= (String& path) {
  parse(path);   return *this;
  }


// disect the path to a file into bite size chunks, mostly directories (e.g. D:\, \abc\, etc.)

void PathUnits::parse(const String& path) {
String t = path;
int    end;

  units.clear();

  while ((end = t.find(_T('\\'))) > 0) {String s = t.substr(0, end);   units += s;   t = t.substr(end+1);}
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



#if 0
PathUnits::operator CString() {
static String s = *this;        return s.str();
//8static CString cs = s.str();  return cs;
  }


bool PathUnits::relativePath(PathUnits& pathUnit, String& rel) {
int i;
int j;

  rel.clear();

  if (nUnits <= 0 || pathUnit.nUnits <= 0) return false;

  for (i = 0; i < nUnits && i < pathUnit.nUnits; i++) if (units[i] != pathUnit.units[i]) break;

  if (!i) return false;

  if (i >= pathUnit.nUnits) return true;

  for (j = i; j < nUnits; j++) rel += "..\\";

  for (j = i; j < pathUnit.nUnits; j++) rel += pathUnit.units[j] + "\\";

  return true;
  }
#endif

