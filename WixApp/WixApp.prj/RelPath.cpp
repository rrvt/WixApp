// Path Descriptor -- full path and short version for display


#include "pch.h"
#include "RelPath.h"
#include "DefaultPath.h"
#include "filename.h"
#include "KeyedPathDlg.h"
#include "Solution.h"
#include "WixOut.h"



void RelPath::writeWixData(TCchar* section, TCchar* key)
                                                      {wxd.writeString(section, key, localPath());}


void RelPath::readWxdPath(TCchar* section, TCchar* key, KeyedPathDlg& dlg) {
String path;

  wxd.readString(section, key, path);

  relPath = isRelPath(path) ? path : getRelPath(path);

  if (relPath.isEmpty())            {getPathOnly(path, dlg);   return;}

  if (!PathFileExists(localPath())) {getPathOnly(localPath(), dlg);}
  }



void RelPath::readWxdData( TCchar* section, TCchar* key, KeyedPathDlg& dlg) {
String path;
String dfltPath;

  wxd.readString(section, key, path);

  relPath = isRelPath(path) ? path : getRelPath(path);

  if (relPath.isEmpty()) {
    dfltPath = defaultPath.getPath(dlg.dfltKey);
    if (!path.isEmpty()) dfltPath += getMainName(path) + _T('.') + getExtension(path);
    relPath = getRelPath(dfltPath);
    }

  if (relPath.isEmpty()) {getPath(path, dlg);   return;}

  if (!PathFileExists(localPath())) {getPath(localPath(), dlg);}
  }


String& RelPath::getPath(TCchar* path, KeyedPathDlg& pathDlg) {
String       s   = path ? path : solution.getRootPath().str();
String       ext = getExtension(path);
String       filter;
KeyedPathDlg dlg = pathDlg;

  dlg.iPath = s;   if (!ext.isEmpty()) {dlg.ext = ext;   dlg.pattern = _T("*.") + ext;}

  getPath(dlg);   return relPath;
  }



String& RelPath::getPathOnly(TCchar* path, KeyedPathDlg& pathDlg) {
String       s   = path ? path : solution.getRootPath().str();
KeyedPathDlg dlg = pathDlg;

  dlg.iPath = s;   getPath(dlg);   relPath = ::getPath(relPath);   return relPath;
  }


void RelPath::writeWxdData(TCchar* section, TCchar* key) {wxd.writeString(section, key, relPath);}


String& RelPath::getPath(KeyedPathDlg& pathDlg) {
String  defPath = defaultPath.getPath(pathDlg.dfltKey);
String  name    = pathDlg.iPath.isEmpty() ? defPath : pathDlg.iPath;
PathDlg dlg(pathDlg.title, name, pathDlg.ext, pathDlg.filterTtl, pathDlg.pattern);
String  path;

  if (!dlg.openFile(path)) path = pathDlg.iPath;

  setRelPath(path);   defaultPath.save(pathDlg.dfltKey, path);   return relPath;
  }


String& RelPath::getRelPath(String& fullPath) {
static String rPath;
String    pathOnly = ::getPath(fullPath);
String    fileName = removePath(fullPath);
PathUnits pathU    = pathOnly;
PUIter    iter(pathU);
String*   pathSub;
String*   solSub;
int       i;

  rPath.clear();

  if (pathOnly.find(_T(':')) < 0) {rPath = fullPath; return rPath;}

  for (solSub = solution.iter(), pathSub = iter(), i = 0; solSub && pathSub;
       solSub = solution.iter++, pathSub = iter++, i++) {

    if (*solSub != *pathSub) break;
    }

  if (i > 0) {

    for (; solSub;  solSub  = solution.iter++) rPath += _T("..\\");

    for (; pathSub; pathSub = iter++)          rPath += *pathSub + _T('\\');

    rPath += fileName;
    }

  return rPath;
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





//////////-----------------
#if 0
void RelPath::readWixData(TCchar* section, TCchar* key, KeyedPathDlg& dsc) {
String path;

  wxd.readString(section, key, path);

  if (!path.isEmpty()) {

    if (!PathFileExists(path) && !dsc.dfltKey.isEmpty())
                    {KeyedPathDlg pthDsc = dsc;   pthDsc.name = path;   getPath(pthDsc);   return;}
    setRelPath(path);
    }
  }
#endif

