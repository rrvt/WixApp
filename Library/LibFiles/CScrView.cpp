// Scroll View


#include "stdafx.h"
#include "CScrView.h"
#include "NotePad.h"


int CScrView::lastPos = 0;


BEGIN_MESSAGE_MAP(CScrView, CScrollView)
  ON_COMMAND(ID_FILE_PRINT,         &CScrollView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_DIRECT,  &CScrollView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()


BOOL CScrView::PreCreateWindow(CREATESTRUCT& cs) {return CScrollView::PreCreateWindow(cs);}


void CScrView::OnInitialUpdate() {CScrollView::OnInitialUpdate();}


void CScrView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
                                        {setScrollSize(); CScrollView::OnUpdate(pSender, lHint, pHint);}


// CScrView printing
/* The following functions are called for printing a page in the order given with one exception:
BOOL OnPreparePrinting(        CPrintInfo* pInfo);  -- 1st
void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);  -- 2nd
     CDC::StartDoc()                                -- 3rd
void OnPrepareDC(    CDC* pDC, CPrintInfo* pInfo);  -- 4th                         <-
     CDC::StartPage()                               -- 5th                          ^
void OnPrint(        CDC* pDC, CPrintInfo* pInfo);  -- 6th                          ^
     CDC::EndPage()                                 -- 7th then loops for each page ^
     CDC::EndDoc()                                  -- after last page
void OnEndPrinting(  CDC* pDC, CPrintInfo* pInfo);  -- last
*/

BOOL CScrView::OnPreparePrinting(CPrintInfo* pInfo) {

  pInfo->m_nNumPreviewPages = 0;

  pInfo->SetMinPage(1);   pInfo->SetMaxPage(9999);

  return DoPreparePrinting(pInfo);                  // Get printer dialog box
  }


void CScrView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) {Date d; d.getToday();  printer.startDev();}


void CScrView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) {

  CScrollView::OnPrepareDC(pDC, pInfo);

  if (pDC->IsPrinting()) preparePrinter(pDC, pInfo);
  else                   prepareDisplay(pDC);
  }


void CScrView::preparePrinter(CDC* pDC, CPrintInfo* pInfo) {
int i;

  pInfo->m_bContinuePrinting = true;

  if (pInfo->m_bPreview) {
    if (pInfo->m_nNumPreviewPages == 2) pInfo->m_nNumPreviewPages = 1;

    if (pInfo->m_nCurPage < printer.lastPageNo) {

      printer.startDev();

      for (i = 0; printer.getNoPages() < pInfo->m_nCurPage-1 && !printer.isEndDoc(); i++) {

        printer.preparePrinting(font, fontSize, pDC, false); printer.beginPrinting(pInfo);

        print(pInfo);
        }
      }
    }

  else if (printer.isEndDoc()) printer.startDev();

  printer.lastPageNo = pInfo->m_nCurPage;

  printer.preparePrinting(font, fontSize, pDC, true); printer.beginPrinting(pInfo);
  }



void CScrView::OnPrint(CDC* pDC, CPrintInfo* pInfo) {print(pInfo);}


// Draw on Printer (i.e. Output to Printer)

void CScrView::print(CPrintInfo* pInfo) {

  printer.toDevice();

  printFooter(pInfo->m_nCurPage);

  pInfo->m_bContinuePrinting = !printer.isEndDoc();

  if (printer.isEndDoc()) {pInfo->SetMaxPage(pInfo->m_nCurPage);}

  printer.clrFont();
  }



void CScrView::printFooter(int pageNo) {printer.printFooter(pageNo, license, licDate);}


void CScrView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) {if (!pInfo->m_bPreview) invalidate();}



// Display Functions

void CScrView::prepareDisplay(CDC* pDC)
                                 {display.startDev(); display.prepareDisplay(font, fontSize, pDC, true);}


// CScrView drawing

void CScrView::OnDraw(CDC* pDC) {display.toDevice();   display.clrFont();   setScrollSize();}


//  SB_LINEUP           0
//  SB_LINELEFT         0
//  SB_LINEDOWN         1
//  SB_LINERIGHT        1
//  SB_PAGEUP           2
//  SB_PAGELEFT         2
//  SB_PAGEDOWN         3
//  SB_PAGERIGHT        3
//  SB_THUMBPOSITION    4
//  SB_THUMBTRACK       5
//  SB_TOP              6
//  SB_LEFT             6
//  SB_BOTTOM           7
//  SB_RIGHT            7
//  SB_ENDSCROLL        8


BOOL CScrView::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll) {
int        x  = nScrollCode >> 8;
POINT      pt;
int        nextPos;
int        delta;

  if (x == SB_THUMBTRACK) {

    nextPos = nPos & 0x0000ffff;    pt = GetScrollPosition();

    delta   = nextPos - lastPos;

    if (abs(delta) > 0x6000) {
      if (lastPos > nextPos) delta += 0x10000;
      else                   delta -= 0x10000;
      }
    lastPos = nextPos;  pt.y += delta;  ScrollToPosition(pt); return true;
    }

  return CScrollView::OnScroll(nScrollCode, nPos, bDoScroll);
  }


void CScrView::setScrollSize() {
RECT  winSize;
int   height = display.getHeight();
int   t      = 1;
CSize scrollViewSize;
CSize pageSize;
CSize scrollSize;

  GetClientRect(&winSize);

  if (height) {t = (winSize.bottom - 1) / height; t *= height;}

  pageSize.cy = t; pageSize.cx = winSize.right;

  scrollSize.cx = display.getWidth();   scrollSize.cy = height;

  display.getMaxPos(scrollViewSize.cx, scrollViewSize.cy);

  SetScrollSizes(MM_TEXT, scrollViewSize, pageSize, scrollSize);
  }



#if 0
void CScrView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* sb) {

if (nPos > 1000) {
int x = 1;
}

  CScrollView::OnVScroll(nSBCode, nPos, sb);
  }
#endif
#if 0
void CScrView::getScrollSize(RECT& winSize, CSize& s, CSize& pageSize, CSize& scrollSize) {
int   maxX   = 0;
int   maxY   = 0;
int   height = 0;
int   t      = 0;

  display.dev.getMaxPos(maxX, maxY); height = display.dev.getHeight();

  if (height) {t = (winSize.bottom - 1) / height; t *= height;}
  else         t = 1;

  pageSize.cy = t; pageSize.cx = winSize.right;

  scrollSize.cx = display.dev.getWidth(); scrollSize.cy = height;

  s.cx = maxX; s.cy = maxY;
  }
#endif
#if 0
void CScrView::repositionTo(TCchar* face, int fontSize, CDC* pDC, CPrintInfo* pInfo) {
int i;

  printer.startDev();

  for (i = 0; printer.dev.noPages < pInfo->m_nCurPage-1 && !printer.endDoc; i++) {

    printer.dev.beginPrinting(pDC, pInfo, false); printer.dev.preparePrinting(face, fontSize);

    print( pInfo);
    }
  }


int i;

  if (printer.dev.withinBounds())
    for (i = 0; !printer.dev.isEndPage() && i < 65; i++) printer.dev << dCrlf;

  if (!license.empty()) printer.dev << license;

  printer.dev << dCenter << toString(pageNo);

  if (!licDate.isEmpty()) {printer.dev << dRight; printer.dev << licDate.getDate();}

  printer.dev << dflushFtr;
  }
#endif

