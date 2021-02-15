// Win2000 to_string replacement
// BobK6RWY


#include "stdafx.h"
#include "ToString.h"


#define      nBuf 25
static Tchar buf[nBuf];


tstring to_tstring(int                Val) {_stprintf_s(buf, nBuf, _T("%i"),   Val); return buf;}
tstring to_tstring(unsigned int       Val) {_stprintf_s(buf, nBuf, _T("%u"),   Val); return buf;}
tstring to_tstring(long               Val) {_stprintf_s(buf, nBuf, _T("%li"),  Val); return buf;}
tstring to_tstring(unsigned long      Val) {_stprintf_s(buf, nBuf, _T("%lu"),  Val); return buf;}
tstring to_tstring(long long          Val) {_stprintf_s(buf, nBuf, _T("%lli"), Val); return buf;}
tstring to_tstring(unsigned long long Val) {_stprintf_s(buf, nBuf, _T("%llu"), Val); return buf;}
tstring to_tstring(float              Val) {_stprintf_s(buf, nBuf, _T("%f"),   Val); return buf;}
tstring to_tstring(double             Val) {_stprintf_s(buf, nBuf, _T("%Lf"),  Val); return buf;}
tstring to_tstring(long double        Val) {_stprintf_s(buf, nBuf, _T("%LLf"), Val); return buf;}

