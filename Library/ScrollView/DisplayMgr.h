// Printer Manager -- Interface to Windows


#pragma once
#include "CDCex.h"
#include "DeviceMgr.h"
#include "DisplayCtx.h"


extern TCchar* ArialFc;
extern TCchar* TimesFc;
extern TCchar* CourierFc;
extern TCchar* ComicFc;
extern TCchar* BlackadderFc;
extern TCchar* BauhausFc;


class DisplayMgr : public DeviceMgr {

DisplayCtx hdrCtx{*this};
DisplayCtx bodyCtx{*this};
DisplayCtx ftrCtx{*this};

protected:

int        lineHeight{1};                     // maximum Line height, used for scrolling
int        charWidth{1};                      // Maximum character width, used for scrolling
int        maxX{1};
int        maxY{1};

public:

       DisplayMgr(CDCex& cdcEx) : DeviceMgr(bodyCtx, cdcEx) { }
      ~DisplayMgr();

  void prepare(CDC* cdc, CRect& r);

  void setFonts(TCchar* face, int tenths);           // Face and tenths of point

  bool startDoc(TCchar* docName);
  bool startPage();
  bool endPage();

  bool output();

  void finLine() {
    if (ctx->x > leftMgn)
      crlf();
    }

  void setLineHeight(int deltaY)  {if (deltaY      > lineHeight) lineHeight = deltaY;}
  void setCharWidth(int avgWidth) {if (avgWidth    > charWidth)  charWidth  = avgWidth;}
  void setMaxX(int x)             {if (x           > maxX)       maxX       = x;}
  void setMaxY(int y)             {if (y           > maxY)       maxY       = y;}

  int  getLineHeight() {return lineHeight;}
  int  getCharWidth()  {return charWidth;}
  int  getMaxX()       {return maxX;}
  int  getMaxY()       {return maxY;}

  void setHdr(TCchar* face, int tenthsPt);
  void startHeader();
  void endHeader();

  void setFtr(TCchar* face, int tenthsPt);
  void startFooter();
  void endFooter();

  bool isBodyCtx()   {return ctx    == &bodyCtx;}

  bool flush()       {return ctx->flush();}

private:

  DisplayMgr() : DeviceMgr(*(DeviceCtx*)0, *(CDCex*)0) { }

  friend class DisplayBfr;
  friend class DisplayCtx;
  friend class DisplayNtPd;
  };




///////-----------------

//  void SetAdobePDFOutputName(String& sOutputFilePath);
//  void hmmToTxtExt(CSize& sz);
//  bool getTextMetrics();
//  void getLogicalUnits();
#if 0
DisplayFonts      bodyFonts{this};
DisplayFonts      hdrFonts{this};
DisplayFonts      ftrFonts{this};
DisplayFonts*     curFonts{&bodyFonts};

TabList    bodyTabs;
TabList    hdrTabs;
TabList    ftrTabs;

DisplayFonts*     prevFonts{0};
CFont*     prevFont{0};
#endif
//  void hmmToTxtExt(CSize& hmm);
#if 0
bool    polyULine{false};     // Draw a Poly Underline when true
bool    doUnderLine{false};
POINT   points[2];

POINT&  begULine{points[0]};  // Begin Poly Underline Position
POINT&  endULine{points[1]};  // End Poly Underline Position
#endif
//  void nextBegin();
//  Tab& find(int curPos) {return ctx->find((curPos);}

//  int  xHmmToTxtExt(int   xHmm) {return MulDiv(xHmm, GetDeviceCaps(LOGPIXELSX), 254);}
//  int  yHmmToTxtExt(int   yHmm) {return MulDiv(yHmm, GetDeviceCaps(LOGPIXELSY), 254);}
#if 0
#include "DisplayBfr.h"
#include "DisplayFonts.h"
#include "TabList.h"
#include "UnderlineText.h"
#endif
//  friend class  DisplayFonts;

//  CSize getTextExtent(TCchar* txt, int n)        {return devDC.getExtent(txt, n);}
//  CSize getTextExtent(Cstring txt)               {return devDC.getExtent(txt);}
//  bool  getTextMetrics(TEXTMETRIC& metrics)      {return devDC.getMetrics(&metrics);}

