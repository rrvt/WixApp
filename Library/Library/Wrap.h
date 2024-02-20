// Wrap --


#pragma once
#include "DevCtx.h"


class Wrap {
bool    enabled;
DevCtx& dvx;
int     extent;
int     maxExtent;

String frag;
bool   usedMax;

public:

  Wrap(DevCtx& devCtx) :
                enabled(false), dvx(devCtx), extent(0), maxExtent(0), usedMax(false) { }
 ~Wrap() { }

  void    set(bool enable, int widthLeft, int maxWidth);

  void    clear() {enabled = false; extent = maxExtent = 0; usedMax= false;}

  bool    operator() (String& s);

  String& stg() {return frag;}

private:

  int    findLast(Tchar ch, String& s);
  int    findMaxExt(TCchar* tc);
  };


