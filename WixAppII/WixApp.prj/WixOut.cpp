// Wix file Output


#include "pch.h"
#include "WixOut.h"
#include "Utilities.h"


WixOut  wix;                 // Wix Input File (output from this app)
IniFile wxd;                 // WixApp data file (contains description of installer)


// Output line provide s in not empty

void WixOut::operator() (int nSpaces, TCchar* prefix, TCchar* tc, TCchar* suffix) {
String s = tc;
String line;

  if (s.isEmpty()) return;

  line = prefix + s + suffix ;    (*this)(nSpaces, line);
  }


// Optput one tabbed line

void WixOut::operator() (int nSpaces, TCchar* tc) {spcs = nSpaces;  (*this)(tc);   crlf();}


void WixOut::operator() (TCchar* tc) {
String line = tc ? tc : _T("");
String line2;
bool   isSplit;
//int    n;

  isSplit = findBrk(spcs, line, line2, BreakBeforePos);


  write(tabStg(spcs));    write(line);    if (!isSplit) return;

  write(_T("\n"));        blkOut(spcs + 2*TabVal, line2);
  }


void WixOut::blkOut(int tab, TCchar* tc, bool eol) {
String line  = tc;
String line2;
bool   isSplit = findBrk(tab, line, line2, MaxLineLength);
int    white   = eol ? MaxLineLength - line.length() : tab;

  write(tabStg(white));   write(line);

  if (!isSplit) return;

  write(_T("\n"));   blkOut(white, line2, false);
  }


bool WixOut::findBrk(int tab, String& line, String& line2, int brkPt) {
int   i;
int   maxLng;
Tchar ch;
Tchar quote    = 0;
bool  isString = false;
int   lastSpace = -1;

  if (tab + line.length() <= MaxLineLength) return false;

  for (i = 0, maxLng = brkPt - tab; i < maxLng; i++) {
    ch = line[i];

    if (ch == quote)                     {isString = false; quote = 0;  continue;}
    if (ch == _T('"') || ch == _T('\'')) {isString = true;  quote = ch; continue;}
    if (isString) continue;

    if (ch == _T(' ')) lastSpace = i;
    }
  if (lastSpace > 0)
           {line2 = line.substr(lastSpace+1);    line  = line.substr(0, lastSpace);   return true;}
  return false;
  }


void WixOut::crlf(int n) {FileIO& f = *this;   for (int i = 0; i < n; i++) f.crlf();}



