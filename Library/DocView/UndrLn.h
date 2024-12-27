// Capture ends of and output an underline


#pragma once

class DeviceContext;


class UndrLn {

int   n;
POINT points[2];

public:

  UndrLn() : n(0) { }
 ~UndrLn() { }

  void clear() {n = 0;}

  void operator= (UndrLn& ul) {n = ul.n;   for(int i = 0; i < 2; i++) points[i] = ul.points[i];}

  void setBegin(long x, long y);
  void setEnd(  long x, long y);
  void output(DeviceContext& dvx);
  };

