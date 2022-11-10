// Wix file Output


#include "pch.h"
#include "WixOut.h"

#include "MessageBox.h"


WixOut  wix;                 // Wix Input File (output from this app)
IniFile wxd;                 // WixApp data file (contains description of installer)


static const int BreakBeforePos =  82;
static const int MaxLineLength  = 102;


// Output line provide s in not empty

void WixOut::operator() (TCchar*  prefix, TCchar* tc, TCchar* suffix) {
String s = tc;
String line;

  if (s.isEmpty()) return;

  line = prefix + s + suffix ;    (*this)(line);   crlf();
  }


void WixOut::operator() (TCchar* tc) {
String line = tc;
String line2;
int    i;
int    n;
Tchar  ch;
Tchar  quote     = 0;
bool   isString  = false;
int    lastSpace = -1;
int    maxLng;

  maxLng = BreakBeforePos - curTab;   n = line.length();

  if (curTab + n > MaxLineLength) {
    for (i = 0; i < maxLng; i++) {
      ch = line[i];

      if (ch == quote)                     {isString = false; quote = 0;  continue;}
      if (ch == _T('"') || ch == _T('\'')) {isString = true;  quote = ch; continue;}
      if (isString) continue;

      if (ch == _T(' ')) lastSpace = i;
      }
    if (lastSpace > 0) {
      line2 = line.substr(lastSpace+1);
      line  = line.substr(0, lastSpace);
      }
    }

  tab(curTab);   if (lastSpace > 0) line += _T("\n");    write(line);

  if (lastSpace > 0) {

    n = line2.length();

    if (n + 2*curTab + 4 < MaxLineLength || line2.find(' ') > 0) {
      int stk = curTab;   curTab = (MaxLineLength - n) / 2; (*this)(line2);   curTab = stk;
      }

    else {tab(curTab+4); write(line2);}
    }
  }


void WixOut::crlf(int n) {FileIO& f = *this;   for (int i = 0; i < n; i++) f.crlf();}





#if 0
// outCounts '\"' as one character

int WixOut::count(String& line) {
int n = line.length();

  if (n > 0 && line[n-1] == _T('\n')) {n--;   messageBox(_T("Found a slash n"));}

  return n;
  }
#endif
//void stg(TCchar* tc) {
//void WixOut::out(TCchar* prefix, TCchar* tc, TCchar* suffix) {

