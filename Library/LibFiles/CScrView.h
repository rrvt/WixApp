// Scroll View


#pragma once
#include "DisplayDev.h"


class CScrView : public CScrollView {

// Attributes

DisplayDev  display;
DisplayDev  printer;

String      license;
Date        licDate;

String      font;
int         fontSize;                 // in Points
String      author;

static int  lastPos;

public:

  CScrView() : font(_T("Arial")), fontSize(120), author(_T("sde")) { }
 ~CScrView() { }

  void setAuthor(TCchar* a) {author = a;}
  void setFont(  TCchar* f, int points = 120) {font = f; fontSize = points < 70 ? points * 10 : points;}

  virtual void OnDraw(CDC* pDC);  // overridden to draw this view
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

  void    invalidate() {Invalidate();}


  virtual void OnInitialUpdate(); // called first time after construct
  virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

private:

  virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
  virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
  virtual void OnPrepareDC(    CDC* pDC, CPrintInfo* pInfo = NULL);
  virtual void OnPrint(        CDC* pDC, CPrintInfo* pInfo);
  virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
  virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);

          void preparePrinter(CDC* pDC, CPrintInfo* pInfo);
          void prepareDisplay(CDC* pDC);
          void setScrollSize();

          void print(CPrintInfo* pInfo);
          void printFooter(int pageNo);

protected:

  DECLARE_MESSAGE_MAP()
  };
