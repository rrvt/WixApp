// Leak Debug


#pragma once


class LeakDbg {
int    n;
String data[128];

public:

  LeakDbg() : n(0) { }
 ~LeakDbg() { }

  void add(TCchar* txt);
  bool track(String& s);
  };


extern LeakDbg leak;
