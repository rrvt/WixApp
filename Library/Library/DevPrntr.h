// Printer Device

#pragma once

#include "DevBase.h"


class DevPrntr : public DevBase {

public:

  DevPrntr() : DevBase() { }         //TCchar* src  src
 ~DevPrntr() { }

  void     prepare(CDC* dc, CPrintInfo* pInfo);

  bool     doEndPageChk();

private:

  void     initializeFrame(CPrintInfo* pInfo);
  };



