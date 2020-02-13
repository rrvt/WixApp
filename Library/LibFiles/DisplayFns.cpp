// Display Functions -- Interface into Doc/View class


#include "stdafx.h"
#include "DisplayFns.h"
#include "trim.h"
#include <afxmt.h>


static CCriticalSection criticalSection;
void   invalidate();


DWORD dspError(const Tchar* fmt, DWORD v) {
Tchar  buf[256];
DWORD  lng;
Tchar  s[256];
Tchar* p;

  lng = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                      0, v, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                      buf, noElements(buf), 0);
  p = trim(buf);   lng = (DWORD) _tcslen(p);

  if (lng) _stprintf_s(s, noElements(s), _T("%s, Last Error is (%lu), \"%s\"\n"), fmt, v, p);
  else     _stprintf_s(s, noElements(s), _T("%s, Last Error is (%lu)\n"),         fmt, v);

  displayMsg(String(s)); return v;
  }


void  dspStderr(const Tchar* fmt) {displayMsg(String(fmt));}


void displayStg(String& msg, int v)
                            {Tchar buf[256]; _stprintf_s(buf, noElements(buf), msg, v); displayMsg(buf);}


void displayStg(String& msg) {
int lng = msg.size();
int i;
DataFile*   cf = getDspHandle();
CSingleLock lock(&criticalSection);

  if (cf) {
    lock.Lock();
    for (i = 0; i < lng; i++) putDsp(cf, msg[i]);
    lock.Unlock();
    }
  }



void clrDsp() {DataFile* cf = getDspHandle();   if (cf) cf->clear();}


void printDsp(TCchar* fmt, ...) {
va_list     ap;
Tchar       stg[1024];
int         i;
DataFile*   cf = getDspHandle();
CSingleLock lock(&criticalSection);

  if (cf) {

    va_start(ap, fmt);  _vstprintf_s(stg, noElements(stg), fmt, ap); va_end(ap);

    lock.Lock();
    for (i = 0; stg[i] && i < noElements(stg); i++) putDsp(cf, stg[i]);
    lock.Unlock();
    }

  invalidate();
  }


void displayStg(TCchar* fmt, ...) {
va_list     ap;
Tchar       stg[1024];
int         i;
DataFile*   cf = getDspHandle();
CSingleLock lock(&criticalSection);

  if (cf) {

    va_start(ap, fmt);  _vstprintf_s(stg, noElements(stg), fmt, ap); va_end(ap);

    lock.Lock();
    for (i = 0; stg[i] && i < noElements(stg); i++) putDsp(cf, stg[i]);
    lock.Unlock();
    }
  }


void crlfDsp() {putDsp(getDspHandle(), '\n');}


void putDsp(DataFile* cf, Tchar ch) {cf->put(ch);}



#ifdef _DEBUGPuTTY
extern "C" {      // Begin extern "C" to allow PuTTY functions to link to the functions containd herein

// Intercept output from PLine functions -- Note c interface (_cdecl)

int fprintf(FILE* h, const Tchar* f, ...) {
va_list ap;
int     n;

  va_start(ap, f); n = vfprintf(h, f, ap); va_end(ap); return n;
  }


int vfprintf(FILE* h, const Tchar* f, va_list ap) {
Tchar    stg[1024];
  vsprintf_s(stg, noElements(stg), f, ap); printDsp("%s", stg); return strlen(stg);
  }

}
#endif

