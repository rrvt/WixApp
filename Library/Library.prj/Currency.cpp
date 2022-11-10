// Currency -- a LONGLONG with feeling
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.


#include "pch.h"
#include "Currency.h"



Currency::operator String() {
String            s;
String::size_type sz;

  if (cur >= 0) getStg(cur / 100, 0, s);
  else {cur = -cur; s = '-'; getStg(cur / 100, 0, s); cur = -cur;}

  sz = s.size();

  if      (sz == 3) s = _T("0")   + s;
  else if (sz == 2) s = _T("0.")  + s;
  else if (sz == 1) s = _T("0.0") + s;
  return s;
  }



void Currency::getStg(LONGLONG v, int place, String& s) {
Tchar digit = (v % 10) + '0';     v /= 10;

  if (v) getStg(v, place+1, s);    if (place == 1) s += '.';   s += digit; return;
  }

