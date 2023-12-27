// Date & Time using CTime


#include "pch.h"
#include "Date.h"
#include "MessageBox.h"
#include "StringInput.h"


// Helper functions for dealing with Edit boxes

static bool vrfyMnth( int cnt, TCchar ch, int& v);
static bool vrfyDay(  int cnt, TCchar ch, int& v);
static bool vrfyYr(   int cnt, TCchar ch, int& v);
static void replDtSel(int i,   TCchar ch, CEdit& ctrl);

static bool vrfyHr(  int cnt, TCchar ch, int& v);
static bool vrfyMin( int cnt, TCchar ch, int& v);
static void replTmSel(int i,  TCchar ch, CEdit& ctrl);



const int Date::MinDate = 30000;

typedef struct tm Tm;
const double Date::SecondsPerDay = 86400;


static TCchar*  msg = _T("Date format accepted:\n"
                    "mm/dd     - defaults to current year\n"
                    "mm/dd/yr\n"
                    "where:\n"
                    "  mm is 1 - 12\n"
                    "  dd is 1 - 31\n"
                    "  yr is 0 - 69 translates to 2000 - 2069\n"
                    "       70 - 99 translates to 1970 - 1999\n"
                    "     >100 - exact year\n"
                    "  Date only good from 1/1/1970 to year 3000\n"
                    "At least one slash ('/') must be present to accept a date.\n"
                    "Time Formats include the following where\n"
                    " H is either one or two hour digits:\n"
                    " Hmm      -- Hours and minutes\n"
                    " hh:mm:ss -- Hour, minutes and seconds\n"
                    " <time>a  -- am\n"
                    " <time>p  -- pm\n"
                    " hhmm     -- Hour and minutes in the 24-hour system\n"
                    "Notes:\n"
                    " ':'      -- may separate hours and minutes in any of the above except the latter\n"
                    " H > 12   -- 24 hour clock always interpreted correctly");


typedef LexT<StringInput, false, false, false> Lex;


static int utcOffset = 0;


Date::Date() : dt(0) {
Tm         utcTime;
Tm         lclTime;
int        noDays;
  dt.GetGmtTm(&utcTime);
  dt.GetLocalTm(&lclTime);

  noDays = lclTime.tm_yday ? 365 - lclTime.tm_yday : 0;

  utcOffset = ((noDays * 24 - lclTime.tm_hour) * 60 + lclTime.tm_min) * 60 + lclTime.tm_sec;
  }


Date Date::operator= (String& s) {
Lex        lex;
__time64_t tm = 0;
int        nDigits;
int        mm = 0;                            // Date may not be ealier than 1/1/1970 at UTC
int        dd = 0;
int        yr = 0;
int        hr = 0;                            // Time w/o date based on 1/1/70 date.  Sorry no zero date!
int        mn = 0;
int        ss = 0;
Token*     t;
Token*     t1;
Tm         today;

  getToday(); dt.GetLocalTm(&today);

  lex.initialize(); lex.input.set(s);

  lex.get_token();  t = lex.token;   t1 = lex.token1;

  if (t1->code == SlashToken) {

    if (t->code == IntLitToken) {
      mm = t->value.integer;
      if (mm < 1 || 12 < mm) {messageBox(msg); goto finDate;}

      lex.accept_two_tokens();

      if (lex.get_token() == IntLitToken) {

        t = lex.token; t1 = lex.token1;

        dd = t->value.integer;
        if (dd < 1 || 31 < dd) {messageBox(msg); goto finDate;}
        lex.accept_token();

        if (t1->code == SlashToken) {
          lex.accept_two_tokens();

          if (lex.get_token() == IntLitToken) {
            yr = lex.token->value.integer;
            if      ( 0 <= yr && yr <  70) yr += 2000;
            else if (70 <= yr && yr < 100) yr += 1900;
            lex.accept_token();
            }
          }
        else yr = today.tm_year + 1900;
        }
      }
    }

  if (lex.get_token() == SlashToken) lex.accept_token();

  if (lex.get_token() == EolToken || lex.token->code == EOFToken) goto finDate;

  if (lex.get_token() == IntLitToken) {
    t = lex.token;   hr = t->value.integer;   nDigits = t->noDigits;

    if (hr > 60 || nDigits > 2) {mn = hr % 100; hr = hr / 100;}

    if (hr < 0 || 24 <= hr || mn < 0 || 60 <= mn) {messageBox(msg); goto finDate;}

    if (nDigits == 4) goto finDate;

    lex.accept_token();

    if (lex.get_token() == ColonToken) {

      lex.accept_token();

      if (lex.get_token() == IntLitToken) {

        mn = lex.token->value.integer;  lex.accept_token();

        if (mn < 0 || 60 <= mn) {messageBox(msg); goto finDate;}

        lex.accept_token();

        if (lex.get_token() == ColonToken) {

          lex.accept_token();

          if (lex.get_token() == IntLitToken) {ss = lex.token->value.integer;}
          lex.accept_token();

          if (ss < 0 || 60 <= ss) {messageBox(msg); goto finDate;}
          }
        }
      }

    if (lex.get_token() == IdentToken) {
      if (!amPM(hr, lex.token->name)) messageBox(msg);   goto finDate;
      }

    if (lex.get_token() == EolToken || lex.token->code == EOFToken) goto finDate;
    }

  messageBox(msg);

finDate:
  if (!yr) {int t = (hr * 60 + mn) * 60 + ss + utcOffset;  CTime x((__time64_t) t); dt  = x;}
  else     {CTime x(yr, mm, dd, hr, mn, ss);    dt = x;   }
  return *this;
  }



