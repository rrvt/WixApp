// Utilities


#include "stdafx.h"
#include "Utilities.h"
#include "Date.h"
#include "NotePad.h"


static String blks = _T("                                                                         ");


String addSepTab(String& s, int max) {
int   t  = s.length();
int   n  = max - t + 2;
Tchar ch = _T(',');

  if (!t) ch = _T(' ');    return s + ch + blks.substr(0, n);
  }


String rightAdj(TCchar* tc, int right) {
String s = tc;
int    t  = s.length();
int    n  = right - t;     if (n < 0) n = 0;

  return blks.substr(0, n) + s;
  }


String addTab(TCchar* tc, int tab) {
String s = tc;
  int n = tab - s.length(); return n > 0 ? s + blks.substr(0, n) : s;
  }


String tabStg(int n) {return blks.substr(0, n);}


String nextTok(String& s, int& pos) {
int bPos = pos;
int n    = s.length();
int ePos;

  ePos = s.find(_T(','), pos);   if (ePos < 0) ePos = s.find(_T('\n'));

  if (ePos < 0) return s.substr(bPos);

  for (pos = ePos+1; pos < n; pos++) if (s[pos] > _T(' ')) break;

  return s.substr(bPos, ePos-bPos);
  }


String normalizeDate(TCchar* date) {
String s  = date ? date : _T("");   if (s.isEmpty()) return s;
Date   dt = s;

  s = dt.format(_T("%D"));  return s;
  }

String normalizeTime(TCchar* time) {
String s  = time ? time : _T("");   if (s.isEmpty()) return s;
Date   dt = s;

  s = dt.format(_T("%R"));  return s;
  }


String getDateNow() {Date dt;   dt.getToday();   return dt.format(_T("%D"));}

String getTimeNow() {Date dt;   dt.getToday();   return dt.format(_T("%R"));}


time_t getDiff(Date& dtUpper, Date& dtLower)
                                   {CTimeSpan delta = dtUpper - dtLower; return delta.GetTotalSeconds();}



void floatingOnly(CEdit& ctrl) {
CString cs;
String  s;
int     n;
int     i;
bool    periodSeen = false;

  ctrl.GetWindowText(cs);   s = cs;

  for (i = 0, n = s.length(); i < n; i++) {
    Tchar ch = s[i];

    if (_T('0') <= ch && ch <= _T('9')) continue;

    if (ch == _T('.') && !periodSeen) {periodSeen = true; continue;}

    Beep(1500, 120);   ctrl.SetSel(i, i+1);   ctrl.ReplaceSel(_T(""));
    }
  }


static TCchar quote = _T('"');


String addQuotes(TCchar* p) {
String s;
bool   needQuotes = false;

  s = p;

  if (s.find(quote) >= 0) {
    String t;
    int    n = s.length();
    int    i;

    for (i = 0; i < n; i++) {Tchar ch = s[i];   if (ch == quote) t += quote;   t += ch;}

    s = t; needQuotes = true;
    }

  if (s.find(_T(',')) >= 0) needQuotes = true;

  if (needQuotes) s = quote + s + quote;

  return s;
  }


String& removeQuotes(String& s) {
String t;
int    i;
int    lng;
Tchar  ch;

  t = s;   lng = t.length();   s.clear();

  for (i = 0; i < lng; i++) {ch = t[i];  if (ch != _T('"') && ch != _T('\'')) s += ch;}

  return s;
  }


bool isEmpty(String* s, ...) {
va_list args;
String* p;

  if (s->size() != 0) return false;

  for (va_start(args, s);  (p = va_arg(args, String*)) && p != 0;) if (p->size() != 0) return false;

  return true;
  }





String formatPhone(TCchar* ph, Tchar sep) {
String s = ph;
String t;
int    n = s.length();   if (!n) return _T("          ");

  t = s.substr(0, 3);   s = t + sep + s.substr(3);

  t = s.substr(0, 7);   s = t + sep + s.substr(7);   return s;
  }

