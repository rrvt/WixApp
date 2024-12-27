// Device Display/Printer Output Base Class



#pragma once
#include "TextAccm.h"


enum DeviceID {NilDevID, DisplayID, DspHdrID, ReportID, RptHdrID, PrinterID, PrtHdrID};


class Device : public TextAccm {
public:
uint noPages;

          Device();
         ~Device() {clear();}

  void    clear() {TextAccm::clear();   noPages = 0;}

  Device& operator= (Device& dev);
  void    restore(DeviceID id);

  void    setBeginPage()              {vert.setBeginPage();}
  void    initFont(TCchar* face, double fontSize) {setBaseFont(face, fontSize);}

  void    initTxt()                   {TextAccm::initialize();}     // Initialization
  void    setHorzMgns(double left, double right);
  void    setVertMgns(double top,  double bot);
  void    setWrapEnable(bool enbld)   {Wrap::setWrapEnable(enbld);}

  void    allowOutput(bool allow)     {suppress = !allow;}

  CDC*    getDC()                     {return get();}
  int     chHeight()                  {return vert.charHeight;}    // Character attributes
  int     getCharWidth()              {return widthInt();}
  void    getMaxPos(long& maxX, long& maxY) {maxY = horz.maxX(); maxY = vert.maxY();}
  double  getFontScale()              {return fontMgr.scale;}
  void    setFontScale(double scale)  {fontMgr.scale = scale;}

  void    setLeftMargin( double val)  {TextAccm::setLeftMargin(val);}
  void    setRightMargin(double val)  {TextAccm::setRightMargin(val);}

  void    setFontFaceII(TCchar* face) {TextAccm::setFontFace(face);}
  void    setFontSizeII(double  val)  {TextAccm::setFontSize(val);}
  void    setBold()                   {TextAccm::setFontBold();}
  void    setItalic()                 {TextAccm::setFontItalic();}
  void    setUnderLine()              {TextAccm::setFontUnderLine();}
  void    setStrikeOut()              {TextAccm::setFontStrikeOut();}
  void    popFontAttr()               {TextAccm::popFontAttr();}

  void    clrTabs()                   {TextAccm::clrTabs();}
  void    setTab(NoteTab& noteTab)    {TextAccm::setTab(noteTab);}

  void    setTabSeen()                {TextAccm::setTabSeen();}
  void    setCenter()                 {TextAccm::setCenter();}
  void    setRight()                  {TextAccm::setRight();}

  void    setBeginUnderLine()         {TextAccm::setBeginUnderLine();}
  void    append(TCchar* tc)          {TextAccm::append(tc);}
  void    append(NoteNmbr& nmbr)      {TextAccm::append(nmbr);}
  void    setEndUnderLine()           {TextAccm::setEndUnderLine();}

  void    crlf()                      {TextAccm::crlf();}
  void    cr()                        {TextAccm::cr();}

  void    flushFtr() {TextAccm::flush(); lf(); cr();}

  void    setBottom()                 {vert.setBottom();}
  bool    isEndPage()                 {return vert.isEndPage();}
  bool    doEndPageChk();
  void    setEndPage();

  bool    isWrapFin()                 {return TextAccm::isWrapFin();}
  void    cleanupWrap()               {TextAccm::cleanupWrap();}
  };




////--------------

//#include "DeviceContext.h"

