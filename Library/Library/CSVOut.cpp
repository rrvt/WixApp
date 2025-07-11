// CSV Output (takes care of internal commas, quotes, etc...


#include "pch.h"
#include "CSVOut.h"


static TCchar quote = _T('"');
       TCchar Comma = _T(',');


static CSVManipInt& setupManipInt(CSVManipInt::Func func, int val);


String& CSVOut::quotes(TCchar* p) {
static String s;
bool   addQuotes = false;

  s = p;

  if (s.find(quote) >= 0) {
    String t;
    int    n = s.length();
    int    i;

    for (i = 0; i < n; i++) {Tchar ch = s[i];   if (ch == quote) t += quote;   t += ch;}

    s = t; addQuotes = true;
    }

  if (s.find(_T(',')) >= 0) addQuotes = true;

  if (addQuotes) s = quote + s + quote;

  return s;
  }



CSVManip vCrlf;           // add to stream to terminate a line on display: dsp << "xyz" << vCrlf;

void CSVOut::initialize() {vCrlf.n = this; vCrlf.func = CSVOut::doCrlf;}



CSVManipInt& nCommas(int val) {return setupManipInt(CSVOut::doNCommas, val);}

CSVManipInt& setupManipInt(CSVManipInt::Func func, int val)
            {NewAlloc(CSVManipInt);   CSVManipInt* m = AllocNode;  m->set(func, val); return *m;}


CSVOut& CSVOut::doNCommas(CSVOut& csv, int n)
                                          {for (int i = 0; i < n; i++) csv << Comma;   return csv;}

