// Capture ends of and output an underline


#pragma once

class DevCtx;


class UndrLn {

int   n;
POINT points[2];

public:

  UndrLn() : n(0) { }
 ~UndrLn() { }

  void clear() {n = 0;}
  void setBegin(long x, long y);
  void setEnd(  long x, long y);
  void output(DevCtx& dvx);
  };

