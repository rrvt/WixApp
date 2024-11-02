// Logging when turned on during compilation


#pragma once
#include "NotePad.h"


#define SftpLogging 1


// Logging Call when #if is true (i.e. 1)

#ifdef SftpLogging

void setLogging(TCchar* m, TCchar* a = _T(" "));
void clrLogging(TCchar* m, TCchar* a = _T(" "));
bool isLogging();

void sftpLog(TCchar* m,    TCchar* a = _T(" "));

inline void sftpLog(TCchar* m, int     a) {String s(a);   sftpLog(m, s);}

#else

void sftpLog(TCchar* m, TCchar* a) { }

#endif

