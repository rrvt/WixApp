// Display Manager -- needed to isolate variables from Print Management


#include "pch.h"
#include "DsplyMgr.h"
#include "CScrView.h"
#include "DevTabs.h"


void DsplyMgr::onPrepareDC(CDC* dc) {

  displayOut.clear();   displayOut.prepare(dc);

  displayOut.initFont(font, fontSize);

  displayOut.setHorzMgns(leftMargin, rightMargin);   displayOut.setVertMgns(topMargin, botMargin);

  vw.onDisplayOutput();   displayOut.startDev();
  }


// CScrView drawing,  Override

void DsplyMgr::onDraw(CDC* pDC) {
DevBase& dev = displayOut.getDev();

  dev.startContext();   dev.disableWrap();   vw.displayHeader(dev);   dev.endContext();

    if (wrapEnabled) dev.enableWrap(); else dev.disableWrap();   displayOut();

  dev.setFooter();   dev.disableWrap();   vw.displayFooter(dev);  dev.clrFooter();

  setScrollSize();
  }


void DsplyMgr::setScrollSize() {
RECT  winSize;
int   height = displayOut.chHeight();
int   t      = 1;
CSize scrollViewSize;
CSize pageSize;
CSize scrollSize;

  vw.GetClientRect(&winSize);

  if (height) {t = (winSize.bottom - 1) / height; t *= height;}

  pageSize.cy = t; pageSize.cx = winSize.right;

  scrollSize.cx = displayOut.lgChWidth();   scrollSize.cy = height;

  displayOut.getMaxPos(scrollViewSize.cx, scrollViewSize.cy);

  vw.SetScrollSizes(MM_TEXT, scrollViewSize, pageSize, scrollSize);
  }

