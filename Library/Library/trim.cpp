// Trim blanks from both ends of string
// (C) Copyright Software Design & Engineering
// All rights reserved
// rrvt 6/26/95


#include "pch.h"
#include "trim.h"
#include <string.h>


// Trim blanks from both ends of string

Tchar* trim(Tchar* p) {
Tchar* q;
  if (!p) return 0;
  while (*p && *p <= ' ') p++;
  q = _tcschr(p,0); if (q == p) return p;
  while (q > p && *--q <= ' ') ; *++q = 0; return p;
  }


// Remove whitespace from end of string, return number of bytes in string

int trimEnd(Tchar* s) {
Tchar* p = _tcschr(s, 0);

  while (--p >= s && *p <= ' ') continue;

  *++p = 0; return (int) (p - s);
  }

