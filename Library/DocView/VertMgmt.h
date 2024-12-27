// Vertical Display Management


#pragma once

class DeviceContext;

class VertMgmt {

int  y;
int  yMax;
int  maxHeight;                           // Max character height to deal with font changes
int  pgHeight;
int  topBnd;                              // The upper and lower text output region, computed
int  botBnd;

bool beginPage;
bool endPage;

bool printing;                            // Printing when true, else displaying
bool footer;                              // Avoid Line Feed when printing and not in footer and
                                          // would exceed bottom of page
public:

int  charHeight;                          // Text height
int  uLineDelta;                          // Y delta from y for the line option

  VertMgmt();
 ~VertMgmt() { }

  void clear();
  VertMgmt& operator= (VertMgmt& vrt);

  void setPgHeight(int pgHght) {pgHeight = pgHght;}
  void initBounds(double topMgn, double botMgn);
  bool setPrinting(bool on) {return printing = on;}
  bool setFooter(bool on)   {return footer   = on;}

  bool isPrinting() {return printing;}

  void getMaxHeight();

  void setBeginPage()    {y = yMax = topBnd; beginPage = true; endPage = false;}
  bool isBeginPage()     {if (beginPage) {beginPage = false; return true;}  return false;}
  void atEndPageCond()   {if (endPage) {endPage = false; beginPage = true;}}
  bool isEndPage()       {return endPage && y > topBnd;}
  bool lf(bool printing, bool footer);
  bool lf();

  void setMaxY(int v) {if (v > yMax) yMax = v;}

  bool exceedsBnd();
  void setEndPage() {endPage = true;}
  void setBottom()  {if (printing) {y = y + maxHeight < botBnd ? botBnd : y + maxHeight/8;}}

  int  pos()          {return y;}
  int  getUlinePos();                       //  {return y + dvx.uLineDelta;}
  int  maxY()         {return yMax;}
  bool withinBounds() {return printing && topBnd < y && y < botBnd;}

private:

  void initY();
  friend class Device;
  };



//////-------------

//DeviceContext&   dvx; DeviceContext& devCtx

