// Underline Display output


#pragma once


#include "Point.h"

class CDCex;


class UnderlineText {

CDCex& cdcEx;
bool   underlining{false};
Point  curPoint;

public:

  UnderlineText(CDCex& cdcEX) : cdcEx(cdcEX) { }
 ~UnderlineText() { }

  bool isUnderlining() {return underlining;}
  void start(int x, int y);
  bool draw( int x, int y);
  void stop() {underlining = false;   curPoint(0, 0);}

private:

  void drawLine(Point* pts);                // Two Points only

  void debug(Point* pts);
  };




//////////-----------

#if 0
class Point : public POINT {

public:

  Point() : POINT({0,0}) { }
  Point(Point& pt) {x = pt.x;   y = pt.y;}
 ~Point() { }

  Point& operator= (Point& pt)     {x       = pt.x;   y       = pt.y;   return *this;}
  Point& operator() (int x, int y) {this->x = x;      this->y = y;      return *this;}
  };
#endif