Date& Date::operator= (COleDateTime& ole) {
SYSTEMTIME sysTm;

  ole.GetAsSystemTime(sysTm);

  CTime cTime(sysTm.wYear, sysTm.wMonth, sysTm.wDay, sysTm.wHour, sysTm.wMinute, sysTm.wSecond);

  dt = cTime;   return *this;
  }


Date::operator COleDateTime() {
SYSTEMTIME sysTm;

  dt.GetAsSystemTime(sysTm);   return sysTm;
  }


Date& Date::toLocalTime(Date& date) {
DYNAMIC_TIME_ZONE_INFORMATION tz;
DWORD                         rslt     = GetDynamicTimeZoneInformation(&tz);
int                           dayLight;

  switch (rslt) {
    case TIME_ZONE_ID_DAYLIGHT: dayLight = tz.DaylightBias; break;
    default                   : dayLight = 0;               break;
    }

  CTimeSpan delta(-(tz.Bias + dayLight) * 60);      //

  dt = date.dt;   dt += delta;   return *this;
  }



bool Date::amPM(int& h, String& txt) {
  if ((txt == _T("a") || txt == _T("A")) && h <= 12) return true;

  if ((txt == _T("p") || txt == _T("P")) && h <= 12) {h += 12; return true;}

  return false;
  }


static const double secPerDay = 86400;   // 60 * 60 * 24



Date::operator variant_t() {
double     t = double(dt.GetTime());
variant_t v;

  v.date = t / secPerDay; v.vt = VT_DATE; return v;
  }


String Date::getTime() {CString s; s = dt.Format(_T("%X")); return s;}



String Date::getHHMM()   {CString s; s = dt.Format(_T("%R")); return s;}
String Date::getHHMMSS() {CString s; s = dt.Format(_T("%T")); return s;}


String Date::getDate() {CString s = dt.Format(_T("%x"));  return s;}



String Date::dayOfWeek() {CString s = dt.Format("%a"); return s;}


Date& Date::operator<< (String& s) {
uint year  = 0;
uint month = 0;
uint day   = 0;
uint hour  = 0;
uint min   = 0;
uint sec   = 0;
uint pos   = 0;

  year  = s.substr( 0, 4).stoi(pos);
  month = s.substr( 4, 2).stoi(pos);
  day   = s.substr( 6, 2).stoi(pos);
  hour  = s.substr( 8, 2).stoi(pos);
  min   = s.substr(10, 2).stoi(pos);
  sec   = s.substr(12, 2).stoi(pos);
  CTime cTime(year, month, day, hour, min, sec);   dt = cTime;   return *this;
  }


static bool updateDate = false;


