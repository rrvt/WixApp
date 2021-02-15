// Currency -- a LONGLONG with feeling
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.


#pragma once


class ArchiveIO;


class Currency {

LONGLONG cur;

public:

  Currency()               {cur = 0;}
  Currency(LONGLONG v)     {cur = v;}
  Currency(COleCurrency v) {cur = v.m_cur.int64;}

  Currency(variant_t v)   {cur = v.cyVal.int64;}

  ~Currency() {}

  Currency operator= (long v)         {cur = v;             return *this;}
  Currency operator= (COleCurrency v) {cur = v.m_cur.int64; return *this;}
  Currency operator= (variant_t v)    {cur = v.cyVal.int64; return *this;}

  operator LONGLONG()              {return cur;}

  Currency operator+ (long v)      {return cur + v;}
  Currency operator- (long v)      {return cur - v;}
  Currency operator* (long v)      {return cur * v;}
  Currency operator/ (long v)      {return cur / v;}
  Currency operator+=(long v)      {return cur += v;}
  Currency operator-=(long v)      {return cur -= v;}
  Currency operator*=(long v)      {return cur *= v;}
  Currency operator/=(long v)      {return cur /= v;}

  Currency operator+  (Currency v) {return cur +  v.cur;}
  Currency operator-  (Currency v) {return cur -  v.cur;}
  Currency operator*  (Currency v) {return cur *  v.cur;}
  Currency operator/  (Currency v) {return cur /  v.cur;}
  Currency operator+= (Currency v) {return cur += v.cur;}
  Currency operator-= (Currency v) {return cur -= v.cur;}
  Currency operator*= (Currency v) {return cur *= v.cur;}
  Currency operator/= (Currency v) {return cur /= v.cur;}

  bool     operator== (long v)     {return cur == v;}
  bool     operator!= (long v)     {return cur != v;}
  bool     operator<= (long v)     {return cur <= v;}
  bool     operator>= (long v)     {return cur >= v;}
  bool     operator== (Currency v) {return cur == v.cur;}
  bool     operator!= (Currency v) {return cur != v.cur;}
  bool     operator<= (Currency v) {return cur <= v.cur;}
  bool     operator>= (Currency v) {return cur >= v.cur;}

           operator String();

private:


  void getStg(LONGLONG v, int place, String& s);

  friend ArchiveIO& operator<< (ArchiveIO& ario, Currency& c);
  friend ArchiveIO& operator>> (ArchiveIO& ario, Currency& c);
  };




