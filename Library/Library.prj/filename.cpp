/* filenames
// (C) Copyright Software Design & Engineering, 3/1/95
// All rights reserved
// rrvt 3/1/95    */


#include "stdafx.h"
#include "filename.h"
#include <io.h>
#include <sys/stat.h>


/* Local Functions    */

static void get_extension(TCchar* name, Tchar* ext, int dSize);


String getPath(TCchar* fullPath) {
String stg = fullPath;
int    pos = stg.findLastOf(_T('\\'));   if (pos >= 0) {stg.resize(pos+1);   return stg;}
       pos = stg.findLastOf(_T('/'));    if (pos >= 0)  stg.resize(pos+1);   return stg;
  }



String getExtension(TCchar* fullPath) {
String stg = fullPath;
int    pos = stg.findLastOf('.');

  return pos > 0 ? stg.substr(pos+1) : _T("");    // , 99
  }


// Removes path and extension, adds and increments a version number to main name

String incVersionNo(TCchar* filePath) {
String                  mainName = getMainName(filePath);
String::reverseIterator p;
int                     n;
int                     dig;
int                     mul = 1;
int                     ver = 0;
String                  stg;

  if (!_tcslen(filePath)) return _T("");

  for (p = mainName.rbegin(), n = 0; p != mainName.rend() && n < 2; p++, n++) {
    if ('0' <= *p && *p <= '9') {dig = mul * (*p-'0'); ver += dig; mul *= 10; continue;}
    break;
    }

  if (*p == '_' && n == 2) {
    mainName.resize(mainName.size() - n);   stg = toString(ver+1);
    if (stg.size() < 2) {stg = _T("0") + stg;}
    mainName += stg;
    }
  else mainName += _T("_00");

  return mainName;
  }


String getMainName(TCchar* fullPath) {
String fn = removePath(fullPath);
int    dotPos;

  dotPos = fn.findLastOf('.');

  if (dotPos > 0) {fn.resize(dotPos);}  return fn;
  }


// i.e. last name between \\ and \\ or eol

String getDirName(TCchar* fullPath) {
String path = fullPath;
int    pos  = path.length() - 1;    if (pos < 0) return _T("");

  if (path[pos] == _T('\\')) path = path.substr(0, pos);

  return removePath(path);
  }


String removePath(TCchar* name) {
Tchar* p = _tcschr((Tchar*) name, 0);

  while (p > name) if (*--p == pathSepChar || *p == ':' || *p == uniSepChar) return ++p;

  return name;
  }


// rename file, deleting new file if it already exists!
// return true if original file is renamed

bool renameFile(Tchar* orig, Tchar* newName) {

  if (_trename(orig, newName)) {
    if (errno == ENOENT)                          return false;
    if (errno != EACCES && errno != EEXIST)       return false;
    if (_tunlink(newName))
      {_tchmod(newName, _S_IWRITE | _S_IREAD); if (_tunlink(newName)) return false;}

    if (_trename(orig, newName)) return false;
    }

  return true;
  }



void appendSepChar(Tchar* name, int dSize) {
Tchar* r;

  r = _tcsrchr(name, 0);

  if (*(r-1) != pathSepChar && r - name + 2 < dSize) {
    *r++ = pathSepChar; *r = 0;
    }
  }


bool change_extension(String& name, TCchar* ext) {
Tchar stg[256];

  _tcscpy_s(stg, noElements(stg), name);

  if (!change_extension(stg, noElements(stg), ext)) return false;

  name = stg; return true;
  }


// Change file name extension to ext, even if extension does not exist
// name - name with or without and extension
// ext  - up to(3 extension characters, no period!
// returns:
// true when extension successfully added to name
// name - contains modified file name

bool change_extension(Tchar* name, int dSize, TCchar* ext) {
Tchar* p = 0;
  p = _tcsrchr(name, '.');
  if (p) {
    *p++ = '\0';

    if (_tcscmp(p, ext) == 0) return false;
    }
  add_extension(name, dSize, ext); return true;
  }


void add_extension(Tchar* name, int dSize, TCchar* ext) {
Tchar* backslash = _tcsrchr(name, pathSepChar);
Tchar* dot       = _tcsrchr(name, '.');

  if (dot <= backslash) {_tcscat_s(name, dSize, _T(".")); _tcscat_s(name, dSize, ext);}
  }


static void get_extension(TCchar* name, Tchar* ext, int dSize) {
TCchar* p = 0;
TCchar* q = name;

  while ((q = _tcschr(q, '.')) != 0) p = ++q;
  if (p) _tcscpy_s(ext, dSize, p); else *ext = 0;
  }



bool isExplicitExt(TCchar* fn) {
TCchar* p = _tcsrchr(fn, '.');

  if (!p) return false;

  for (p++; *p; p++) if (*p == '*' || *p == '?') return false;

  return true;
  }




void removeFile(TCchar* name) {
  try {CFile::Remove(name);} catch (CFileException* pEx) {pEx->Delete();}
  }


String noBlanks(TCchar* name) {
String stg = name;
int    pos;

  for (pos = stg.findLastOf(' '); pos >= 0; pos = stg.findLastOf(' ')) stg[pos] = '_';

  return stg;
  }


void removeOneDir(String& t) {
int lng = t.length();
int n;
int pos;
int lastPos = -1;
int i;

  for (n = 0, pos = t.find(_T('\\'), 0); pos >= 0; n++, pos = t.find(_T('\\'), ++pos)) lastPos = pos;

  if (lastPos + 1 >= lng) n--;

  for (i = 1, pos = t.find(_T('\\'), 0); i < n; i++, pos = t.find(_T('\\'), ++pos)) continue;

  if (pos > 0) t = t.substr(0, pos);    t += _T('\\');
  }

