// Wix file Output


#include "stdafx.h"
#include "WixOut.h"


WixOut  wix;                 // Wix Input File (output from this app)
IniFile wxd;                 // WixApp data file (contains description of installer)



// Output line provide s in not empty

void WixOut::out(int tabVal, TCchar* prefix, String& s, TCchar* suffix) {
String line;

  if (s.isEmpty()) return;

  line = prefix + s + suffix + _T("\n");    stg(tabVal, line);
  }


void WixOut::stg(int tabVal, String& s) {
String line = s;
String line2;
int    i;
int    n;
Tchar   ch;
Tchar   quote     = 0;
bool   isString  = false;
int    lastSpace = -1;
int    maxLng    = 82 - tabVal;

  n = count(line);

  if (tabVal + n > 102) {
    for (i = 0; i < maxLng; i++) {
      ch = line[i];

      if (ch == quote)             {isString = false; quote = 0;  continue;}
      if (ch == '"' || ch == _T('\'')) {isString = true;  quote = ch; continue;}
      if (isString) continue;

      if (ch == ' ') lastSpace = i;
      }
    if (lastSpace > 0) {
      line2 = line.substr(lastSpace+1);
      line  = line.substr(0, lastSpace);
      }
    }

  tab(tabVal);   if (lastSpace > 0) line += _T("\n");  write(line);
  if (lastSpace > 0) {

    n = count(line2);

    if (n + 2*tabVal + 4 < 102 || line2.find(' ') > 0) stg((102 - n) / 2, line2);

    else {tab(tabVal+4); write(line2);}
    }
  }


// outCounts '\"' as one character

int WixOut::count(String& line) {
int n = line.length();

  if (n > 0 && line[n-1] == _T('\n')) n--;   return n;
  }

