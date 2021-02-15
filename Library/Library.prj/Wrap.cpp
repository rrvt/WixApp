// Wrap --


#include "stdafx.h"
#include "Wrap.h"


void Wrap::initialize(CDC* pDC, int widthLeft, int maxWidth, bool isItalic)
                          {dc = pDC; extent = widthLeft; maxExtent = maxWidth; this->isItalic = isItalic;}


int Wrap::operator() (TCchar* p) {
String s         = p;
String t;
int    threshold = 5 * extent / 8;
int    posWh;
int    posComma;
int    brkPt;
String word;
bool   indented = true;

  lines.clear();

  if (!enabled) {WrapData wd; wd.line = s; lines += wd;  return lines.end();}

  word = findFirst(s);

  if (extent <= width(word))
                      {WrapData wd; wd.line = s; wd.indented = false; lines += wd;  return lines.end();}

  while (width(s) > extent) {
    brkPt    = 0;   t.clear();

    posWh    = findLast(_T(' '), s);
    posComma = findLast(_T(','), s);

    if (posWh    > brkPt) brkPt = posWh;
    if (posComma > brkPt) brkPt = posComma;
    if (brkPt > 0) t = s.substr(0, brkPt);

    if (width(t) < threshold) {brkPt = findLastChar(s); t = s.substr(0, brkPt);}
    if (t.isEmpty())          {brkPt = s.length() / 2;  t = s.substr(0, brkPt);}

    if (extent < maxExtent / 3)
      {extent = maxExtent; indented = false;}

    WrapData wd; wd.line = t;   wd.indented = indented;

    lines += wd;    s = s.substr(brkPt);  s.trimLeft();
    }

  if (!s.isEmpty()) {WrapData wd; wd.line = s; wd.indented = indented; lines += wd;}

  return lines.end();
  }


String Wrap::findFirst(String& s) {
int pos = s.find(_T(' '));   if (pos < 0) pos = s.find(_T(','));   if (pos < 0) pos = s.length();

  return pos <= 0 ? _T("WWWWW") : s.substr(0, pos);
  }


int Wrap::findLast(Tchar ch, String& s) {
int     i;
int     lng = s.size();
String  part;
CString cs;
int     lastPos = -1;

  for (i = 0; i < lng; i++) {
    if (s[i] == ch) {
      part = s.substr(0, i);

      if (width(part) > extent) break;

      lastPos = i;
      }
    }

  return lastPos;
  }


int Wrap::findLastChar(String& s) {
int     i;
int     lng = s.size();
String  part;
CString cs;

  for (i = 1; i <= lng; i++) {part = s.substr(0, i);    if (width(part) > extent) break;}

  return i-1;
  }



// returns CString and width in current units

int Wrap::width(String& s) {
CSize   sz;
Cstring cs = s;   sz = dc->GetOutputTextExtent(cs);

  if (isItalic) sz.cx += 2;   return sz.cx;
  }


int Wrap::findLastWh(String& s, int noChars) {
int   lastPos = 0;
int   n = s.length();
int   i;

  n = n < noChars ? n : noChars;

  for (i = 0; i < n; i++) if (s[i] <= _T(' ')) lastPos = i;

  return lastPos;
  }


