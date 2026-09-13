// Scroll View Base Class


#include "pch.h"
#include "ScrollView.h"
#include "CDCex.h"
#include "DisplayNtPd.h"
#include "Printer.h"
//#include "PrinterMgr.h"


int ScrollView::lastPos{0};


ScrollView::ScrollView(TCchar* name) : dm(displayDC), appName(name) {  }


void ScrollView::OnInitialUpdate() {
DisplayInfo info;

  CScrollView::OnInitialUpdate();

  onPrepareDisplay(info);    docName = info.docName;
  }


void ScrollView::OnPrepareDC(CDC* dc, CPrintInfo* info) {

  GetClientRect(&winSize);

  dm.prepare(dc, winSize);

  CScrollView::OnPrepareDC(dc, info);   //cdc = dc;
  }


void ScrollView::OnDraw(CDC* dc) {
DisplayNtPd displayNtPd(onPrepareOutput(), dm, *this);

  displayDC = dc;   setHeader(displayNtPd);   setFooter(displayNtPd);

  displayNtPd(docName);   setScrollSize();
  }


// Printing Commands

void ScrollView::printerSetup() {
PrinterInfo info;
PrinterDlg  dlg(info, this);

  onPreparePrinter(info);   dlg.DoModal();
  }


void ScrollView::printFile(bool getNoPages) {
PrinterInfo info;
String      docName;
PrinterDlg  dlg(info, this);
PrinterHdl  printerHdl;
PrinterMgr  printerMgr(printerDC);

  onPreparePrinter(info);

  docName = !info.docName.isEmpty() ? info.docName : appName;

  if (dlg.DoModal() != IDOK) return;                    // Saves data in prtrDevAttr

  if (!printerMgr.prepare(printerHdl())) return;        // create DC from data in prtrDevAttr

  PrintNtPd prntNp(printerMgr, onPreparePrinting());

  setHeader(prntNp);   setFooter(prntNp);   if (getNoPages) prntNp.getNoPages(docName);

  prntNp(docName);
  }


/*
typedef struct tagSIZE {
  LONG cx;
  LONG cy;
} SIZE, *PSIZE, *LPSIZE;
*/

void ScrollView::setScrollSize() {
int   height = dm.getLineHeight();
int   t      = 1;
CSize scrollViewSize;
CSize pageSize;
CSize scrollSize;

  if (height) {t = (winSize.Height() + height - 1) / height;   t *= height;}

  pageSize.cy = t; pageSize.cx = winSize.right;

  scrollSize.cx = dm.getCharWidth();   scrollSize.cy = height;

  scrollViewSize.cx = dm.getMaxX();   scrollViewSize.cy = dm.getMaxY();

  SetScrollSizes(MM_TEXT, scrollViewSize, pageSize, scrollSize);
  }


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


BOOL ScrollView::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll) {              // Override
int   x  = nScrollCode >> 8;
POINT pt;
int   nextPos;
int   delta;

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




void ScrollView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/) {
  setScrollSize();
  }




////////////////-------------

#if 0
void ScrollView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) {
  // TODO: Add your specialized code here and/or call the base class

  CScrollView::OnBeginPrinting(pDC, pInfo);
  }

void ScrollView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) {
  // TODO: Add your specialized code here and/or call the base class

  CScrollView::OnEndPrinting(pDC, pInfo);
  }


BOOL ScrollView::OnPreparePrinting(CPrintInfo* pInfo) {

  // TODO:  call DoPreparePrinting to invoke the Print dialog box

  return CScrollView::OnPreparePrinting(pInfo);
  }


void ScrollView::OnPrint(CDC* pDC, CPrintInfo* pInfo) {
  // TODO: Add your specialized code here and/or call the base class

  CScrollView::OnPrint(pDC, pInfo);
  }

void ScrollView::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView) {
  // TODO: Add your specialized code here and/or call the base class

  CScrollView::OnEndPrintPreview(pDC, pInfo, point, pView);
  }
#endif
















////////--------

#if 0
void ScrollView::setScrollSize() {
RECT  winSize;
int   height = 1;                       //ntPdToDev.chHeight();
int   t      = 1;
CSize docAttr;
CSize pageAttr;
CSize lineAttr;

  getDocAttr(docAttr);

  GetClientRect(&winSize);

  if (height) {t = (winSize.bottom - 1) / height; t *= height;}

  pageAttr.cy = t; pageAttr.cx = winSize.right;

  lineAttr.cx = charWidth;   lineAttr.cy = charHeight;

  SetScrollSizes(MM_TEXT, docAttr, pageAttr, lineAttr);
  }
#endif

