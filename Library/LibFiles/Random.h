// Random Number Generator
// rrvt 3/31/17


#pragma once


class Random {
unsigned long x;

public:

  Random(long iVal) : x(iVal) {}
  Random() {x = (ulong) GetTickCount();}
 ~Random() {}

  float next();                       // Produces a value between 0 and 1.0
  };
