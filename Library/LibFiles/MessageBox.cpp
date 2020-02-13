// Message Boxes with Various Inputs
// Translate a system error valude into a text string


#include "stdafx.h"
#include "MessageBox.h"


void getError(DWORD v, String& err) {
DWORD  lng;
Tchar  buf[256];

  lng = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                      0, v, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                      buf, noElements(buf), 0);

  err.format(_T("Err: %li, %s"), v, buf);
  }


void messageBox(String s) {AfxMessageBox(s);}


void messageBox(TCchar* s) {AfxMessageBox(s);}

void messageBox(CString& s) {messageBox(String(s));}


void debug(TCchar* s) {messageBox(String(s));}

void debug(TCchar* s, ulong v) {String t;  t = s; t += _T(" = "); t += v; messageBox(t);}

void debug(TCchar* s, String v) {String t; t = s; t += _T(" = "); t += v; messageBox(t);}

