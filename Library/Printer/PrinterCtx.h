// There are three contexts for output to the printer: Header, Body and Footer


#pragma once
#include "DeviceCtx.h"

class CDCex;
class PrinterMgr;


class PrinterCtx : public DeviceCtx {
public:
PrinterMgr& pm;

       PrinterCtx(PrinterMgr& prtrMgr);
      ~PrinterCtx() { }

  bool isAtLeftMgn();
  void setLeftMgn( double v);
  void setRightMgn(double v);
  bool isEndPage() {return y + deltaY > yEnd;}

  void startUnderlining();

  void moveNextTab();

  bool crlf();

  void updateMetrics();

  bool flush();

private:

  PrinterCtx() : pm(*(PrinterMgr*)0), DeviceCtx(*(CDCex*)0) { }
  };






