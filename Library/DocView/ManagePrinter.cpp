// Print Manager -- needed to isolate variables from Display Management!


#include "pch.h"
#include "ManagePrinter.h"
#include "CApp.h"
#include "CScrView.h"
#include "Printer.h"

#include "MessageBox.h"

/* Doc/View Framework Calls to implement printing
     CMyView::OnPreparePrinting    o Set length of doc if known
             |                     o Call DoPreparePrining to display Print dialog box which
             V                     o creates DC
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
BOOL OnPreparePrinting(        CPrintInfo* pInfo);  -- 1st                  // handled by CScrView
void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);  -- 2nd
     CDC::StartDoc()                                -- 3rd
void OnPrepareDC(    CDC* pDC, CPrintInfo* pInfo);  -- 4th
     CDC::StartPage()                               -- 5th
void OnPrint(        CDC* pDC, CPrintInfo* pInfo);  -- 6th
     CDC::EndPage()                                 -- 7th then loops for each page
     CDC::EndDoc()                                  -- after last page
void OnEndPrinting(  CDC* pDC, CPrintInfo* pInfo);  -- last
*/


ManagePrinter::ManagePrinter(CScrView& view) : ManageDevBase(view, npd, ntPdToDev),
                         ntPdToDev(npd, PrinterID), dc(0), info(0), endPrinting(false), pageNo(0) { }


void ManagePrinter::onBeginPrinting(CDC* cdc, CPrintInfo* pInfo) {

  clear();   dc = cdc;   info = pInfo;   info->m_bContinuePrinting = true;

  setFontScale(printer.scale);   info->m_nNumPreviewPages = 1;

  info->SetMinPage(1);   info->SetMaxPage(9999);

  ntPdToDev.prepare(dc, info);

  vw.onBeginPrinting();   ntPdToDev.startDev();
  }


// To determine number of pages

int  ManagePrinter::getNoPages() {
uint     i;

  prepareDC();   setMargins();   ntPdToDev.startDev();

  ntPdToDev.allowOutput(false);

    for (i = 0; !ntPdToDev.isEndDoc(); i++) onePageOut();

  ntPdToDev.allowOutput(true); return i;
  }


void ManagePrinter::clear() {
  npd.clear();   ntPdToDev.clear();   endPrinting = false;
  leftFooter.clear();   date.clear();   pageNo = 0;
  }


void ManagePrinter::onPrepareDC(CDC* pDC, CPrintInfo* pInfo) {                       // Override

  prepareDC();   setMargins();

  if (info->m_bPreview) findNextPreviewPage(dc, info);
  }


void ManagePrinter::prepareDC() {
DEVMODE devMode;

  memset(&devMode, 0, sizeof(devMode));

  dc->ResetDC(&devMode);                                  // sets the Addtribute devmode parameter

  ntPdToDev.initFont(font, fontSize);
  }


void ManagePrinter::setHorzMgns() {
double leftMgn;
double rightMgn;

  pageNo   = info->m_nCurPage;

  leftMgn  = pageNo & 1 ? printer.leftOdd  : printer.leftEven;
  rightMgn = pageNo & 1 ? printer.rightOdd : printer.rightEven;

  ntPdToDev.setHorzMgns(leftMgn, rightMgn);
  }


// Find the next preview page by suppressing the output of preceding pages.
// The OnPrint function is used to output to the preview window.

void ManagePrinter::findNextPreviewPage(CDC* dc, CPrintInfo* info) {
uint i;

  endPrinting = false;   ntPdToDev.allowOutput(false);   ntPdToDev.startDev();

  for (i = 1; i < info->m_nCurPage; i++) onePageOut();

  ntPdToDev.allowOutput(true);
  }


// Draw on Printer (i.e. Output to Printer) -- Called by Windows after OnPrepareDC (thru CScrView)

void ManagePrinter::onPrint(CDC* cdc, CPrintInfo* pInfo) {

  onePageOut();

  endPrinting = isFinishedPrinting(info);
  }


void ManagePrinter::onePageOut() {
Device&   dev = ntPdToDev.getDev();
DevStream devStrm(PrtHdrID);

  if (pageNo > 1) dev = pageBackup;

  dev.setBeginPage();

  devStrm.init(dev);
    devStrm.setWrapEnable(false);   vw.printHeader(devStrm, pageNo);
  dev.restore(PrtHdrID);

    dev.setWrapEnable(wrapEnabled);   ntPdToDev();

  devStrm.init(dev);   devStrm.setFooter(true);   devStrm.setBottom();
    devStrm.setWrapEnable(false);   vw.printFooter(devStrm, pageNo);
  devStrm.setFooter(false); dev.restore(PrtHdrID);    dev.nonBlankLine = false;

  pageBackup = dev;
  }


// The most secure way to cease printed output is to change the continuePrinting value in the
// printer info structure from true to false.  This function determinse that by examining the
// progress of the passage through the NotePad list of entities.

bool ManagePrinter::isFinishedPrinting(CPrintInfo* info) {
bool fin = ntPdToDev.isEndDoc();

  if (info) {info->m_bContinuePrinting = !fin;   if (fin) info->SetMaxPage(info->m_nCurPage);}

  return fin;
  }




////--------------------

#if 1
#else
//dev.startContext();   dev.disableWrap();   vw.printHeader(dev, pageNo);   dev.endContext();
#endif
#if 1
#else
//dev.setFooter();   dev.disableWrap();   vw.printFooter(dev, pageNo);   dev.clrFooter();
#endif

