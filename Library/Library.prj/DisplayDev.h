// Display Device Data


#pragma once
#include "Device.h"
#include "NotePad.h"


class DisplayDev {
String   id;
Device   dev;

NtPdIter npIter;
Note*    note;

bool     endDoc;
bool     debugging;

public:
uint     lastPageNo;

  DisplayDev(TCchar* src, NotePad& np) : id(src),          dev(src),          npIter(np),
                                         note(0),          endDoc(false),
                                         debugging(false), lastPageNo(0) {clear();}
 ~DisplayDev() { }

  void     clear() {dev.clear();}

  void     startDev();
  void     operator() ();                  // Output to Device (Display or Printer)

  void     preparePrinting(TCchar* font, double fontSize, CDC* pDC, CPrintInfo* pInfo)
                                            {dev.preparePrinting(font, fontSize, pDC, pInfo);}
  void     setHorzMgns(double left, double right) {dev.setHorzMgns(left, right);}
  void     setVertMgns(double top,  double bot)   {dev.setVertMgns(top, bot);}

  double   getFontScale() {return dev.getFontScale();}
  void     setFontScale(double scl) {dev.setFontScale(scl);}

  void     suppressOutput()                 {dev.suppressOutput();}
  void     negateSuppress()                 {dev.negateSuppress();}
  void     disableWrap()                    {dev.disableWrap();}
  void     enableWrap()                     {dev.enableWrap();}

  bool     isEndDoc()                       {return endDoc;}
  void     clrFont()                        {dev.clrFont();}

  Device& getDisplay() {return dev;}               // Useful for producing a footer

  // Display functions

  void prepareDisplay( TCchar* font, double fontSize, CDC* pDC) {dev.prepareDisplay(font, fontSize, pDC);}
  int  lgChWidth() {return dev.lgChWidth();}
  int  flChWidth() {return dev.flChWidth();}
  int  chHeight()  {return dev.chHeight();}
  int  vertPos()   {return dev.vertPos();}
  void getMaxPos(long& maxX, long& maxY) {dev.getMaxPos(maxX, maxY);}
  void clrLines()  {dev.clrLines();}
  int  maxLines()  {return dev.maxLines();}

  uint getNoPages()    {return dev.noPages;}

private:

  DisplayDev() : dev(_T("")), npIter(*(NotePad*)0) { }
  };

