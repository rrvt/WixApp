// Vertical Display Management


#include "stdafx.h"
#include "VertMgmt.h"

#include "MessageBox.h"
//#define DebugVM


#ifdef DebugVM
static int dbgY;
static int dbgTop;
static int dbgBot;
static int dbgN;
static int dbgMaxH;
static int dbgV;
static int dbgNoLines;

static int dbgCnt;
#endif

#if 0
testNoLines =
dbgNoLines = noLines;
if (dbgCnt == 3) {
int x = 1;
}
testTop = topBnd;
#endif


VertMgmt::VertMgmt() {clear();}


void VertMgmt::clear() {

  y = yMax = maxHeight = uLineDelta = botEdge = topBnd = botBnd = 0; chHeight = 1;  topMgn = botMgn = 0.0;
  beginPage = endPage = false; noLines = maxLines = 0;
#ifdef DebugVM
dbgCnt = 0;
#endif
  }


void VertMgmt::setAttributes(int height, double topMargin, double botMargin) {
  botEdge = height; topMgn = topMargin; botMgn = botMargin; initY();
  }


void VertMgmt::setTopMargin(double v) {topMgn = v;  initY();}
void VertMgmt::setBotMargin(double v) {botMgn = v;  initY();}


static int bufX = 0;
static int endBuf[10] = {0};


void VertMgmt::setBottom()  {
  if (y < botBnd) {
    y = botBnd;

    endBuf[bufX] = y;

    if (bufX > 0) {
      if (endBuf[bufX-1] != endBuf[bufX]) {
        String s;  s.format(_T("Whoops %i: %i, %i"), bufX, endBuf[bufX-1], endBuf[bufX]);
        int x = 1;  messageBox(s);
        }
      }
    if (++bufX >= noElements(endBuf)) bufX = 0;
    }
  }


void VertMgmt::setHeight(CDC* dc) {
TEXTMETRIC metric;

  if (dc->GetTextMetrics(&metric)) {

    chHeight = metric.tmHeight + metric.tmExternalLeading + 2;

    if (chHeight > maxHeight) maxHeight = chHeight;

    uLineDelta = chHeight - metric.tmInternalLeading - metric.tmExternalLeading;
    }
  }


bool VertMgmt::exceedsBnd(int n) {
#ifdef DebugVM
dbgY   = y;
dbgBot = botBnd;
dbgN   = n;
dbgMaxH = maxHeight;
dbgV    = y + n * maxHeight;
#endif

  return y + n * maxHeight > botBnd;
  }


bool VertMgmt::lf(bool printing, bool footer) {

  if (printing && !footer && exceedsBnd(1)) {

    setEndPage(); return false;
    }

  y += maxHeight;   maxHeight = chHeight;   setMaxY(y);   noLines++;

  if (printing && !footer && noLines > maxLines) maxLines = noLines;

#ifdef DebugVM
dbgY = y;
#endif

  return true;
  }

void VertMgmt::setEndPage() {
#ifdef DebugVM
dbgCnt++;
#endif
  y = botBnd; endPage = true;
  }


void VertMgmt::initY() {
  topBnd = y = int(topMgn * chHeight);  botBnd = int(botEdge - botMgn * chHeight);
#ifdef DebugVM
dbgTop = topBnd;
dbgBot = botBnd;
dbgY   = y;
#endif
  }

