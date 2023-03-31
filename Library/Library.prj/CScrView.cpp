// Scroll View


#include "pch.h"
#include "CScrView.h"
#include "CApp.h"
#include "Display.h"
#include "IniFile.h"
#include "Printer.h"


       TCchar* RptOrietnSect = _T("ReportOrientn");
static TCchar* NoteOrietnKey = _T("NotePad");


int CScrView::lastPos = 0;


BEGIN_MESSAGE_MAP(CScrView, CScrollView)
  ON_COMMAND(ID_FILE_PRINT,         &CScrollView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
  ON_COMMAND(ID_FILE_PRINT_SETUP,   &onSetupPrinter)
END_MESSAGE_MAP()


void CScrView::OnInitialUpdate() {

  CScrollView::OnInitialUpdate();

  display.load();  dMgr.setFontScale(display.scale);
  printer.load(0); pMgr.setFontScale(printer.scale);

  initNoteOrietn();   initRptOrietn();
  }


void CScrView::initNoteOrietn() {
  dspNote.prtrOrietn = prtNote.prtrOrietn =
                                  (PrtrOrient) iniFile.readInt(RptOrietnSect, NoteOrietnKey, PortOrient);}


void CScrView::saveNoteOrietn() {iniFile.write(RptOrietnSect, NoteOrietnKey, prtNote.prtrOrietn);}



void CScrView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
                                    {dMgr.setScrollSize(); CScrollView::OnUpdate(pSender, lHint, pHint);}


void CScrView::OnPrepareDC(CDC* dc, CPrintInfo* info) {

  if (dc->IsPrinting())                    pMgr.onPrepareDC(dc, info);
  else {CScrollView::OnPrepareDC(dc, 0);   dMgr.onPrepareDC(dc);}
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


BOOL CScrView::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll) {              // Override
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


// Printing Logic
// CScrView printing
/* The following functions are called for printing a page in the order given with one exception:
BOOL OnPreparePrinting(        CPrintInfo* pInfo);  -- 1st
void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);  -- 2nd
     CDC::StartDoc()                                -- 3rd
void OnPrepareDC(    CDC* pDC, CPrintInfo* pInfo);  -- 4th                         <-// CScrView => Mgr
     CDC::StartPage()                               -- 5th                          ^
void OnPrint(        CDC* pDC, CPrintInfo* pInfo);  -- 6th                          ^
     CDC::EndPage()                                 -- 7th then loops for each page ^
     CDC::EndDoc()                                  -- after last page
void OnEndPrinting(  CDC* pDC, CPrintInfo* pInfo);  -- last
*/

BOOL CScrView::OnPreparePrinting(CPrintInfo* pInfo) {
bool          rslt;
CPrintDialog* dlg;
String        name;

  onPreparePrinting(pInfo);

  if (pInfo->m_bPreview) {pInfo->m_nNumPreviewPages = 1;}

  theCApp->initPrinterAttr();

    rslt = CView::DoPreparePrinting(pInfo);                       // Print dialog box Function

    dlg = pInfo->m_pPD;     if (!dlg) return rslt;

    name = dlg->GetDeviceName();   if (name != printer.name) printer.load(name);

  theCApp->savePrinterAttr();   return rslt;
  }


void CScrView::onSetupPrinter() {
String curName;
String newName;

  theCApp->initPrinterAttr();   curName = theCApp->getPrinterName();

    theCApp->setupPrinterDlg();

  newName = theCApp->getPrinterName();

  if (curName != newName) printer.load(newName);

  theCApp->savePrinterAttr();   printer.store();
  }


DevBase& CScrView::getPrtDevDC(CDC*& dc) {return pMgr.getDevDC(dc);}


DevBase& CScrView::getDplDevDC(CDC*& dc) {return dMgr.getDevDC(dc);}


