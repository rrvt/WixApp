// Date & Time using CTime



#pragma once


class Date {

CTime dt;

public:

  Date();
  Date(int hr, int min, int sec) : dt(1901, 1, 1, hr, min, sec) {}
  Date(int yr, int mon, int day, int hr, int min, int sec) : dt(yr, mon, day, hr, min, sec) {}
 ~Date() {}

  Date operator= (String& s);

  void getToday() {dt = CTime::GetCurrentTime();}

  Date& operator= (time_t t) {dt = t; return *this;}

  Date& operator= (variant_t& v)
          {double t;   if (v.vt == VT_DATE) {t = v; t *= SecondsPerDay; dt = time_t(t);}   return *this;}


  String getDate();
  String getTime();
  String dayOfWeek();
  operator String () {Cstring s; s = dt.Format(_T("%a %c")); return s;}

  bool empty()    {return dt.GetTime() == 0;}

  CTimeSpan operator -  (Date& t) {return dt - t.dt;};
  Date&     operator += (CTimeSpan n) {dt += n; return *this;}
  Date      operator +  (CTimeSpan n) {Date d; d.dt = dt; d.dt += n; return d;}

  operator variant_t ();

private:

static const double SecondsPerDay;

  bool amPM(int& h, String& txt);

  friend bool operator== (const Date& d, const Date& t);
  friend bool operator!= (const Date& d, const Date& t);
  friend bool operator>= (const Date& d, const Date& t);
  friend bool operator<= (const Date& d, const Date& t);
  friend bool operator>  (const Date& d, const Date& t);
  friend bool operator<  (const Date& d, const Date& t);
  };



inline bool operator== (const Date& d, const Date& t) {return (bool) (d.dt == t.dt);}
inline bool operator!= (const Date& d, const Date& t) {return (bool) (d.dt != t.dt);}
inline bool operator>= (const Date& d, const Date& t) {return (bool) (d.dt >= t.dt);}
inline bool operator<= (const Date& d, const Date& t) {return (bool) (d.dt <= t.dt);}
inline bool operator>  (const Date& d, const Date& t) {return (bool) (d.dt >  t.dt);}
inline bool operator<  (const Date& d, const Date& t) {return (bool) (d.dt <  t.dt);}


inline String toString(Date& d)       {String s; return s;}
inline String toString(CTimeSpan& sp) {String s; return s;}
