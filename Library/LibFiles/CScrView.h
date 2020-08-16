// Scroll View


#pragma once
#include "DisplayDev.h"


// CScrView printing
/* The following functions are called for printing a page in the order given with one exception:
void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);  -- 1st
BOOL OnPreparePrinting(        CPrintInfo* pInfo);  -- 2nd
     CDC::StartDoc()                                -- 3rd      // Handled by CView
void OnPrepareDC(    CDC* pDC, CPrintInfo* pInfo);  -- 4th                         <-
     CDC::StartPage()                               -- 5th                          ^ // Handled by CView
void OnPrint(        CDC* pDC, CPrintInfo* pInfo);  -- 6th                          ^
     CDC::EndPage()                                 -- 7th then loops for each page ^ // Handled by CView
     CDC::EndDoc()                                  -- after last page                // Handled by CView
void OnEndPrinting(  CDC* pDC, CPrintInfo* pInfo);  -- last
*/


enum PrtrOrient {Portrait, Landscape};


class CScrView : public CScrollView {

PrtrOrient  orient;
bool        printing;
bool        endPrinting;
bool        outputDone;
bool        startDocDone;
bool        wrapEnabled;

DisplayDev  display;
DisplayDev  printer;

String      font;
int         fontSize;                 // in 10* Points
CDC*        dc;
CPrintInfo* info;

static int  lastPos;

double      leftMargin;
double      rightMargin;
double      topMargin;
double      botMargin;

public:

String      rightFooter;                                          // Data to print at right side of footer
Date        date;                                                 // Date to print at left edge of footer

  CScrView() : orient(Portrait), printing(false), endPrinting(false), outputDone(false),
               startDocDone(false), wrapEnabled(true), font(_T("Arial")), fontSize(120), dc(0), info(0),
               leftMargin(1.0), rightMargin(1.0), topMargin(1.0), botMargin(1.0) {}
 ~CScrView() { }

  void setFont(  TCchar* f, int points = 120) {font = f; fontSize = points < 70 ? points * 10 : points;}

  virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
  virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
  virtual void onPrepareOutput();
  virtual void OnDraw(CDC* pDC);                                  // overridden to draw this view

          void     invalidate() {Invalidate();}

          void     setOrientation(PrtrOrient orientation) {orient = orientation;}
          bool     setPrntrOrient(HANDLE h, CDC* dc = 0);

  virtual void     trialRun(int& maxLines, int& noPages);
  virtual void     OnPrint(CDC* pDC, CPrintInfo* pInfo);
  virtual void     OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
  virtual void     printFooter(Display& dev, int pageNo);         // Overload if different footer desired
          bool     isPrinting() {return printing;}

          void     setHorzMgns(double left, double right) {leftMargin = left; rightMargin = right;}
          void     setVertMgns(double top,  double bot)   {topMargin  = top;  botMargin   = bot;}

          Display& getDev() {return printing ? printer.getDisplay() : display.getDisplay();}

          void     suppressOutput() {if (printing) printer.suppressOutput();}
          void     negateSuppress() {if (printing) printer.negateSuppress();}
          void     disableWrap()    {wrapEnabled = false;}
          void     enableWrap()     {wrapEnabled = true;}

protected:

  virtual BOOL     OnPreparePrinting(CPrintInfo* pInfo);

private:

  virtual BOOL     OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);

          void     preparePrinter(CPrintInfo* pInfo);
          void     preview(CPrintInfo* pInfo);
          void     prepareDisplay();
          void     setScrollSize();

          void     print(             CPrintInfo* pInfo);
          void     startFooter(       CPrintInfo* pInfo, Display& dev);
          bool     isFinishedPrinting(CPrintInfo* pInfo);
protected:

  DECLARE_MESSAGE_MAP()
  };
