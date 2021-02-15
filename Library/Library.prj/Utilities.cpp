// Utilities


#include "stdafx.h"
#include "Utilities.h"
#include "Date.h"
#include "NotePad.h"


static String blks = _T("          ");


String addSepTab(String& s, int max) {
int   t  = s.length();
int   n  = max - t + 2;
Tchar ch = _T(',');

  if (!t) ch = _T(' ');    return s + ch + blks.substr(0, n);
  }


String addTab(String& s, int max) {int n = max - s.length(); return s + blks.substr(0, n);}


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


String& removeQuotes(String& s) {
String t;
int    i;
int    lng;
Tchar  ch;

  t = s;   lng = t.length();   s.clear();

  for (i = 0; i < lng; i++) {ch = t[i];  if (ch != _T('"') && ch != _T('\'')) s += ch;}

  return s;
  }

