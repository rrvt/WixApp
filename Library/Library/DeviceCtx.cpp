// Virtual Base Class for Display and Printer Context classes


#include "pch.h"
#include "DeviceCtx.h"


DeviceCtx::~DeviceCtx() { }



void DeviceCtx::dspMetrics() {
String    s;
FontItem* font = fonts.getTop();   if (!font) return;

  s.format(_T("%s: %i, %i, %i, %i, %i"),
                                      font->getFace(), xEnd, yEnd, avgCharWidth, deltaY, deltaUL);
  messageBox(s);
  }





/////////////-------------------
#if 0
void DeviceCtx::setFace(TCchar* face)  {fonts.setFace(face);           updateMetrics();}
void DeviceCtx::setPoint(int tenthsPt) {fonts.setPoint(tenthsPt);      updateMetrics();}
void DeviceCtx::setBold()              {fonts.setBold();               updateMetrics();}
void DeviceCtx::setItalic()            {fonts.setItalic();             updateMetrics();}
void DeviceCtx::setFUnderline()        {fonts.setUnderline();          updateMetrics();}
void DeviceCtx::setStrikeout()         {fonts.setStrikeout();          updateMetrics();}
void DeviceCtx::popFont()              {fonts.pop();                   updateMetrics();}
#endif

//void DeviceCtx::clearTabs()              {tabs.clear();}
//void DeviceCtx::setTab(NoteTab& tab)     {tabs.set(tab.pos, tab.right);}

//void DeviceCtx::setCenter()              {center = true;}
//void DeviceCtx::setRight()               {right  = true;}

//void DeviceCtx::createFont(TCchar* face, int tenthsPt)
//                                                  {fonts.create(face, tenthsPt);  updateMetrics();}

//void DeviceCtx::finClearTabs()           {tabs.clear();}
//void DeviceCtx::finSetTab(NoteTab& tab)  {tabs.set(tab.pos, tab.right);}


