// Vertical Display Management


#pragma once


class VertMgmt {

int    y;
int    yMax;
int    chHeight;                          // Text height
int    maxHeight;                         // Max text height to deal with font changes
int    uLineDelta;                        // Y delta from y for the line option

int    botEdge;                           // Input Parameters
double topMgn;                            // Top and Bottom Margins
double botMgn;

int    topBnd;                            // The upper and lower text output region, computed
int    botBnd;

bool   beginPage;
bool   endPage;
int    noLines;
int    maxLines;

public:

  VertMgmt();
 ~VertMgmt() { }

  void clear();

  void setAttributes(int height, double topMargin, double botMargin);
  void setTopMargin(double v);
  void setBotMargin(double v);
  void setHeight(CDC* dc);

  bool exceedsBnd(int n);       // {return y + n * maxHeight > botBnd;}
  void setBeginPage()    {beginPage = true; endPage = false;}
  bool isBeginPage()     {if (beginPage) {beginPage = false; return true;}  return false;}
  void atEndPageCond()   {if (endPage) {endPage = false; beginPage = true; noLines = 0;}}
  bool isEndPage()       {return endPage && y > topBnd;}
  bool lf(bool printing, bool footer);
  void clrLines()        {maxLines = 0; noLines = 0;}
  int  getMaxLines()     {return maxLines;}

  void setMaxY(int v) {if (v > yMax) yMax = v;}
  void setEndPage();          //{y = botBnd; endPage = true;}
  void setBottom() {if (y < botBnd) y = botBnd;}

  int  pos()          {return y;}
  int  getUlinePos()  {return y + uLineDelta;}
  int  heightCh()     {return chHeight;}
  int  maxY()         {return yMax;}
  bool withinBounds() {return topBnd < y && y < botBnd;}

private:

  void initY();
  };

