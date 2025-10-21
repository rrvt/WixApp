

#include "pch.h"
#include "ThDisplay.h"
#include "Invalidate.h"
#include "NotePad.h"


void ThDisplay::dspCStg(TCchar* p) {
int    pos;
String s;
String t;

  s = p;

  if (!s.isEmpty()) {
    for (pos = s.find(_T('\n')); pos >= 0; pos = s.find(_T('\n')))
                            {t = s.substr(0, pos);   notePad << t << nCrlf;   s = s.substr(pos+1);}

    if (!s.isEmpty()) notePad << s;   invalidate();
    }
  }





ThDisplay thDisplay;



