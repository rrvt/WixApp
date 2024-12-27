// Common Utilities

#include "pch.h"
#include "Utilities.h"


void dspErr(TCchar* prefix, DWORD errorCode) {
Tchar   buf[1024];
String s;

  FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                0,
                errorCode,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPTSTR) buf,
                noElements(buf),
                0
                );

  s.format(_T("%s%s\n"), prefix, buf);  Output(s);
  }





// Our output routines for displaying messages to the console window or to stderr

void Output(TCchar* s) {
ToAnsi toAnsi(s);

  if (!s) return;

  cout << toAnsi();
  }


// Remove quotes and spaces from each end

void trim(String& s) {
int i;
int lng = s.length();
Tchar ch;

  for (i = 0; i < lng; i++)   {ch = s[i];  if (ch != _T(' ') && ch != _T('"')) break;}
  if (i) s = s.substr(i);

  lng = s.length();
  for (i = lng-1; i >= 0; i--) {ch = s[i]; if (ch != _T(' ') && ch != _T('"')) break;}
  if (++i < lng) s = s.substr(0, i);
  }

