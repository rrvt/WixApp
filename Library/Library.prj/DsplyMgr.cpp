// Display Manager -- needed to isolate variables from Print Management


#include "stdafx.h"
#include "DsplyMgr.h"
#include "CScrView.h"


void DsplyMgr::OnPrepareDC(CDC* dc) {

  dspDev.clear();

  dspDev.setHorzMgns(leftMargin, rightMargin);   dspDev.setVertMgns(topMargin, botMargin);

  dspDev.prepareDisplay(font, fontSize, dc);

  vw.onPrepareOutput(false);
  }


// CScrView drawing,  Override

void DsplyMgr::OnDraw(CDC* pDC) {dspDev();   dspDev.clrFont();   setScrollSize();}


void DsplyMgr::setScrollSize() {
RECT  winSize;
int   height = dspDev.chHeight();
int   t      = 1;
CSize scrollViewSize;
CSize pageSize;
CSize scrollSize;

  vw.GetClientRect(&winSize);

  if (height) {t = (winSize.bottom - 1) / height; t *= height;}

  pageSize.cy = t; pageSize.cx = winSize.right;

  scrollSize.cx = dspDev.lgChWidth();   scrollSize.cy = height;

  dspDev.getMaxPos(scrollViewSize.cx, scrollViewSize.cy);

  vw.SetScrollSizes(MM_TEXT, scrollViewSize, pageSize, scrollSize);
  }