//  void updateDC(CPrintDialogEx& dlg);
//  int  toHMM(String& s) {uint x;   return int(s.stod(x) * 254 + 0.5);}
//  int  toHMM(double  v) {return int(v * 254.0 + 0.5);}

  //bool textOut(int x, int y, TCchar* txt, int n) {return devDC()->TextOut(x, y, txt, n);}
//  bool textOut(int x, int y, Cstring txt)        {return devDC()->TextOut(x, y, txt);}
//  void drawLine(Point* pt, uint rgb);

//  void finEndUnderLine()              {ctx->finEndUnderLine();}
//  void cntnuUnderLine()            {ctx->cntnuUnderLine();}
//  void doUnderLine(Point* pts);                               // Draw the lines


//  void finBegUnderLine() {ctx->finBegUnderLine();}

//  void appendText(TCchar* txt)     {ctx->appendText(txt);}

//  int  xInToTxtExt(double xIn);
//  int  yInToTxtExt(double yIn);
//  void setFtrOffset();

//DisplayCtx*    prevCtx{0};                  // save and restore ctx

//bool           initialized{false};
//bool           wrap{true};
//DisplayCtx*    ctx{&bodyCtx};               // Current Context
//  void enableWrap()  {wrap = true;}
//  void disableWrap() {wrap = false;}
#if 0
//  void setLeftMgn( double v)       {ctx->setLeftMgn(v);}
//  void setRightMgn(double v)       {ctx->setRightMgn(v);}

  void setFace(TCchar* face)       {ctx->setFace(face);}
  void setPoint(int tenthsPt)      {ctx->setPoint(tenthsPt);}
  void setBold()                   {ctx->setBold();}
  void setItalic()                 {ctx->setItalic();}
  void setFUnderline()             {ctx->setFUnderline();}
  void setStrikeout()              {ctx->setStrikeout();}
  void popFont()                   {if (ctx->buf.doPopFont()) finPopFont();}

  void popAll()                    {ctx->fonts.popAll();  updateMetrics();}
  void updateMetrics()             {ctx->updateMetrics();}

  void clearTabs()                 {ctx->clearTabs();}
  void setTab(int val, bool right) {ctx->setTab(val, right);}
  void moveNextTab()               {ctx->moveNextTab();}

  void setCenter()                 {ctx->setCenter();}
  void setRight()                  {ctx->setRight();}

  void startUnderlining()          {ctx->startUnderlining();}
  bool isUnderlining()      {return ctx->isUnderlining();}
  bool drawUnderline()      {return ctx->drawUnderline();}
  void stopUnderlining()           {ctx->stopUnderlining();}

  void setText(TCchar* txt)        {ctx->setText(txt);}

  bool isAtLeftMgn() {return ctx->x <= leftMgn;}
  bool isBufEmpty()  {return ctx->isBufEmpty();}
#endif
#if 0
  void finLeftMgn( double v) {leftMgn  = xActualPixels(v);}
  void finRightMgn(double v) {rightMgn = xActualPixels(v);   ctx->xEnd = width - rightMgn;}
  int  xActualPixels(double inches) {return devDC.xActualPixels(inches);}
  int  yActualPixels(double inches) {return devDC.yActualPixels(inches);}

  void createFont(TCchar* face, int tenthsPt) {ctx->createFont(face, tenthsPt);}
  void finFace(TCchar* face)                  {ctx->finFace(face);}
  void finPoint(int tenthsPt)                 {ctx->finPoint(tenthsPt);}
  void finBold()                              {ctx->finBold();}
  void finItalic()                            {ctx->finItalic();}
  void finFUnderline()                        {ctx->finFUnderline();}
  void finStrikeout()                         {ctx->finStrikeout();}
  void finPopFont()                           {ctx->finPopFont();}

  void finClearTabs()                         {ctx->finClearTabs();}
  void finSetTab(int val, bool right)         {ctx->finSetTab(val, right);}
  void finMoveNextTab()                       {ctx->finMoveNextTab();}

  void finCenter()                            {ctx->finCenter();}
  void finRight()                             {ctx->finRight();}
#endif
//  void setCurCtx(DeviceCtx& pc);
//  void rcvPrvCtx();

//  bool examineFont();

