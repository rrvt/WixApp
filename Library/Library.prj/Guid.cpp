// Get String containing a GUID


#include "pch.h"
#include "Guid.h"
#include "Rpcdce.h"
#include "Rpc.h"


bool getGuid(String& s) {
GUID    guid;
OLECHAR buf[512];
OLECHAR ch;
int     i;

  s.clear();

  if (CoCreateGuid(&guid) != S_OK) return false;

  if (!StringFromGUID2(guid,  buf, noElements(buf))) return false;

  for (i = 0; i < noElements(buf); i++) {
    ch = buf[i];   if (!ch) return true;

    if (ch == _T('{') || ch == _T('}')) continue;

    s += (Tchar) ch;
    }

  return false;
  }

