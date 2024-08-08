// Path Descriptor -- full path and short version for display


#include "pch.h"
#include "RelPath.h"
#include "DefaultPath.h"
#include "filename.h"
#include "Solution.h"
#include "WixOut.h"




KeyedPathDsc::KeyedPathDsc(TCchar* key, TCchar* ttl, TCchar* nm, TCchar* e, TCchar* pat) :
                                                      dfltKey(key), PathDlgDsc(ttl, nm, e, pat) { }


void RelPath::readWixData(TCchar* section, TCchar* key, KeyedPathDsc& dsc) {
String path;

  wxd.readString(section, key, path);

  if (!path.isEmpty()) {

    if (!PathFileExists(path) && !dsc.dfltKey.isEmpty())
                    {KeyedPathDsc pthDsc = dsc;   pthDsc.name = path;   getPath(pthDsc);   return;}
    setRelPath(path);
    }
  }


void RelPath::writeWixData(TCchar* section, TCchar* key)
                                                      {wxd.writeString(section, key, localPath());}


String& RelPath::getPath(KeyedPathDsc& dsc) {
String     defPath = defaultPath.getPath(dsc.dfltKey);
String     name    = dsc.name.isEmpty() ? defPath : dsc.name;
PathDlgDsc pathDsc(dsc.title, name, dsc.ext, dsc.pattern);
String     path;

  if (!getOpenDlg(pathDsc, path)) path = dsc.name;

  setRelPath(path);   defaultPath.save(dsc.dfltKey, path);   return relPath;
  }


// Find common components in Path with Solution path.  Base Dsc on Solution path.

void RelPath::setRelPath(const String& fullPath) {
String    pathOnly = ::getPath(fullPath);
String    fileName = removePath(fullPath);
PathUnits pathU    = pathOnly;
PUIter    iter(pathU);
String*   pathSub;
String*   solSub;
int       i;

  relPath.clear();

  if (pathOnly.find(_T(':')) < 0) {relPath = fullPath; return;}

  for (solSub = solution.iter(), pathSub = iter(), i = 0; solSub && pathSub;
       solSub = solution.iter++, pathSub = iter++, i++) {

    if (*solSub != *pathSub) break;
    }

  if (i > 0) {

    for (; solSub;  solSub  = solution.iter++) relPath += _T("..\\");

    for (; pathSub; pathSub = iter++)          relPath += *pathSub + _T('\\');

    relPath += fileName;
    }
  }


// Local full path -- Prefix with local root directory after removing ..\'s

String  RelPath::localPath() {
PathUnits units    = ::getPath(relPath);
PUIter    iter(units);
String    fileName = removePath(relPath);
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


// Product full path -- Prefix relative path with root directory variable

String RelPath::prodPath()
                  {return relPath.isEmpty() ? String(_T("")) : _T("$(var.SolutionDir)") + relPath;}


