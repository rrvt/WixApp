// Virtual Base Class for Display and Printer Context classes


#pragma once
#include "DeviceBfr.h"
#include "FontStack.h"
#include "TabList.h"
#include "UnderlineText.h"


class DeviceCtx {
public:
DeviceBfr     buf;
FontStack     fonts;
TabList       tabs;
UnderlineText underlineText;    // Contains the data needed for under lining text
TEXTMETRIC    metrics;          // Metrics of the current font

int           x{0};             // Horizontal Position
int           xEnd{0};          // End of Horizontal Position
int           y{0};             // Vertical Position
int           yEnd{0};          // End of Vertical Position

int           deltaY{0};        // Amount to add to y for each line, changes with font
int           deltaUL{0};       // Underline position relative to top of line (i.e. y)
int           avgCharWidth{0};  // Integer tab scaled by average char width to horizontal position

bool          center{false};    // Center the next buffer of text
bool          right{false};     // Place the next buffer of text at right margin

           DeviceCtx(CDCex& cdcEx) :
                                    buf(cdcEx), fonts(cdcEx), tabs(cdcEx), underlineText(cdcEx) { }
  virtual ~DeviceCtx() = 0;

  virtual void setLeftMgn( double v) { }
  virtual void setRightMgn(double v) { }

          void setFace(TCchar* face)   {fonts.setFace(face);           updateMetrics();}
          void setPoint(int tenthsPt)  {fonts.setPoint(tenthsPt);      updateMetrics();}
          void setBold()               {fonts.setBold();               updateMetrics();}
          void setItalic()             {fonts.setItalic();             updateMetrics();}
          void setFUnderline()         {fonts.setUnderline();          updateMetrics();}
          void setStrikeout()          {fonts.setStrikeout();          updateMetrics();}
          void popFont()               {fonts.pop();                   updateMetrics();}

          void clearTabs()             {tabs.clear();}
          void setTab(NoteTab& tab)    {tabs.set(tab.pos, tab.right);}
  virtual void moveNextTab() { }

          void setCenter()             {center = true;}
          void setRight()              {right  = true;}

  virtual void startUnderlining() { }
          void setText(TCchar* txt)    {buf += txt;}
          void stopUnderlining()       {underlineText.stop();}

  virtual bool crlf() {return true;}

  virtual bool isAtLeftMgn()           {return false;}
  virtual bool isEndPage()             {return y + deltaY > yEnd;}

  virtual bool flush()                 {return false;}

          bool isBufEmpty()            {return buf.isEmpty();}
          void textOut(int n)          {buf.textOut(this, n);   drawUnderline();}

          bool isUnderlining()         {return underlineText.isUnderlining();}
          bool drawUnderline()         {return underlineText.draw(x, y + deltaUL);}
          int  yUnderLine()            {return y + deltaUL;}

          int  find(int curPos)        {return tabs.find(curPos);}

          void setDefaultFont(TCchar* face, int tenthsPt) {fonts.setDefault(face, tenthsPt);}
          void initializeFont()                           {fonts.initialize();   updateMetrics();}
          void clrFonts()                                 {fonts.popAll();}

  virtual void setMaxX(int x) { }

          void createFont(TCchar* face, int tenthsPt)
                                                  {fonts.create(face, tenthsPt);  updateMetrics();}

          void finClearTabs()           {tabs.clear();}
          void finSetTab(NoteTab& tab)  {tabs.set(tab.pos, tab.right);}

  virtual void updateMetrics() { }

          void dspMetrics();
  };





////////-----------------
#if 0
  virtual
  virtual
  virtual
  virtual
  virtual
  virtual
  virtual
#endif
//          void setDefaultFont()                           {fonts.setDefault();}
//CFont*        cFont{0};         // Current Font, i.e. copy of the top of the font stack

