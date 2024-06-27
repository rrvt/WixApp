// Utility Functions


#include "pch.h"
#include "WixUtilities.h"
#include "filename.h"
#include "FilePaths.h"
#include "MessageBox.h"


static String replaceNonWixCh(String& s);


String removeBlnks(String& s) {
int    n = s.length();
int    i;
String t;

  for (i = 0; i < n; i++) {
    Tchar& ch = s[i];

    if (ch == _T('\\') || ch == _T('/')) {t += _T('_'); continue;}
    if (ch > _T(' ')) t += ch;
    }

  return t;
  }


String cleanName(String& s) {
int    n = s.length();
int    i;
String t;

  for (i = 0; i < n; i++) {
    Tchar& ch = s[i];

    t += ch == _T('\\') || ch == _T('/') ? _T('.') : ch;
    }

  return t;
  }




String getWixID(String& id, TCchar* ext)
        {String s = replaceNonWixCh(id);   if (!s.isEmpty()) {s += _T("."); s += ext;}   return s;}


String replaceNonWixCh(String& s) {
int    n = s.length();
int    i;
String t;

  for (i = 0; i < n; i++) {
    Tchar ch = s[i];

    if (i == 0) {
      if (_T('A') <= ch && ch <= _T('Z')) {t = ch;      continue;}
      if (_T('a') <= ch && ch <= _T('z')) {t = ch;      continue;}
      if (ch == _T('_'))                  {t = ch;      continue;}
      continue;
      }
    if (_T('A') <= ch  && ch <= _T('Z')) {t += ch;      continue;}
    if (_T('a') <= ch  && ch <= _T('z')) {t += ch;      continue;}
    if (_T('0') <= ch  && ch <= _T('9')) {t += ch;      continue;}
    if (ch == _T('_'))                   {t += ch;      continue;}
    if (ch == _T('.'))                   {t += ch;      continue;}
    if (ch == _T('\\') || ch == _T('/')) {t += _T('_'); continue;}
    if (ch == _T('-'))                   {t += _T('_'); continue;}
    if (ch == _T(' '))                   {t += _T("_"); continue;}
    }
  return t;
  }


void findLastName(String& path, String& parent, String& name) {
String s   = path;
int    lng = s.length();
Tchar  ch  = s.find(_T('/')) >= 0 ? _T('/') : _T('\\');
int    pos;

  while (s[0] == ch) s = s.substr(1);

  pos = s.find(ch);
  if (pos == s.length() - 1) {parent.clear(); name = s.substr(0, pos); return;}

  pos = s.findLastOf(ch);
  if (pos > 0) {name = s.substr(pos+1);  parent = s.substr(0, pos);   return;}

  parent.clear();  name = s;
  }


String combineNames(String& parent, String& name) {
String s;

  if (!parent.isEmpty()) s = parent + _T('\\');
  s += name;  return s;
  }



String findExeFile(TCchar* name) {
FilePaths fp;
FPsIter   iter(fp);
String*   path;

  fp.findFiles(_T("C:\\Program Files (x86)\\"), name);

  for (path = iter(); path; ) return *path;

  fp.findFiles(_T("C:"), _T("Program Files*"), name);

  for (path = iter(); path; ) return *path;

  fp.findFiles(_T("D:\\Sources\\"), name);

  for (path = iter(); path; ) return *path;

  String t; return t;
  }


String getModulePath() {
Tchar fullPath[MAX_PATH];

  if (GetModuleFileName(NULL, fullPath, noElements(fullPath))) return getPath(fullPath);
  return _T("");
  }



