// Capture ends of and output an underline


#include "pch.h"
#include "UndrLn.h"
#include "DevCtx.h"

void UndrLn::setBegin(long x, long y) {points[0].x = x; points[0].y = y;  n = 1;}


void UndrLn::setEnd(  long x, long y) {points[1].x = x; points[1].y = y;  n = 2;}


void UndrLn::output(DevCtx& dvx) {if (points[0].y == points[1].y) dvx.line(points, n);}

