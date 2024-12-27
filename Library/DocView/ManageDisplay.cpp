// Display Manager -- needed to isolate variables from Print Management


#include "pch.h"
#include "ManageDisplay.h"
#include "CScrView.h"
#include "DeviceData.h"
//#include "DevTabs.h"


void ManageDisplay::onPrepareDC(CDC* dc) {

  ntPdToDev.prepare(dc);

  ntPdToDev.initFont(font, fontSize);

  ntPdToDev.setHorzMgns(leftMargin, rightMargin);   ntPdToDev.setVertMgns(topMargin, botMargin);

  vw.onDisplayOutput();   ntPdToDev.startDev();
  }


void ManageDisplay::onDraw(CDC* pDC) {
Device&   dev = ntPdToDev.getDev();
DevStream devStrm(DspHdrID);

  if (!npd.isEmpty())  {
    devStrm.init(dev);
      devStrm.setWrapEnable(false);   vw.displayHeader(devStrm);
    dev.restore(DspHdrID);

    dev.setWrapEnable(wrapEnabled);   ntPdToDev();

    devStrm.init(dev);   devStrm.setFooter(true);
      devStrm.setWrapEnable(false);   vw.displayFooter(devStrm);
    devStrm.setFooter(false); dev.restore(DspHdrID);    dev.nonBlankLine = false;
    }

  setScrollSize();
  }


void ManageDisplay::setScrollSize() {
RECT  winSize;
int   height = ntPdToDev.chHeight();
int   t      = 1;
CSize scrollViewSize;
CSize pageSize;
CSize scrollSize;

  vw.GetClientRect(&winSize);

  if (height) {t = (winSize.bottom - 1) / height; t *= height;}

  pageSize.cy = t; pageSize.cx = winSize.right;

  scrollSize.cx = ntPdToDev.getCharWidth();   scrollSize.cy = height;

  ntPdToDev.getMaxPos(scrollViewSize.cx, scrollViewSize.cy);

  vw.SetScrollSizes(MM_TEXT, scrollViewSize, pageSize, scrollSize);
  }




///------------------

#if 1
#else
            {dev.startContext();   dev.disableWrap();   vw.displayHeader(dev);   dev.endContext();}
#endif
#if 1
#else
  if (!npd.isEmpty())
            {dev.setFooter();      dev.disableWrap();   vw.displayFooter(dev);   dev.clrFooter();}
#endif
// CScrView drawing,  Override

#if 0
void     setFooter() {footer = true;  /*startContext();*/if (printing) vert.setBottom();}
void     clrFooter() {footer = false; /*endContext();*/  TextAccm::nonBlankLine = false;}
#endif

//  ntPdToDev.clear();

