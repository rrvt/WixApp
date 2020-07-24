// Path Descriptor -- full path and short version for display


#include "stdafx.h"
#include "PathDesc.h"
#include "DefaultPath.h"
#include "filename.h"
#include "GetPathDlg.h"
#include "Solution.h"
#include "WixOut.h"


void PathDesc::readWixData(TCchar* section, TCchar* key) {
String path;

  wxd.readString(section, key, path);  if (!path.isEmpty()) relativeSolution(path);
  }


void PathDesc::writeWixData(TCchar* section, TCchar* key) {
  wxd.writeString(section, key, path());
  }


String& PathDesc::browse(TCchar* title, TCchar* ext, TCchar* pat) {
String defPath = defaultPath.getCurPath();
String path;

  getPathDlg(title, defPath, ext, pat, path);   relativeSolution(path);

  defaultPath.save(path);

  return relSol;
  }


// Find common components in Path with Solution path.  Base Dsc on Solution path.

void PathDesc::relativeSolution(const String& fullPath) {
String    pathOnly = getPath(fullPath);
String    fileName = removePath(fullPath);
PathUnits pathU = pathOnly;
PUIter    iter(pathU);
String*   pathSub;
String*   solSub;
int       i;

  if (pathOnly.find(_T(':')) < 0) {relSol = fullPath; return;}

  for (solSub = solution.iter(), pathSub = iter(), i = 0; solSub && pathSub;
       solSub = solution.iter++, pathSub = iter++, i++) {

    if (*solSub != *pathSub) break;
    }

  relSol.clear();

  if (i > 0) {

    for (; solSub;  solSub  = solution.iter++) relSol += _T("..\\");

    for (; pathSub; pathSub = iter++)          relSol += *pathSub + _T('\\');

    relSol += fileName;
    }
  }


String  PathDesc::path() {
PathUnits units    = getPath(relSol);
PUIter    iter(units);
String    fileName = removePath(relSol);
String    path = _T("");
String*   sub;
int       i;
int       j;
int       n = solution.noUnits();

  for (sub = iter(), i = 0; sub; sub = iter++, i++) if (*sub != _T("..")) break;

  for (j = 0, sub = solution.iter(); j < n - i && sub;
                                                    j++, sub = solution.iter++) path += *sub + _T('\\');

  for (i, sub = units[i]; sub; i++, sub = units[i]) path += *sub + _T('\\');

  path += fileName;

  return path;
  }




#if 0
int    pos      = pathOnly.findLast(_T('\\'), pathOnly.length()-2);
String prefix;

  if (pos >= 0) prefix = pathOnly.substr(pos);

  if (!prefix.isEmpty() || !fileName.isEmpty()) relSol = _T("...") + prefix + fileName;
#endif

