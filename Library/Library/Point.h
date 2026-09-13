// Point class to automate assignment/conversion


#pragma once


class Point : public POINT {

public:

  Point() : POINT({0,0}) { }
  Point(Point& pt) {x = pt.x;   y = pt.y;}
 ~Point() { }

  Point& operator= (Point& pt)     {x       = pt.x;   y       = pt.y;   return *this;}
  Point& operator() (int x, int y) {this->x = x;      this->y = y;      return *this;}
  };





///////---------

#if  0
class DisplayUdl {

int                  n;
Expandable<Point, 2> data;

public:

  DisplayUdl() : n(0) { }
 ~DisplayUdl() {data.clear();}

  void clear() {n = 0;}

  bool isEmpty() {return !n;}

  void start(  int x, int y);
  bool nextBeg(int x, int y);
  bool end(    int x, int y);

  int    noPairs()   {return n / 2;}
  Point* pair(int i) {return &data[i];}

private:

  void   set(int x, int y);
  };
#endif
