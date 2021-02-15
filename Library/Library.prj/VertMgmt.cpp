// Vertical Display Management


#include "stdafx.h"
#include "VertMgmt.h"
#include "MessageBox.h"


VertMgmt::VertMgmt() {clear();}


void VertMgmt::clear() {
  y = yMax = maxHeight = uLineDelta = botEdge = topBnd = botBnd = 0; chHeight = 1;  topMgn = botMgn = 0.0;

  beginPage = endPage = false; noLines = maxLines = 0;
  }


void VertMgmt::setAttributes(int height, double topMargin, double botMargin) {
  botEdge = height; topMgn = topMargin; botMgn = botMargin; initY();
  }


void VertMgmt::setTopMargin(double v) {topMgn = v;  initY();}
void VertMgmt::setBotMargin(double v) {botMgn = v;  initY();}


void VertMgmt::setHeight(CDC* dc) {
TEXTMETRIC metric;

  if (dc->GetTextMetrics(&metric)) {

    chHeight = metric.tmHeight + metric.tmExternalLeading + 2;

    if (chHeight > maxHeight) maxHeight = chHeight;

    uLineDelta = chHeight - metric.tmInternalLeading - metric.tmExternalLeading;
    }
  }


bool VertMgmt::exceedsBnd(int n) {return y + n * maxHeight > botBnd;}


bool VertMgmt::lf(bool printing, bool footer) {

  if (printing && !footer && exceedsBnd(1)) {setEndPage(); return false;}

  y += maxHeight;   maxHeight = chHeight;   setMaxY(y);   noLines++;

  if (printing && !footer && noLines > maxLines) maxLines = noLines;

  return true;
  }


void VertMgmt::setEndPage() {y = botBnd; endPage = true;}


void VertMgmt::initY() {
  topBnd = y = int(topMgn * chHeight/2);  botBnd = int(botEdge - botMgn * chHeight);
  }

