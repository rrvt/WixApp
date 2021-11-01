// Horizonal line management for display/printer output


#include "stdafx.h"
#include "Horiz.h"


void Horiz::clear() {
  position = maxPos = rightEdge = leftMgn = leftBase = rightMgn = leftBnd = rightBnd = savePos = 0;
  avgLgChWidth = 1.0; avgFlChWidth = 1.0; tabMgmt.clear();
  }

void Horiz::setAttributes(int width, double leftMargin, double rightMargin)
    {rightEdge = width; leftMgn = leftBase = leftMargin; rightMgn = rightMargin; initPos(); setRtEdge();}


void Horiz::setLeftMargin( double v) {leftMgn = v + leftBase;  initPos();}
void Horiz::setRightMargin(double v) {rightMgn = v; setRtEdge();}



void Horiz::setAvgLgChWidth(CDC* dc) {
TEXTMETRIC metric;
int        buf[26];
int        n;
int        i;
double     sum;

  if (dc->GetTextMetrics(&metric)) {

    dc->GetCharWidth(_T('A'), _T('Z'), buf);

    for (i = 0, n = noElements(buf), sum = 0; i < n; i++) sum += buf[i];

    avgLgChWidth = sum / n;

    avgFlChWidth = metric.tmAveCharWidth;   if (avgFlChWidth > avgLgChWidth) avgLgChWidth = avgFlChWidth;
    }
  }


void Horiz::cr() {if (position > maxPos) maxPos = position;  position = leftBnd;}  // Carriage Return Only



// Insert an approximate character position for the next tab position

void TabMgmt::setTab(Horiz& hz, int pos) {TabX tab;   set(hz, pos, tab);}


void TabMgmt::setRTab(Horiz& hz, int pos) {TabX tab;   tab.right = true; set(hz, pos, tab);}


void TabMgmt::set(Horiz& hz, int pos, TabX& tab) {
double tabPos = pos;

  tab.pos = tabPos > 0 ? tabPos * hz.avgLgChWidth + hz.leftBnd : hz.rightBnd + hz.avgLgChWidth * tabPos;

  tabs = tab;               // inserts in sorted order
  }


void TabMgmt::findNextTab(Horiz& hz, TabX& tabX) {
double pos       = hz.position;
double lgChWidth = hz.avgLgChWidth;
double left      = hz.leftBnd;
int    i;
int    n;
double newCurPos;

  for (i = 0; i < tabs.end(); i++) if (tabs[i].pos - pos >= lgChWidth) {tabX = tabs[i]; return;}

  // Not found, so go to then next 5th position

  n = int((pos - left + lgChWidth - 1) / lgChWidth);  n = (n + 4) / 5;

  newCurPos = n * 5 * lgChWidth + left;

  if (newCurPos - pos < lgChWidth) newCurPos += 5 * lgChWidth;

  tabX.pos = newCurPos; tabX.right = false;
  }

