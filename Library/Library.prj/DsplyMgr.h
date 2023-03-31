// Display Manager -- needed to isolate variables from Print Management


#pragma once
#include "NtPdToDsply.h"
#include "ShowMgr.h"


class DsplyMgr : public ShowMgr {

NotePad     npd;
NtPdToDsply displayOut;

double      leftMargin;
double      topMargin;
double      rightMargin;
double      botMargin;

public:

  DsplyMgr(CScrView& view) : ShowMgr(view, npd, displayOut), npd(), displayOut(npd),
                             leftMargin(0.33), rightMargin(0.33), topMargin(0.33), botMargin(0.33) { }
 ~DsplyMgr() { }

  void     setMgns(double left, double top, double right, double bot)
                             {leftMargin = left; topMargin = top;  rightMargin = right;  botMargin = bot;}

  void     onPrepareDC(CDC* dc);
  void     startDev() {displayOut.startDev();}
  void     onDraw(CDC* pDC);
  void     setScrollSize();

private:

  DsplyMgr() : ShowMgr(*(CScrView*)0, *(NotePad*)0, *(NtPdToDsply*)0), displayOut(*(NotePad*)0) { }
  };


