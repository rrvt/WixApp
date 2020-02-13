// Get System Error as String


#include "stdafx.h"
#include "GetError.h"


void getError(DWORD v, String& err) {
DWORD  lng;
Tchar  buf[256];

  lng = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                      0, v, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                      buf, noElements(buf), 0);

  err.format(_T("Err: %li, %s"), v, buf);
  }

