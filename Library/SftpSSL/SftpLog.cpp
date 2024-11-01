// Logging when turned on during compilation


#include "pch.h"
#include "SftpLog.h"
#include "NotePad.h"


static bool logging = false;
static void sftpLogOut(TCchar* m, TCchar* a);


void setLogging(TCchar* m, TCchar* a) {logging = true;  sftpLog(m, a);}
void clrLogging(TCchar* m, TCchar* a) {sftpLog(m, a);   logging = false;}
bool isLogging() {return logging;}


void sftpLog(TCchar* m, TCchar* a) {
String s;
int    pos;

  if (!logging) return;

  s = a ? a : _T(" ");

  for (pos = s.find(_T('\n')); pos >= 0; pos = s.find(_T('\n'))) {

    String t = s.substr(0, pos);   s = s.substr(pos+1);

    sftpLogOut(m, t);
    }

  if (!s.isEmpty()) sftpLogOut(m, s.trim());
  }


void sftpLogOut(TCchar* m, TCchar* a) {
  notePad << _T("Log ") << m;
  if (a && *a) notePad << _T(": ") << a;
  notePad << nCrlf;
  }

