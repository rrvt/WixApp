// Vertical Display Management


#pragma once
#include "DevCtx.h"


class VertMgmt {

DevCtx&   dvx;
int       maxHeight;                            // Max character height to deal with font changes

int       y;
int       yMax;
int       topBnd;                               // The upper and lower text output region, computed
int       botBnd;

bool      beginPage;
bool      endPage;

public:

  VertMgmt(DevCtx& devCtx);
  VertMgmt() : dvx(*(DevCtx*)0) { }
 ~VertMgmt() { }

  void clear();

  void initBounds();

  void getMaxHeight() {if (dvx.chHeight > maxHeight) maxHeight = dvx.chHeight;}


  void setBeginPage()    {y = yMax = topBnd; beginPage = true; endPage = false;}
  bool isBeginPage()     {if (beginPage) {beginPage = false; return true;}  return false;}
  void atEndPageCond()   {if (endPage) {endPage = false; beginPage = true;}}
  bool isEndPage()       {return endPage && y > topBnd;}
  bool lf(bool printing, bool footer);

  void setMaxY(int v) {if (v > yMax) yMax = v;}

  bool exceedsBnd();
  void setEndPage() {endPage = true;}
  void setBottom()  {y = y + maxHeight < botBnd ? botBnd : y + maxHeight/8;}

  int  pos()          {return y;}
  int  getUlinePos()  {return y + dvx.uLineDelta;}
  int  heightCh()     {return dvx.chHeight;}
  int  maxY()         {return yMax;}
  bool withinBounds() {return topBnd < y && y < botBnd;}

private:

  void initY();
  friend class DevBase;
  };




// bool printing

//int    noLines;
//int    maxLines;
//  void clrLines()        {noLines = 0;}
//  int  getMaxLines()     {return maxLines;}

//int       pageHght;                           // Input Parameters
//double    topMargin;                          // Top and Bottom Margins
//double    botMargin;
//  void setHeight(CDC* dc);
//  void clearMax() {maxHeight = 0;}

//int       chHeight;                           // Text height
//int       maxHeight;                          // Max text height to deal with font changes
//int       uLineDelta;                         // Y delta from y for the line option