void Date::onChangeDate(CEdit& ctrl) {
CString date;
int     i;
int     n;
String  s;
int     count = 0;
int     slCnt = 0;
bool    legal;
int     mon   = 0;
int     day   = 0;
int     yr    = 0;

  if (!updateDate) {                                                      // To avoid infinite recursion
    updateDate = true;

    ctrl.GetWindowText(date);   s = date;

    for (i = 0, n = s.length(); i < n; i++) {
      Tchar ch = s[i];    legal = false;

      if (_T('0') <= ch && ch <= _T('9')) {
       legal = true;

       if (count || ch != _T('0')) {
         if (slCnt == 0 && !vrfyMnth(count, ch, mon)) {replDtSel(i, 0, ctrl); break;}
         if (slCnt == 1 && !vrfyDay( count, ch, day)) {replDtSel(i, 0, ctrl); break;}
         if (slCnt == 2 && !vrfyYr(  count, ch, yr))  {replDtSel(i, 0, ctrl); break;}
         }

       if (count > 1) {replDtSel(i, slCnt < 2 ? ch : 0, ctrl); break;}

       count++;
       }

      if (ch == _T('/')) {

        if (slCnt >= 2) {replDtSel(i, 0, ctrl); break;}

        count = 0;   slCnt++;   legal = true;
        }

      if (!legal) {replDtSel(i, 0, ctrl); break;}
      }

    updateDate = false;
    }
  }



bool vrfyMnth(int cnt, TCchar ch, int& v) {v = v * cnt * 10 + ch - _T('0');   return 1 <= v && v <= 12;}
bool vrfyDay( int cnt, TCchar ch, int& v) {v = v * cnt * 10 + ch - _T('0');   return 1 <= v && v <= 31;}
bool vrfyYr(  int cnt, TCchar ch, int& v) {v = v * cnt * 10 + ch - _T('0');   return 0 <= v && v <= 40;}


void replDtSel(int i, TCchar ch, CEdit& ctrl) {
bool   aCh = ch != 0;
String s;

  if (aCh) {s = _T('/'); s += ch;}
  else     {Beep(1500, 120);}

  ctrl.SetSel(i, i+1);   ctrl.ReplaceSel(s);
  }


static bool updateTime = false;


void Date::onChangeTime(CEdit& ctrl) {
CString time;
int     i;
int     n;
String  s;
int     count     = 0;
int     clnCnt    = 0;
bool    legal;
int     hr  = 0;
int     min = 0;

  if (!updateTime) {                                                      // To avoid infinite recursion
    updateTime = true;

      ctrl.GetWindowText(time);   s = time;

      for (i = 0, n = s.length(); i < n; i++) {
        Tchar ch = s[i];   legal = false;

        if (_T('0') <= ch && ch <= _T('9')) {
          legal = true;

          if (count || ch != _T('0')) {
            if (i     > 4) {replTmSel(i, 0, ctrl); break;}
            if (count > 1) {replTmSel(i, ch, ctrl); clnCnt++; count = 1; break; }

            if (    !clnCnt && !vrfyHr( count, ch, hr))  {replTmSel(i, 0, ctrl); break;}
            else if (clnCnt && !vrfyMin(count, ch, min)) {replTmSel(i, 0, ctrl); break;}
            }

          count++;
          }

        if (ch == _T(':')) {

          if (clnCnt >= 1) {replTmSel(i, 0, ctrl); break;}

          count = 0;   clnCnt++;  legal = true;
          }

        if (!legal) {replTmSel(i, 0, ctrl); break;}
        }

    updateTime = false;
    }


  }




bool vrfyHr( int cnt, TCchar ch, int& v) {v = v * cnt * 10 + ch - _T('0');   return v < 24;}
bool vrfyMin(int cnt, TCchar ch, int& v) {v = v * cnt * 10 + ch - _T('0');   return v < 60;}


void replTmSel(int i, TCchar ch, CEdit& ctrl) {
bool   aCh = ch != 0;
String s;

  if (aCh) {s = _T(':'); s += ch;}
  else     {Beep(1500, 120);}

  ctrl.SetSel(i, i+1);   ctrl.ReplaceSel(s);
  }




void ToDate::convert() {

  if (stg.length() < 14) return;

  year  = next(4);
  month = next(2);
  day   = next(2);
  hour  = next(2);
  min   = next(2);
  sec   = next(2);

  Date d(year, month, day, hour, min, sec);  date = d;
  }


int ToDate::next(int noChar) {
String s = stg.substr(pos, noChar);
uint   x;

  pos += noChar;  return s.stoi(x);
  }

