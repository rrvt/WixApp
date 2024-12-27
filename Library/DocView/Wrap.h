// Wrap --


#pragma once
#include "DeviceContext.h"


class Wrap : public DeviceContext {
bool   enabled;
int    extent;
int    maxExtent;

String frag;
bool   usedMax;

public:

  Wrap() : enabled(false), extent(0), maxExtent(0), usedMax(false) { }
 ~Wrap() { }

  void    clear();
  void    setWrapEnable(bool enbld) {enabled = enbld;}

  void    copy(Wrap& wrp);

  void    setWrap(bool enable, int widthLeft, int maxWidth);

  bool    isEnabled() {return enabled;}

  bool    wrap(String& s);

  String& stg() {return frag;}

private:

  int    findLast(Tchar ch, String& s);
  int    findMaxExt(TCchar* tc);
  };


