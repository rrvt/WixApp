// Message Boxes with Various Inputs
// Translate a system error valude into a text string


#include "pch.h"
#include "MessageBox.h"




void getError(DWORD v, String& err) {
DWORD  lng;
Tchar  buf[256];

  lng = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                      0, v, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                      buf, noElements(buf), 0);

  err.format(_T("Err: %li, %s"), v, buf);
  }


void messageBox(TCchar* prefix, TCchar* tc)
                                      {String s;  s = prefix;  s += _T(": ");  s += tc;  messageBox(s);}


void messageBox(String s) {AfxMessageBox(s);}


void messageBox(TCchar* s) {AfxMessageBox(s);}


void messageBox(CString& s) {messageBox(String(s));}


void messageBox(TCchar* lbl, void* p, int n) {
int    i;
Byte*  x = (Byte*) p;
String s;

  s.format(_T("%s Addr: %08x -- %i Byte(s): "), lbl, p, n);

  for (i = n-1; i >= 0; i--) {
    if (i != n-1) s += _T(' ');

    String t;  t.format(_T("%02x"), x[i]);

    s += t;
    }
  messageBox(s);
  }


void debug(TCchar* s) {messageBox(String(s));}

void debug(TCchar* s, ulong v) {String t;  t = s; t += _T(" = "); t += v; messageBox(t);}

void debug(TCchar* s, String v) {String t; t = s; t += _T(" = "); t += v; messageBox(t);}

