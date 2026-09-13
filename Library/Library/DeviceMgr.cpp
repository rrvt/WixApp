// Virtual Base Class for Display and Printer Manager classes


#include "pch.h"
#include "DeviceMgr.h"


DeviceMgr::~DeviceMgr() { }


bool DeviceMgr::setLeftMgn( double v)
  {if (!ctx->flush()) return false;  ctx->setLeftMgn(v);   return true;}

bool DeviceMgr::setRightMgn(double v)
  {if (!ctx->flush()) return false;  ctx->setRightMgn(v);   return true;}

bool DeviceMgr::setFace(TCchar* face)
  {if (!ctx->flush()) return false;  ctx->setFace(face);   return true;}

bool DeviceMgr::setPoint(double pts)
  {if (!ctx->flush()) return false;  ctx->setPoint(int(pts * 10.0));   return true;}

bool DeviceMgr::setBold()
  {if (!ctx->flush()) return false;  ctx->setBold();   return true;}

bool DeviceMgr::setItalic()
  {if (!ctx->flush()) return false;  ctx->setItalic();   return true;}

bool DeviceMgr::setFUnderline()
  {if (!ctx->flush()) return false;  ctx->setFUnderline();   return true;}

bool DeviceMgr::setStrikeout()
  {if (!ctx->flush()) return false;  ctx->setStrikeout();   return true;}

bool DeviceMgr::popFont()
  {if (!ctx->flush()) return false;  ctx->popFont();   return true;}

bool DeviceMgr::clearTabs()
  {if (!ctx->flush()) return false;  ctx->clearTabs();   return true;}

bool DeviceMgr::setTab(NoteTab& tab)
  {if (!ctx->flush()) return false;  ctx->setTab(tab);   return true;}

bool DeviceMgr::moveNextTab()
  {if (!ctx->flush()) return false;  ctx->moveNextTab();   return true;}

bool DeviceMgr::setCenter()
  {if (!ctx->flush()) return false;  ctx->setCenter();   return true;}

bool DeviceMgr::setRight()
  {if (!ctx->flush()) return false;  ctx->setRight();   return true;}

bool DeviceMgr::startUnderlining()
  {if (!ctx->flush()) return false;  ctx->startUnderlining();   return true;}

void DeviceMgr::setText(TCchar* txt) {ctx->setText(txt);}

bool DeviceMgr::stopUnderlining()
  {if (!ctx->flush()) return false;  ctx->stopUnderlining();   return true;}

bool DeviceMgr::crlf()
  {if (!ctx->flush()) return false;  return ctx->crlf();}


void DeviceMgr::examineFont() {
Point  line[2];                                       //  tmDescent;
Point& pt0    = line[0];                              //  tmInternalLeading;
Point& pt1    = line[1];
int    x0     = ctx->x;                               //  tmExternalLeading;
int    x1     = x0 + ctx->avgCharWidth;               //  tmHeight;
int    x2     = x1 + ctx->avgCharWidth;
int    x3     = x2 + ctx->avgCharWidth;
int    x4     = x3 + ctx->avgCharWidth;
int    x5     = x4 + ctx->avgCharWidth;
int    x6     = x5 + ctx->avgCharWidth;
int    y      = ctx->y;                               //  tmAscent;
uint   blue   = RGB(  0,   0, 255);
uint   green  = RGB(  0, 255,   0);
uint   red    = RGB(255,   0,   0);
uint   magenta= RGB(255,   0, 255);
uint   yellow = RGB(255, 255,   0);
uint   black  = RGB(  0,   0,   0);
uint   cyan   = RGB(  0, 255, 255);

  pt0.x = x0;      pt1.x = x1;

  pt0.y = pt1.y = y;                                  //  drawLine(line, black);
                                                      //
  pt0.y = pt1.y = y + ctx->metrics.tmAscent;          //  drawLine(line, green);
  pt0.y = pt1.y = y + ctx->metrics.tmDescent;         //  drawLine(line, red);
  pt0.y = pt1.y = y + ctx->metrics.tmHeight;          //  drawLine(line, black);

  pt0.x = x1;   pt1.x = x2;

  pt0.y = pt1.y = y + ctx->metrics.tmAscent + ctx->metrics.tmDescent; // drawLine(line, black);
  pt0.y = pt1.y = y + ctx->metrics.tmInternalLeading;                 // drawLine(line, magenta);
  pt0.y = pt1.y = y + ctx->metrics.tmExternalLeading;                 // drawLine(line, yellow);

  ctx->x += x2;
  }


void DeviceMgr::setCurCtx(DeviceCtx& pc) {prevCtx = ctx;   ctx = &pc;   ctx->x = leftMgn;}


void DeviceMgr::rcvPrvCtx() {ctx = prevCtx;   ctx->x = leftMgn;}


void DeviceMgr::debug() {
CFont*  cFont = displayDC.getCurrentFont();   if (!cFont) return;
LOGFONT logFont;
String  s;

  cFont->GetLogFont(&logFont);

  s.format(_T("Current Font: %s, %i"), logFont.lfFaceName, logFont.lfHeight);   messageBox(s);
  }

