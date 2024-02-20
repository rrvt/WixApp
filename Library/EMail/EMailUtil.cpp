


#include "pch.h"
#include "EMailUtil.h"

TCchar    TEolCh    = _T('\n');
TCchar*   TCrlf     = _T("\r\n");
const int Threshold = 78;




void substitute(TCchar* txt, TCchar* forTxt, String& body) {
String s;
int    pos;
int    len;

  if (!forTxt || !(len = _tcslen(forTxt))) return;

  for (pos = body.find(forTxt); pos >= 0; pos = body.find(forTxt))
                                       {s = body.substr(0, pos);   body = s + txt + body.substr(pos+len);}
  }

