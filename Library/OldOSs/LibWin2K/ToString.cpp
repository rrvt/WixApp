// Win2000 to_string replacement
// BobK6RWY


#include "stdafx.h"
#include "ToString.h"


static char buf[25];


string to_string(int                Val) {sprintf_s(buf, sizeof(buf), "%i",   Val); return buf;}
string to_string(unsigned int       Val) {sprintf_s(buf, sizeof(buf), "%u",   Val); return buf;}
string to_string(long               Val) {sprintf_s(buf, sizeof(buf), "%li",  Val); return buf;}
string to_string(unsigned long      Val) {sprintf_s(buf, sizeof(buf), "%lu",  Val); return buf;}
string to_string(long long          Val) {sprintf_s(buf, sizeof(buf), "%lli", Val); return buf;}
string to_string(unsigned long long Val) {sprintf_s(buf, sizeof(buf), "%llu", Val); return buf;}
string to_string(float              Val) {sprintf_s(buf, sizeof(buf), "%f",   Val); return buf;}
string to_string(double             Val) {sprintf_s(buf, sizeof(buf), "%Lf",  Val); return buf;}
string to_string(long double        Val) {sprintf_s(buf, sizeof(buf), "%LLf", Val); return buf;}

