// Random Number Generator
// rrvt 3/31/17


#include "pch.h"
#include "Random.h"

// Knuth First Edition Random number constants

const unsigned long a =   45000061;
const unsigned long c =  907633343;
const double        m = 4294967296.0;

// Produces a value between 0 and 1.0

float Random::next() {
double f;
  x = a * x + c; f = x; return (float) (f/m);
  }

