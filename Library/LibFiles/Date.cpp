// Date & Time using CTime


#include "stdafx.h"
#include "Date.h"
#include "MessageBox.h"
#include "StringInput.h"



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


typedef LexT<StringInput, LexTOut, false, false, false> Lex;


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


String Date::getDate() {CString s = dt.Format(_T("%x"));  return s;}



String Date::dayOfWeek() {CString s = dt.Format("%a"); return s;}

