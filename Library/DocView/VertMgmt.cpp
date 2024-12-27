// Vertical Display Management


#include "pch.h"
#include "VertMgmt.h"
#include "DeviceContext.h"


VertMgmt::VertMgmt() {clear();}


void VertMgmt::clear() {
  y = yMax = maxHeight = pgHeight = topBnd = botBnd = 0;   beginPage = endPage = false;
  charHeight = uLineDelta = 0;    printing = footer = false;

  }


VertMgmt& VertMgmt::operator= (VertMgmt& vrt) {
    y          = vrt.y;           yMax       = vrt.yMax;
    maxHeight  = vrt.maxHeight;   pgHeight   = vrt.pgHeight;
    topBnd     = vrt.topBnd;      botBnd     = vrt.botBnd;
    beginPage  = vrt.beginPage;   endPage    = vrt.endPage;
    charHeight = vrt.charHeight;  uLineDelta = vrt.uLineDelta;
    printing   = vrt.printing;    footer     = vrt.footer;
    charHeight = vrt.charHeight;  uLineDelta = vrt.uLineDelta;
    return *this;
    }


void VertMgmt::initBounds(double topMgn, double botMgn) {
  topBnd = y = int(topMgn * charHeight);
  botBnd =     int(pgHeight - botMgn * charHeight);
  }


void VertMgmt::getMaxHeight() {if (charHeight > maxHeight) maxHeight = charHeight;}


int VertMgmt::getUlinePos()  {return y + uLineDelta;}


bool VertMgmt::lf(bool printing, bool footer) {

  if (printing && !footer && exceedsBnd()) {setEndPage(); return false;}

  y += maxHeight ? maxHeight : charHeight;   maxHeight = charHeight;   setMaxY(y);

  return true;
  }


bool VertMgmt::lf() {

  if (printing && !footer && exceedsBnd()) {setEndPage(); return false;}

  y += maxHeight ? maxHeight : charHeight;   maxHeight = charHeight;   setMaxY(y);

  return true;
  }


bool VertMgmt::exceedsBnd() {return y + (17 * maxHeight) / 8 >= botBnd;}



////-----------

//int VertMgmt::heightCh()     {return chHeight;}

