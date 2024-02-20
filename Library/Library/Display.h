// Display attributes


#pragma once


struct Display {

double scale;

  Display() : scale(0.0) { }
 ~Display() { }

  void load();
  void store();
  };

extern Display display;

