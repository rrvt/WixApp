// Conditional Debug It

#pragma once


class DebugIt {
bool hit;
public:

  DebugIt() : hit(false) { }

  void set() {hit = true;}
  bool isHit() {return hit;}
  };


extern DebugIt debugIt;
