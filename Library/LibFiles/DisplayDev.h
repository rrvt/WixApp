// Display Device Data


#pragma once
#include "Display.h"
#include "NotePad.h"


class DisplayDev {
Display     dev;

NotePadLoop notePadLp;
Note*       note;

int         lastLeftMargin;

bool        endDoc;
bool        debugging;

public:
uint        lastPageNo;

  DisplayDev() {initialize();}
 ~DisplayDev() { }

  void preparePrinting(TCchar* font, int fontSize, CDC* pDC, bool doOutput)
                                                   {dev.preparePrinting(font, fontSize, pDC, doOutput);}
  void beginPrinting(CPrintInfo* pInfo) {dev.beginPrinting(pInfo);}
  bool isEndDoc() {return endDoc;}
  void clrFont()  {dev.clrFont();}
  void printFooter(int pageNo, String& license, Date& licDate);

  // Display functions

  void prepareDisplay( TCchar* font, int fontSize, CDC* pDC, bool doOutput)
                                                    {dev.prepareDisplay(font, fontSize, pDC, doOutput);}
  int  getWidth()  {return dev.getWidth();}
  int  getHeight() {return dev.getHeight();}
  void getMaxPos(long& maxX, long& maxY) {dev.getMaxPos(maxX, maxY);}

  void startDev();
  void toDevice();                  // Output to Device (Display or Printer)

  uint getNoPages()    {return dev.noPages;}

private:

  void initialize();
  };

