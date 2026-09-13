// Printer Manager -- Interface to Windows


#pragma once
#include "CDCex.h"
#include "DeviceMgr.h"
#include "PrinterCtx.h"



class PrinterMgr : public DeviceMgr {

PrinterCtx     hdrCtx{*this};
PrinterCtx     bodyCtx{*this};
PrinterCtx     ftrCtx{*this};

protected:
public:

       PrinterMgr(CDCex& cdcEx) : DeviceMgr(bodyCtx, cdcEx)  { }
      ~PrinterMgr() {cdcEx.~CDCex();}

  bool prepare(HDC dc);

  void setFonts(TCchar* face, int tenths);      // Face and tenths of point

  bool startDoc(TCchar* docName);
  bool startPage();
  void updateCoordinates();                     // Starting Coordinates are modified by header

  bool output();
  void finLine() {if (ctx->x > leftMgn) crlf();}


  void setLineHeight(int deltaY)  { }     // used for scrolling
  void setCharWidth(int avgWidth) { }
  void setMaxX(int x)             { }

  bool endPage();
  bool endDoc()    {return suppress || cdcEx.endDoc();}

  void setHdr(TCchar* face, int tenthsPt);
  void startHeader();
  void endHeader();

  void setFtr(TCchar* face, int tenthsPt);
  void startFooter();
  void endFooter();

  bool isBodyCtx()   {return ctx == &bodyCtx;}

  bool flush()       {return ctx->flush();}

private:


  PrinterMgr() : DeviceMgr(*(DeviceCtx*)0, *(CDCex*)0) { }

  friend class PrinterCtx;
  friend class BufferMgr;
  friend class PrintNtPd;
  };




///////-----------------

//  void SetAdobePDFOutputName(String& sOutputFilePath);
//  void hmmToTxtExt(CSize& sz);
//  bool getTextMetrics();
//  void getLogicalUnits();
#if 0
Fonts      bodyFonts{this};
Fonts      hdrFonts{this};
Fonts      ftrFonts{this};
Fonts*     curFonts{&bodyFonts};

TabList    bodyTabs;
TabList    hdrTabs;
TabList    ftrTabs;

Fonts*     prevFonts{0};
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
#include "BufferMgr.h"
#include "Fonts.h"
#include "TabList.h"
#include "UnderLine.h"
#endif
#if 0
  void finEndUnderLine()              {ctx->finEndUnderLine();}
  void cntnuUnderLine()            {ctx->cntnuUnderLine();}
  void doUnderLine(Point* pts);                               // Draw the lines
#endif
//  void appendText(TCchar* txt)     {ctx->appendText(txt);}
//  void setOutput(TCchar* outputName) {docInfo.lpszOutput  = outputName;}
  //  void finBegUnderLine() {ctx->finBegUnderLine();}

  //  void updateDC(CPrintDialogEx& dlg);
//  int  xInToTxtExt(double xIn)  {return devDC.xInToTxtExt(xIn);}
//  int  yInToTxtExt(double yIn)  {return devDC.yInToTxtExt(yIn);}

//  void drawLine(Point* pt, uint rgb);


//  int  toHMM(String& s) {uint x;   return int(s.stod(x) * 254 + 0.5);}
//  int  toHMM(double  v) {return int(v * 254.0 + 0.5);}
//String         printerName;

//  bool textOut(int x, int y, TCchar* tc, int n)
//                                         {return suppress ? true : devDC.textOut(x, y, tc, n);}
//  void attach(HDC hdc) {devDC.attach(hdc);}
//  void finLeftMgn( double v) {leftMgn  = xActualPixels(v);}
//  void finRightMgn(double v) {rightMgn = xActualPixels(v);   ctx->xEnd = width - rightMgn;}

//PrinterCtx*    prevCtx{0};                // save and restore ctx

//bool           initialized{false};
//bool           wrap{true};
#if 0
//PrinterCtx*    ctx{&bodyCtx};             // Current Context
bool           suppress{false};

int            width{0};                  // Dimensions in Pixels (Logical units)
int            length{0};
int            topMgn{0};
int            leftMgn{0};
int            rightMgn{0};
int            bottomMgn{0};
#endif
//  void setFtrOffset();
#if 0
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

  void finCenter() {ctx->finCenter();}
  void finRight()  {ctx->finRight();}
#endif
//  void setCurCtx(DeviceCtx& pc);
//  void rcvPrvCtx();
#if 0
//  void setLeftMgn( double v)       {ctx->setLeftMgn(v);}
  void setRightMgn(double v)       {ctx->setRightMgn(v);}

  void setFace(TCchar* face)       {ctx->setFace(face);}
  void setPoint(int tenthsPt)      {ctx->setPoint(tenthsPt);}
  void setBold()                   {ctx->setBold();}
  void setItalic()                 {ctx->setItalic();}
  void setFUnderline()             {ctx->setFUnderline();}
  void setStrikeout()              {ctx->setStrikeout();}
  void popFont()                   {if (ctx->buf.doPopFont())           finPopFont();}

  void popAll()                    {ctx->fonts.popAll();                updateMetrics();}
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
  //, devDC(*(CDCex*)0) { }
       /*, devDC(deviceContext){ } */
//CDCex& devDC;

