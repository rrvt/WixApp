// There are three contexts for output to the printer: Header, Body and Footer


#pragma once
#include "DeviceCtx.h"

class CDCex;
class DisplayMgr;


class DisplayCtx : public DeviceCtx {
public:
DisplayMgr&   dm;

       DisplayCtx(DisplayMgr& displayMgr);
      ~DisplayCtx() { }

  bool isAtLeftMgn();
  void setLeftMgn( double v);
  void setRightMgn(double v);
  bool isEndPage() {return false;}

  void setMaxX(int x);

  void startUnderlining() {underlineText.start(x, y + deltaUL);}

  void moveNextTab();

  bool crlf();



  void updateMetrics();

  bool flush();

private:

  DisplayCtx() : dm(*(DisplayMgr*)0), DeviceCtx(*(CDCex*)0) { }
  };



