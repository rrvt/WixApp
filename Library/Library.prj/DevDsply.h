// Display Device


#pragma once

#include "DevBase.h"


class DevDsply : public DevBase {

public:

       DevDsply() : DevBase() { }
      ~DevDsply() { }

  void prepare(CDC* dc);

  void getMaxPos(long& maxX, long& maxY) {maxY = txt->maxX(); maxY = vert.maxY();}
  };


