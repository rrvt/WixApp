// Global Options


#pragma once
#include "CScrView.h"


struct Options {
double     dspScale;

PrtrOrient orient;
String     topMargin;
String     leftMargin;
String     rightMargin;
String     botMargin;
double     prtScale;

  Options() : orient(Portrait), topMargin(0.0), leftMargin(0.0), rightMargin(0.0), botMargin(0.0),
              dspScale(0.0), prtScale(0.0) { }
 ~Options() { }

  void operator() (CScrView* vw);

  void load();
  void store();
  };

extern Options options;

