// Display Manager -- needed to isolate variables from Print Management


#pragma once
#include "NtPdToDev.h"
#include "ManageDevBase.h"


class ManageDisplay : public ManageDevBase {

NotePad   npd;
NtPdToDev ntPdToDev;

double    leftMargin;
double    topMargin;
double    rightMargin;
double    botMargin;

public:

  ManageDisplay(CScrView& view) : ManageDevBase(view, npd, ntPdToDev), npd(),
                                  ntPdToDev(npd, DisplayID),
                                  leftMargin(0.33), rightMargin(0.33),
                                  topMargin(0.33),  botMargin(0.33) { }
 ~ManageDisplay() { }

  void setMgns(double left, double top, double right, double bot)
                      {leftMargin = left; topMargin = top;  rightMargin = right;  botMargin = bot;}

  void onPrepareDC(CDC* dc);
  void startDev() {ntPdToDev.startDev();}
  void onDraw(CDC* pDC);
  void setScrollSize();

private:

  ManageDisplay() : ManageDevBase(*(CScrView*)0, *(NotePad*)0, *(NtPdToDev*)0),
                                                         ntPdToDev(*(NotePad*)0, DisplayID) { }
  };


