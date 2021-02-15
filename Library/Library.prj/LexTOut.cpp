// LexT virtual Output class


#include "stdafx.h"
#include "LexTOut.h"
#include "NotePad.h"


void LexTOut::operator()(NotePad& np, TCchar* fmt, ...) {
va_list ap;
Tchar   stg[1024];

  va_start(ap, fmt);   _vstprintf_s(stg, noElements(stg), fmt, ap); va_end(ap);

  np << stg;
  }

