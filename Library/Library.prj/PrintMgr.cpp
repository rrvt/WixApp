// Print Manager -- needed to isolate variables from Display Management!


#include "stdafx.h"
#include "PrintMgr.h"
#include "CScrView.h"


// CScrView printing
/* The following functions are called for printing a page in the order given with one exception:
BOOL OnPreparePrinting(        CPrintInfo* pInfo);  -- 1st
void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);  -- 2nd
     CDC::StartDoc()                                -- 3rd      // Handled by CView
void OnPrepareDC(    CDC* pDC, CPrintInfo* pInfo);  -- 4th                         <-
     CDC::StartPage()                               -- 5th                          ^ // Handled by CView
void OnPrint(        CDC* pDC, CPrintInfo* pInfo);  -- 6th                          ^
     CDC::EndPage()                                 -- 7th then loops for each page ^ // Handled by CView
     CDC::EndDoc()                                  -- after last page                // Handled by CView
void OnEndPrinting(  CDC* pDC, CPrintInfo* pInfo);  -- last
*/


PrintMgr::PrintMgr(CScrView& view) : ShowMgr(_T("Print"), view, npd), orient(Portrait),
                                                                                  wrapEnabled(false)  { }


void PrintMgr::clear() {npd.clear();  dspDev.clear();}


bool PrintMgr::setOrient(HANDLE h, CDC* dc) {
DEVMODE* devMode;

  if (!h) return false;

  devMode = (DEVMODE*)::GlobalLock(h);    if (!devMode) return false;

  switch (orient) {
    case Portrait : devMode->dmOrientation = DMORIENT_PORTRAIT;               // portrait mode
                    devMode->dmFields     |= DM_ORIENTATION; break;

    case Landscape: devMode->dmOrientation = DMORIENT_LANDSCAPE;              // landscape mode
                    devMode->dmFields     |= DM_ORIENTATION; break;
    }

  if (dc) dc->ResetDC(devMode);

  ::GlobalUnlock(h); return true;
  }




PrtrOrient PrintMgr::getOrient(HANDLE h) {
DEVMODE*   devMode;
short      dmOrient;
PrtrOrient orient = Portrait;

  if (!h) return orient;

  devMode = (DEVMODE*)::GlobalLock(h);    if (!devMode) return orient;

  if ((devMode->dmFields & DM_ORIENTATION) != 0) {

    dmOrient = devMode->dmOrientation;

    if (     dmOrient == DMORIENT_PORTRAIT)  orient = Portrait;
    else if (dmOrient == DMORIENT_LANDSCAPE) orient = Landscape;
    }

  ::GlobalUnlock(h); return orient;
  }


void PrintMgr::OnPrepareDC(CDC* dc, CPrintInfo* info) {                       // Override

  cdc = dc;  pinfo = info;

  preparePrinter(dc, info);

  if (info->m_bPreview) preview(dc, info);
  }



// Get printer dialog box

BOOL PrintMgr::OnPreparePrinting(CPrintInfo* info) {

  printing = true; outputDone = false; startDocDone = false;

  if (vw.DoPreparePrinting(info)) return true;

  printing = false; return false;
  }


bool PrintMgr::finPreparePrinting(bool rslt)
                                  {outputDone = startDocDone = false; printing = rslt;  return printing;}


// Initialize dc for printer and other initialization, called for each page
//BOOL ResetDC(const DEVMODE* lpDevMode);

void PrintMgr::preparePrinter(CDC* dc, CPrintInfo* info) {
int     pageNo = info->m_nCurPage;
DEVMODE devMode;

  memset(&devMode, 0, sizeof(devMode));

  dc->ResetDC(&devMode);

  info->m_bContinuePrinting = true;     endPrinting = false;

  info->m_nNumPreviewPages = 0;

  info->SetMinPage(1);   info->SetMaxPage(9999);

  dspDev.clear();

  dspDev.setHorzMgns(leftMargin, rightMargin);   dspDev.setVertMgns(topMargin, botMargin);

  dspDev.preparePrinting(font, fontSize, dc, info);

  if (!outputDone) {outputDone = true; vw.onPrepareOutput(true);} // Only need to prepare data once
  }


int PrintMgr::noLinesPrPg() {
int maxLines = 9999;
int i;
int mxPgs;

  suppressOutput();

  npd.clear();

  for (i = 0; i < 75; i++) {String s; s.format(_T("Line %i"), i);     npd << s << nCrlf;}

  trialRun(maxLines, mxPgs);   return maxLines;
  }


// To determine number of lines in page and number of pages this is run twice for each printed output

void  PrintMgr::trialRun(int& maxLines, int& noPages) {
uint i;

  dspDev.startDev();   dspDev.clrLines();

  for (i = 0; !dspDev.isEndDoc(); i++) {

    dspDev.preparePrinting(font, fontSize, cdc, pinfo);

    dspDev.suppressOutput();

    if (wrapEnabled) dspDev.enableWrap(); else dspDev.disableWrap();

    dspDev();   dspDev.clrFont();
    }

  maxLines = dspDev.maxLines();    noPages = i;

  if (noPages == 1) maxLines++;  pinfo->m_nCurPage = 1;   preparePrinter(cdc, pinfo);
  }


// The OnPrint function is used to output to the preview window.  This function is required to find
// the next page to display.  Useful to contol the paging in my program rather than in CView...

void PrintMgr::preview(CDC* dc, CPrintInfo* info) {
uint i;

  dspDev.startDev();

  for (i = 1; i < info->m_nCurPage; i++) {

    dspDev.suppressOutput();

    if (wrapEnabled) dspDev.enableWrap(); else dspDev.disableWrap();

    dspDev();   dspDev.clrFont();

    if (!isFinishedPrinting(info)) dspDev.preparePrinting(font, fontSize, dc, info);
    }
  }



// Draw on Printer (i.e. Output to Printer)

void PrintMgr::OnPrint(CDC* dC, CPrintInfo* info) {

  if (wrapEnabled) dspDev.enableWrap(); else dspDev.disableWrap();

  dspDev();   startFooter(dspDev.getDisplay(), info);   dspDev.clrFont();

  if (isFinishedPrinting(info)) {printing = false;  endPrinting = true;}
  }


// The output location details are initialized and then the virtual function printFooter is called.
// The user may provide a footer function patterned after PrintMgr's version below.

void PrintMgr::startFooter(Device& dev, CPrintInfo* pInfo)
                        {dev.setFooter();   vw.printFooter(dev, pInfo->m_nCurPage);   dev.clrFooter();}


// Default footer for dev output.

void PrintMgr::printFooter(Device& dev, int pageNo) {          // Overload if different footer desired.

  if (!leftFooter.empty()) dev << leftFooter;

  dev << dCenter << toString(pageNo);

  if (!date.isEmpty()) {dev << dRight; dev << date.getDate() << _T("   ") << date.getTime();}

  dev << dFlushFtr;
  }


// The most secure way to cease printed output is to change the continuePrinting value in the printer
// info structure from true to false.  This function determinse that by examining the progress of the
// passage through the NotePad list of entities.

bool PrintMgr::isFinishedPrinting(CPrintInfo* info) {
bool fin = dspDev.isEndDoc();

  if (info) {info->m_bContinuePrinting = !fin;   if (fin) info->SetMaxPage(info->m_nCurPage);}

  return fin;
  }

