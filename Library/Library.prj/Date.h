// Date & Time using CTime



#pragma once


class Date {

CTime dt;

public:

static const int MinDate;           // Minimum No of Seconds allowed by MFC, Date date(MinDate);


  Date();
  Date(int hr, int min, int sec) : dt(1901, 1, 1, hr, min, sec) {}
  Date(int yr, int mon, int day, int hr, int min, int sec) : dt(yr, mon, day, hr, min, sec) {}
  Date(time_t sec) : dt(sec) { }
  Date(String& s) {*this = s;}
  Date(CString& cs) {String s = cs; *this = s;}
 ~Date() {}

  Date       operator= (String& s);                    // Translates m/d/yy h/m/s all digits to CTime
  Date       operator= (CString& cs) {String s = cs; return s;}
  Date&      operator= (CTime& tm)   {dt = tm; return *this;}

  Date&      operator= (variant_t& v)
          {double t;   if (v.vt == VT_DATE) {t = v; t *= SecondsPerDay; dt = time_t(t);}   return *this;}

         void getToday() {dt = CTime::GetCurrentTime();}


  static void onChangeDate(CEdit& ctrl);
  static void onChangeTime(CEdit& ctrl);


  time_t   getSeconds() {return dt.GetTime();}
  String   getDate();
  String   getTime();
  String   getHHMM();
  String   getHHMMSS();
  String   dayOfWeek();
  String   format(TCchar* f) {Cstring s; s = dt.Format(f);                    return s;}
  operator String ()         {Cstring s; s = dt.Format(_T("%#m/%#d/%y %H:%M")); return s;}
  String   toUnix() {Cstring s; s = dt.Format(_T("%Y%m%d%H%M%S")); return s;}

  bool     isEmpty()    {return dt.GetTime() == 0;}

  CTimeSpan operator -  (Date& t) {return dt - t.dt;};
  Date&     operator += (CTimeSpan n) {dt += n; return *this;}
  Date      operator +  (CTimeSpan n) {Date d; d.dt = dt; d.dt += n; return d;}

  bool      operator == (Date& d) {return dt == d.dt;}
  bool      operator != (Date& d) {return dt != d.dt;}
  bool      operator >= (Date& d) {return dt >= d.dt;}
  bool      operator <= (Date& d) {return dt <= d.dt;}
  bool      operator >  (Date& d) {return dt >  d.dt;}
  bool      operator <  (Date& d) {return dt <  d.dt;}

  operator variant_t ();

private:

static const double SecondsPerDay;

  bool amPM(int& h, String& txt);
  };


inline String toString(Date& d)       {String s; return s;}
inline String toString(CTimeSpan& sp) {String s; return s;}


class ToDate {
String stg;
int    pos;

int    year;
int    month;
int    day;
int    hour;
int    min;
int    sec;

Date   date;

public:

  ToDate(TCchar* ts) : stg(ts), pos(0) {convert();}

  Date& operator() () {return date;}

private:

  void convert();
  int  next(int noChar);
  };

// Format Codes
// %a -- Abbreviated weekday name in the locale
// %A -- Full weekday name in the locale
// %b -- Abbreviated month name in the locale
// %B -- Full month name in the locale
// %c -- Date and time representation appropriate for locale
// %C -- The year divided by 100 and truncated to an integer, as a decimal number (00?99)
// %d -- Day of month as a decimal number (01 - 31)
// %D -- Equivalent to %m/%d/%y
// %e -- Day of month as a decimal number (1 - 31), where single digits are preceded by a space
// %F -- Equivalent to %Y-%m-%d
// %g -- The last 2 digits of the ISO 8601 week-based year as a decimal number (00 - 99)
// %G -- The ISO 8601 week-based year as a decimal number
// %h -- Abbreviated month name (equivalent to %b)
// %H -- Hour in 24-hour format (00 - 23)
// %I -- Hour in 12-hour format (01 - 12)
// %j -- Day of the year as a decimal number (001 - 366)
// %m -- Month as a decimal number (01 - 12)
// %M -- Minute as a decimal number (00 - 59)
// %n -- A newline character (\n)
// %p -- The locale's A.M./P.M. indicator for 12-hour clock
// %r -- The locale's 12-hour clock time
// %R -- Equivalent to %H:%M
// %S -- Second as a decimal number (00 - 59)
// %t -- A horizontal tab character (\t)
// %T -- Equivalent to %H:%M:%S, the ISO 8601 time format
// %u -- ISO 8601 weekday as a decimal number (1 - 7; Monday is 1)
// %U -- Week number of the year as a decimal number (00 - 53), where the first Sunday is the first day
//       of week 1
// %V -- ISO 8601 week number as a decimal number (00 - 53)
// %w -- Weekday as a decimal number (0 - 6; Sunday is 0)
// %W -- Week number of the year as a decimal number (00 - 53), where the first Monday is the first day
//       of week 1
// %x -- Date representation for the locale
// %X -- Time representation for the locale
// %y -- Year without century, as decimal number (00 - 99)
// %Y -- Year with century, as decimal number
// %z -- The offset from UTC in ISO 8601 format; no characters if time zone is unknown
// %Z -- Either the locale's time-zone name or time zone abbreviation, depending on registry settings; no
//       characters if time zone is unknown
// %% -- Percent sign
//
// As in the printf function, the # flag may prefix any formatting code. In that case, the meaning of
// the format code is changed as follows:
//
//
// %#a, %#A, %#b, %#B, %#g, %#G, %#h, %#n,
// %#p, %#t, %#u, %#w, %#X, %#z, %#Z, %#%  -- # flag is ignored.
//
// %#c -- Long date and time representation, appropriate for the locale. For example:
//        "Tuesday, March 14, 1995, 12:41:29".
//
// %#x -- Long date representation, appropriate to the locale. For example: "Tuesday, March 14, 1995".
//
// %#d, %#D, %#e, %#F, %#H, %#I, %#j, %#m,
// %#M, %#r, %#R, %#S, %#T, %#U, %#V, %#W,
// %#y, %#Y                                -- Remove leading zeros or spaces (if any).
//
// The ISO 8601 week and week-based year produced by %V, %g, and %G, uses a week that begins on
// Monday, where week 1 is the week that contains January 4th, which is the first week that includes
// at least four days of the year. If the first Monday of the year is the 2nd, 3rd, or 4th, the preceding
// days are part of the last week of the preceding year. For those days, %V is replaced by 53, and both
// %g and %G are replaced by the digits of the preceding year.
