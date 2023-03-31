// Print Manager -- needed to isolate variables from Display Management!


#include "pch.h"
#include "PrintMgr.h"
#include "CApp.h"
#include "CScrView.h"
#include "Printer.h"

#include "MessageBox.h"

/* Doc/View Framework Calls to implement printing
     CMyView::OnPreparePrinting    o Set length of doc if known
             |                     o Call DoPreparePrining to display Print dialog box which creates DC
             V
     CMyView::OnBeginPrinting      o Set length of document based on DC
             |                     o Allocate DGI resources
             V
         CDC::StartDoc
             |
             V
  |->CMyView::OnPrepareDC          o Change DC attributes
  |          |                     o Check for end of document
  |          V
  |      CDC::StartPage
  |          |
  |          V
  |  CMyView::OnPrint              o Print specified page, including Headers and Footers
  |          |
  |          V
  -------CDC::EndPage
             |
             V
         CDC::EndDoc
             |
             V
     CMyView::OnEndPrinting        o Deallocate GDI resources
*/

/* The following functions are called for printing a page in the order given with one exception:
BOOL OnPreparePrinting(        CPrintInfo* pInfo);  -- 1st                    // handled by CScrView
void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);  -- 2nd
     CDC::StartDoc()                                -- 3rd
void OnPrepareDC(    CDC* pDC, CPrintInfo* pInfo);  -- 4th
     CDC::StartPage()                               -- 5th
void OnPrint(        CDC* pDC, CPrintInfo* pInfo);  -- 6th
     CDC::EndPage()                                 -- 7th then loops for each page
     CDC::EndDoc()                                  -- after last page
void OnEndPrinting(  CDC* pDC, CPrintInfo* pInfo);  -- last
*/


PrintMgr::PrintMgr(CScrView& view) : ShowMgr(view, npd, pageOut), pageOut(npd), dc(0), info(0),
                                     endPrinting(false), pageNo(0) { }



void PrintMgr::onBeginPrinting(CDC* cdc, CPrintInfo* pInfo) {

  clear();   dc = cdc;   info = pInfo;   info->m_bContinuePrinting = true;

  info->m_nNumPreviewPages = 1;

  info->SetMinPage(1);   info->SetMaxPage(9999);

  pageOut.prepare(dc, info);

  vw.onBeginPrinting();   pageOut.startDev();
  }


// To determine number of pages

int  PrintMgr::getNoPages() {
uint     i;

  prepareDC();   setMargins();   pageOut.startDev();   pageOut.suppressOutput();

  for (i = 0; !pageOut.isEndDoc(); i++) onePageOut();

  pageOut.negateSuppress(); return i;
  }


void PrintMgr::clear() {
  npd.clear();   pageOut.clear();   endPrinting = false;
  leftFooter.clear();   date.clear();   pageNo = 0;
  }


void PrintMgr::onPrepareDC(CDC* pDC, CPrintInfo* pInfo) {                       // Override

  prepareDC();   setMargins();

  if (info->m_bPreview) findNextPreviewPage(dc, info);
  }


void PrintMgr::prepareDC() {
DEVMODE devMode;

  memset(&devMode, 0, sizeof(devMode));   dc->ResetDC(&devMode); // sets the Addtribut devmode parameter

  pageOut.initFont(font, fontSize);
  }


void PrintMgr::setHorzMgns() {
double leftMgn;
double rightMgn;

  pageNo   = info->m_nCurPage;

  leftMgn  = pageNo & 1 ? printer.leftOdd  : printer.leftEven;
  rightMgn = pageNo & 1 ? printer.rightOdd : printer.rightEven;

  pageOut.setHorzMgns(leftMgn, rightMgn);
  }


// Find the next preview page by suppressing the output of preceding pages.
// The OnPrint function is used to output to the preview window.

void PrintMgr::findNextPreviewPage(CDC* dc, CPrintInfo* info) {
uint i;

  endPrinting = false;   pageOut.clearOps();   pageOut.suppressOutput();   pageOut.startDev();

  for (i = 1; i < info->m_nCurPage; i++) onePageOut();

  pageOut.negateSuppress();
  }


// Draw on Printer (i.e. Output to Printer) -- Called by Windows after OnPrepareDC (thru CScrView)

void PrintMgr::onPrint(CDC* cdc, CPrintInfo* pInfo) {

  onePageOut();

  endPrinting = isFinishedPrinting(info);
  }


void PrintMgr::onePageOut() {
DevBase& dev = pageOut.getDev();

  dev.setBeginPage();

  dev.startContext();   dev.disableWrap();   vw.printHeader(dev, pageNo);   dev.endContext();

    if (wrapEnabled) dev.enableWrap(); else dev.disableWrap();   pageOut();

  dev.setFooter();   dev.disableWrap();   vw.printFooter(dev, pageNo);   dev.clrFooter();
  }


// The most secure way to cease printed output is to change the continuePrinting value in the printer
// info structure from true to false.  This function determinse that by examining the progress of the
// passage through the NotePad list of entities.

bool PrintMgr::isFinishedPrinting(CPrintInfo* info) {
bool fin = pageOut.isEndDoc();

  if (info) {info->m_bContinuePrinting = !fin;   if (fin) info->SetMaxPage(info->m_nCurPage);}

  return fin;
  }

