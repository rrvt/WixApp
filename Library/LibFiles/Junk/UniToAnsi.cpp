// Unicode to Ansi Object


#include "stdafx.h"
#include "UniToAnsi.h"


void ToAnsi::convert(TCchar* tp) {

  cnt = tp ? _tcslen(tp) : 0;    p = new char[cnt+1];

  #ifdef _UNICODE

    if (WideCharToMultiByte(CP_ACP, 0, tp, cnt, (LPSTR) p, cnt+1, 0, 0)) {p[cnt] = 0; return;}

  #else

    if (tp) {_tcscpy_s(p, cnt+1, tp); return;}

  #endif

  p[0] = 0;
  }




