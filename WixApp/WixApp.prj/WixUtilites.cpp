// Utility Functions


#include "stdafx.h"
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

  pos = s.find(ch);      if (pos == s.length() - 1) {parent.clear(); name = s.substr(0, pos); return;}

  pos = s.findLastOf(ch);  if (pos > 0) {name = s.substr(pos+1);  parent = s.substr(0, pos);   return;}

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



  #if 0
// Output line provide s in not empty

void outputLine(int tab, TCchar* prefix, String& s, TCchar* suffix, FileIO& file) {
String line;

  if (s.isEmpty()) return;

  line = prefix + s + suffix + _T("\n");    outputStg(tab, line, file);
  }


void outputStg(int tab, String& s, FileIO& file) {
String line = s;
String line2;
int    i;
int    n;
Tchar   ch;
Tchar   quote     = 0;
bool   isString  = false;
int    lastSpace = -1;
int    maxLng    = 82 - tab;

  n = outCount(line);

  if (tab + n > 102) {
    for (i = 0; i < maxLng; i++) {
      ch = line[i];

      if (ch == quote)             {isString = false; quote = 0;  continue;}
      if (ch == _T('"') || ch == _T('\')') {isString = true;  quote = ch; continue;}
      if (isString) continue;

      if (ch == _T(' ')) lastSpace = i;
      }
    if (lastSpace > 0) {
      line2 = line.substr(lastSpace+1);
      line  = line.substr(0, lastSpace);
      }
    }

  file.tab(tab);   if (lastSpace > 0) line += _T("\n");   file.write(line);
  if (lastSpace > 0) {

    n = outCount(line2);

    if (n + 2*tab + 4 < 102 || line2.find(_T(' ')) > 0) outputStg((102 - n) / 2, line2, file);

    else {file.tab(tab+4); file.write(line2);}
    }
  }


// outCounts '\"' as one character

int outCount(String& line) {
int n = line.length();

  if (n > 0 && line[n-1] == _T('\n')) n--;   return n;
  }
#endif

