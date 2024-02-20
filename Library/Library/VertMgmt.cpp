// Vertical Display Management


#include "pch.h"
#include "VertMgmt.h"


VertMgmt::VertMgmt(DevCtx& devCtx) : dvx(devCtx) {clear();}


void VertMgmt::clear() {y = yMax = topBnd = botBnd = 0;   beginPage = endPage = false;}


void VertMgmt::initBounds() {
double factor;

  factor = dvx.chHeight;   factor /= 2;

  topBnd = y = int(dvx.topMgn * factor);
  botBnd =     int(dvx.pgHeight - dvx.botMgn * factor);
  }


bool VertMgmt::lf(bool printing, bool footer) {

  if (printing && !footer && exceedsBnd()) {setEndPage(); return false;}

  y += maxHeight ? maxHeight : dvx.chHeight;   maxHeight = dvx.chHeight;   setMaxY(y);   return true;
  }


bool VertMgmt::exceedsBnd() {return y + (17 * maxHeight) / 8 >= botBnd;}

