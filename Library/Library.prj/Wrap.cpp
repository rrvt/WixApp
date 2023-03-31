// Wrap --


#include "pch.h"
#include "Wrap.h"


void Wrap::set(bool enable, int widthLeft, int maxWidth)
        {enabled = enable && maxWidth >= 100; extent = widthLeft; maxExtent = maxWidth; usedMax = false;}


bool Wrap::operator() (String& s) {
int    threshold = 5 * extent / 8;
int    posWh;
int    posComma;
int    brkPt = 0;
String word;

  if (!enabled || dvx.width(s) <= extent) {frag = s;   s.clear();   return false;}

  posWh    = findLast(_T(' '), s);
  posComma = findLast(_T(','), s);

  if (posWh    > brkPt) brkPt = posWh;
  if (posComma > brkPt) brkPt = posComma;
  if (!brkPt)           brkPt = findMaxExt(s);

  if (usedMax || brkPt > 10) frag = s.substr(0, brkPt);
  else {extent = maxExtent;   usedMax = true;  return (*this)(s);}

  if (dvx.width(frag) < threshold) {brkPt = findMaxExt(s); frag = s.substr(0, brkPt);}

  s = s.substr(brkPt);  s.trimLeft();   return !s.isEmpty();
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

      if (dvx.width(part) > extent) break;

      lastPos = i;
      }
    }

  return lastPos;
  }


int Wrap::findMaxExt(TCchar* tc) {
String t = tc;
int    n = t.length();
int    i;
String part;

  for (i = n-2; i > 0; i--) {part = t.substr(0, i);  if (dvx.width(part) <= extent) break;}

  return i;
  }



