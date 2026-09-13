// Virtual Base Class for Display and Printer Manager classes


#pragma once
#include "DeviceCtx.h"
#include "CDCex.h"


class DeviceMgr {
protected:
CDCex&     cdcEx;

DeviceCtx* ctx;                     // Current Context
DeviceCtx* prevCtx{0};              // save and restore ctx

bool       wrap{true};
bool       suppress{false};

int        width{0};                // Dimensions in Pixels (Logical units)
int        length{0};
int        topMgn{0};
int        leftMgn{0};
int        rightMgn{0};
int        bottomMgn{0};

int        hdrOffset{0};            // Vertical distance from top of header to first body text line
int        ftrOffset{0};            // Vertical distance preserved for the footer

public:
               DeviceMgr(DeviceCtx& body, CDCex& devCtx) : cdcEx(devCtx), ctx(&body) { }
  virtual     ~DeviceMgr() = 0;

  virtual void setFonts(TCchar* face, int tenths) { }           // Face and tenths of point

  virtual bool output()      {return false;}
          void enableWrap()  {wrap = true;}
          void disableWrap() {wrap = false;}

  virtual void setLineHeight(int deltaY)  { }     // used for scrolling
  virtual void setCharWidth(int avgWidth) { }
  virtual void setMaxX(int x)             { }

  virtual bool endPage() {return false;}
  virtual bool endDoc()  {return false;}

  virtual void setHdr(TCchar* face, int tenthsPt) { }
  virtual void startHeader() { }
  virtual void endHeader() { }

  virtual bool startDoc(TCchar* docName) {return false;}
  virtual bool startPage() {return false;}

  virtual void setFtr(TCchar* face, int tenthsPt) { }
  virtual void startFooter() { }
  virtual void endFooter() { }

          void setCurCtx(DeviceCtx& pc);
          void rcvPrvCtx();

// Needed in Process One Item

          bool setLeftMgn( double v);
          bool setRightMgn(double v);

          bool setFace(TCchar* face);
          bool setPoint(double pts);
          bool setBold();
          bool setItalic();
          bool setFUnderline();
          bool setStrikeout();
          bool popFont();

          bool clearTabs();
          bool setTab(NoteTab& tab);          // {ctx->setTab(val, right);}
          bool moveNextTab();

          bool setCenter();
          bool setRight();

          bool startUnderlining();
          void setText(TCchar* txt);
          bool stopUnderlining();

          void examineFont();
          bool crlf();

// End of functions need by Process One Item

          void popAll();
          void updateMetrics() {ctx->updateMetrics();}

          bool isUnderlining()      {return ctx->isUnderlining();}
          bool drawUnderline()      {return ctx->drawUnderline();}

          bool isAtLeftMgn() {return ctx->x <= leftMgn;}
  virtual bool isBodyCtx() {return false;}
          bool isBufEmpty()  {return ctx->isBufEmpty();}

          void finLeftMgn( double v) {leftMgn  = xActualPixels(v);}
          void finRightMgn(double v) {rightMgn = xActualPixels(v);   ctx->xEnd = width - rightMgn;}
          int  xActualPixels(double inches) {return cdcEx.xActualPixels(inches);}
          int  yActualPixels(double inches) {return cdcEx.yActualPixels(inches);}

          void createFont(TCchar* face, int tenthsPt) {ctx->createFont(face, tenthsPt);}

          void finClearTabs()                         {ctx->finClearTabs();}
          void finSetTab(NoteTab& tab)                {ctx->finSetTab(tab);}

          void debug();

  friend class DisplayCtx;
  };

