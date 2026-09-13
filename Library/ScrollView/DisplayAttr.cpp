// Printer Device Attributes


#include "pch.h"
#include "DisplayAttr.h"
#include "IniFileEx.h"
#include "Utilities.h"
#include "WinPos.h"


DisplayAttr displayAttr;


static TCchar* DisplayKey      = _T("Display");
static TCchar* TopMarginKey    = _T("TopMargin");
static TCchar* LeftMarginKey   = _T("LeftMargin");
static TCchar* RightMarginKey  = _T("RightMargin");
static TCchar* BottomMarginKey = _T("BottomMargin");



void DisplayAttr::load(CDC* dc, DriverInfo& info) {
String dfltTopMgn;
String dfltLeftMgn;
String dfltRightMgn;
String dfltBottomMgn;

  dfltTopMgn    = info.topMgn;
  dfltLeftMgn   = info.leftMgn;
  dfltRightMgn  = info.rightMgn;
  dfltBottomMgn = info.bottomMgn;

  width  = toInchesX(dc, (int)info.width);
  length = toInchesY(dc, (int)info.length);

  iniFile.read(DisplayKey, TopMarginKey,    topMgn,    dfltTopMgn);
  iniFile.read(DisplayKey, LeftMarginKey,   leftMgn,   dfltLeftMgn);
  iniFile.read(DisplayKey, RightMarginKey,  rightMgn,  dfltRightMgn);
  iniFile.read(DisplayKey, BottomMarginKey, bottomMgn, dfltBottomMgn);
  }



String DisplayAttr::toInchesX(CDC* dc, int pixels) {
double dbl = pixels * dc->GetDeviceCaps(HORZSIZE);
double div = 25.4   * double(dc->GetDeviceCaps(HORZRES));

  return get(dbl/div);
  }


String DisplayAttr::toInchesY(CDC* dc, int pixels) {
double dbl = pixels * dc->GetDeviceCaps(VERTSIZE);
double div = 25.4   * double(dc->GetDeviceCaps(VERTRES));

  return get(dbl/div);
  }


void DisplayAttr::loadDefaults(DriverInfo& info) {
  topMgn    = info.topMgn;
  leftMgn   = info.leftMgn;
  rightMgn  = info.rightMgn;
  bottomMgn = info.bottomMgn;
  }


void DisplayAttr::save() {

  iniFile.write(DisplayKey, TopMarginKey,    topMgn);
  iniFile.write(DisplayKey, LeftMarginKey,   leftMgn);
  iniFile.write(DisplayKey, RightMarginKey,  rightMgn);
  iniFile.write(DisplayKey, BottomMarginKey, bottomMgn);
  }







//////////////----------------

#if 0
void DisplayAttr::set(double width, double length)
                  {this->width = width;   this->length = length;   /*paperSz = UserDefinedPSz;*/}


#endif
#if 0
  rect   = winPos.get();
  width  = toInchesX(dc, rect.right  - rect.left);
  length = toInchesY(dc, rect.bottom - rect.top);
#endif

