// Utilities


#include "pch.h"
#include "Utilities.h"
#include "NotePad.h"


String toRemotePath(TCchar* path) {
String s = path;
int    n = s.length();
int    i;

  for (i = 0; i < n; i++) if (s[i] == _T('\\')) s[i] = _T('/');

  return s;
  }


String toLocalPath(TCchar* path) {
String s = path;
int    n = s.length();
int    i;
int    pos;
String prefix;

  for (i = 0; i < n; i++) if (s[i] == _T('/')) s[i] = _T('\\');

  pos = s.find(_T("\\\\"));   if (pos >= 0) {prefix = s.substr(0,pos+1);   s = prefix + s.substr(pos+2);}

  return s;
  }



void dspLines(String& s) {
String ss = s;
int    pos;
String t;

  while (!ss.isEmpty()) {
    pos = ss.find(_T('\n'));   t = pos > 0 ? ss.substr(0, pos) : ss ;

    notePad << t << nCrlf;   ss = ss.substr(t.length() + 1);
    }
  }



String parseName(TCchar* path) {
String s   = path;
int    pos = s.findLastOf(_T('\\'));   if (pos < 0) return s;

  return s.substr(pos + 1);
  }


String& fixLocalPath(String& path)
                          {if (path[path.length() - 1] != _T('\\')) path += _T('\\'); return path;}
String& fixRemotePath(String& path)
                          {if (path[path.length() - 1] != _T('/'))  path += _T('/');  return path;}


String removeSpaces(TCchar* name) {
String s = name;
int    pos;
String t;

  for (pos = s.find(_T(' ')); pos >= 0; pos = s.find(_T(' ')))
                                                           {t += s.substr(0, pos);   s = s.substr(pos+1);}
  t += s;   return t;
  }


String ensureSite(TCchar* name) {
String s = name;   s.lowerCase();
String t = name;
int    pos;

  pos = s.find(_T("site"));   if (pos >= 0) t = t.substr(0, pos);
  pos = s.find(_T("web"));    if (pos >= 0) t = t.substr(0, pos);

  t.trim();   t += _T(" Web Site");   t.trim();   return t;
  }


int pathLevel(TCchar* path) {
String pth = path;
String sect;
int    lvl = 0;
int    pos;

  for (pos = pth.find(_T('\\')); pos >= 0; pos = pth.find(_T('\\'))) {
    sect = pth.substr(0, pos+1);   pth = pth.substr(pos+1);   if (!sect.isEmpty()) lvl++;
    }

  if (!pth.isEmpty()) lvl++;

  return lvl;
  }

