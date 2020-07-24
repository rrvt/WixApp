// Horizonal line management for display/printer output


#include "stdafx.h"
#include "Horiz.h"


void Horiz::clear() {
  position = maxPos = rightEdge = leftMgn = rightMgn = leftBnd = rightBnd = savePos = 0;
  avgChWidth = 1.0; tabMgmt.clear();
  }

void Horiz::setAttributes(int width, double leftMargin, double rightMargin)
              {rightEdge = width; leftMgn = leftMargin; rightMgn = rightMargin; initPos(); setRtEdge();}


void Horiz::setLeftMargin( double v) {leftMgn = v;  initPos();}
void Horiz::setRightMargin(double v) {rightMgn = v; setRtEdge();}



void Horiz::setAvgChWidth(CDC* dc) {
TEXTMETRIC metric;
int        buf[26];
int        n;
int        i;
double     sum;
double     avg;

  if (dc->GetTextMetrics(&metric)) {

    dc->GetCharWidth(_T('A'), _T('Z'), buf);

    for (i = 0, n = noElements(buf), sum = 0; i < n; i++) sum += buf[i];

    avgChWidth = sum / n;

    avg = metric.tmAveCharWidth;   if (avg > avgChWidth) avgChWidth = avg;
    }
  }


void Horiz::cr() {if (position > maxPos) maxPos = position;  position = leftBnd;}                 // Carriage Return Only



// Insert an approximate character position for the next tab position

void TabMgmt::setTab(Horiz& hz, int pos) {TabX tab;   set(hz, pos, tab);}


void TabMgmt::setRTab(Horiz& hz, int pos) {TabX tab;   tab.right = true; set(hz, pos, tab);}


void TabMgmt::set(Horiz& hz, int pos, TabX& tab) {
double tabPos = pos;

  tab.pos = tabPos > 0 ? tabPos * hz.avgChWidth + hz.leftBnd : hz.rightBnd + hz.avgChWidth * tabPos;

  tabs = tab;               // inserts in sorted order
  }


void TabMgmt::findNextTab(Horiz& hz, TabX& tabX) {
double pos     = hz.position;
double chWidth = hz.avgChWidth;
double left    = hz.leftBnd;
int    i;
int    n;
double newCurPos;

  for (i = 0; i < tabs.end(); i++) if (tabs[i].pos - pos >= chWidth) {tabX = tabs[i]; return;}

  // Not found, so go to then next 5th position

  n = int((pos - left + chWidth - 1) / chWidth);  n = (n + 4) / 5;

  newCurPos = n * 5 * chWidth + left;

  if (newCurPos - pos < chWidth) newCurPos += 5 * chWidth;

  tabX.pos = newCurPos; tabX.right = false;
  }

