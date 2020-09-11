// Wrap --


#include "stdafx.h"
#include "Wrap.h"


void Wrap::initialize(CDC* pDC, int widthLeft, bool isItalic)
                                               {dc = pDC; extent = widthLeft; this->isItalic = isItalic;}



int Wrap::operator() (TCchar* p) {
String s         = p;
String t;
int    threshold = 5 * extent / 8;
int    posWh;
int    posComma;
int    brkPt;
String word = _T("Word");

  lines.clear();

  if (!enabled) {lines += s;  return lines.end();}

  if (extent <= width(word)) {lines += s; return 1;}

  while (width(s) > extent) {
    brkPt    = 0;   t.clear();

    posWh    = findLast(_T(' '), s);
    posComma = findLast(_T(','), s);

    if (posWh    > brkPt) brkPt = posWh;
    if (posComma > brkPt) brkPt = posComma;
    if (brkPt > 0) t = s.substr(0, brkPt);

    if (width(t) < threshold) {brkPt = findLastChar(s); t = s.substr(0, brkPt);}
    if (t.isEmpty())          {brkPt = s.length() / 2;  t = s.substr(0, brkPt);}

    lines += t; s = s.substr(brkPt); s.trim();
    }

  if (!s.isEmpty()) lines += s;

  return lines.end();
  }


int Wrap::findLast(Tchar ch, String& s) {
int     i;
int     lng = s.size();
String  part;
CString cs;
int     lastPos = -1;

  for (i = 0; i < lng; i++) {
    if (s[i] == ch) {
      part = s.substr(0, i);   if (width(part) > extent) break;

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


