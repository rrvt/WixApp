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

  DisplayDev() {clear(); note = 0; lastLeftMargin = 0;  endDoc = debugging = false; lastPageNo = 0;}
 ~DisplayDev() { }

  void     clear() {dev.clear();}          // startDev clears and sets as required to perform operator()

  void     startDev();
  void     operator() ();                  // Output to Device (Display or Printer)

  void     preparePrinting(TCchar* font, int fontSize, CDC* pDC, CPrintInfo* pInfo)
                                            {dev.preparePrinting(font, fontSize, pDC, pInfo);}
  void     setHorzMgns(double left, double right) {dev.setHorzMgns(left, right);}
  void     setVertMgns(double top,  double bot)   {dev.setVertMgns(top, bot);}

  void     suppressOutput()                 {dev.suppressOutput();}
  void     negateSuppress()                 {dev.negateSuppress();}
  void     disableWrap()                    {dev.disableWrap();}
  void     enableWrap()                     {dev.enableWrap();}

  bool     isEndDoc()                       {return endDoc;}
  void     clrFont()                        {dev.clrFont();}

  Display& getDisplay() {return dev;}               // Useful for producing a footer

  // Display functions

  void prepareDisplay( TCchar* font, int fontSize, CDC* pDC) {dev.prepareDisplay(font, fontSize, pDC);}
  int  chWidth()   {return dev.chWidth();}
  int  chHeight()  {return dev.chHeight();}
  int  vertPos()   {return dev.vertPos();}
  void getMaxPos(long& maxX, long& maxY) {dev.getMaxPos(maxX, maxY);}
  void clrLines() {dev.clrLines();}
  int  maxLines()  {return dev.maxLines();}

  uint getNoPages()    {return dev.noPages;}
  };

