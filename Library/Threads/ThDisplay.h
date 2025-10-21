// Thread Display based on template


#pragma once
#include "ThDisplayT.h"



typedef ThDisplayT<Tchar, 16> ThDisplayBase;


class ThDisplay : public ThDisplayBase {

private:

  void dspCStg(TCchar* p);
  };


extern ThDisplay thDisplay;

