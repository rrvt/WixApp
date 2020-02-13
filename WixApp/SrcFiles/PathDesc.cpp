// Path Descriptor -- full path and short version for display


#include "stdafx.h"
#include "PathDesc.h"
#include "filename.h"
#include "Solution.h"


// Find common components in Path with Solution path.  Base Dsc on Solution path.

PathDesc& PathDesc::operator= (const String& fullPath) {
String pathOnly = getPath(fullPath);
String fileName = removePath(fullPath);
PathUnits pathU = pathOnly;
String* pathSub;
String* solSub;
int     i;

  for (solSub = solution.startLoop(), pathSub = pathU.startLoop(), i = 0; solSub && pathSub;
       solSub = solution.nextSubDir(), pathSub = pathU.nextUnit(), i++) {

    if (*solSub != *pathSub) break;
    }

  dsc.clear();

  if (i > 0) {

    for (; solSub;  solSub  = solution.nextSubDir()) dsc += _T("..\\");

    for (; pathSub; pathSub = pathU.nextUnit())      dsc += *pathSub + _T('\\');

    dsc += fileName;
    }

  return *this;
  }


String  PathDesc::full() {
PathUnits units    = getPath(dsc);
String    fileName = removePath(dsc);
String    path;
String*   sub;
int       i;
int       j;
int       n = solution.noUnits();

  if (!dsc.empty()) {

    for (sub = units.startLoop(), i = 0; sub; sub = units.nextUnit(), i++) if (*sub != _T("..")) break;

    for (j = 0, sub = solution.startLoop(); j < n - i && sub;
                                                   j++, sub = solution.nextSubDir()) path += *sub + _T('\\');

    for (i, sub = units[i]; sub; i++, sub = units[i]) path += *sub + _T('\\');

    path += fileName;
    }

  return path;
  }




#if 0
int    pos      = pathOnly.findLast(_T('\\'), pathOnly.length()-2);
String prefix;

  if (pos >= 0) prefix = pathOnly.substr(pos);

  if (!prefix.empty() || !fileName.empty()) dsc = _T("...") + prefix + fileName;
#endif

