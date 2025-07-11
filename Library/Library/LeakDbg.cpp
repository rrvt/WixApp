// Leak Debug


#include "pch.h"
#include "LeakDbg.h"


LeakDbg leak;


void LeakDbg::add(TCchar* txt) {
  if (n < noElements(data))
    data[n++] = txt;
  }


bool LeakDbg::track(String& s) {return s.find(_T("5473Blos")) >= 0;}

