// Print Manager -- needed to isolate variables from Display Management!


#pragma once
#include "Printer.h"
#include "NtPdToDev.h"
#include "ManageDevBase.h"

class CScrView;


class ManagePrinter : public ManageDevBase {

NotePad     npd;
NtPdToDev   ntPdToDev;

CDC*        dc;                       // Capture during initialization for convenience
CPrintInfo* info;

bool        endPrinting;              // Flag to end printing

String      leftFooter;               // Data to print at right side of footer
Date        date;                     // Date to print at left edge of footer

int         pageNo;
Device      pageBackup;

public:

  ManagePrinter(CScrView& view);
 ~ManagePrinter() { }

  void clear();

/* Doc/View Framework Calls to implement printing
     CMyView::OnPreparePrinting    o Set length of doc if known
             |                     o Call DoPreparePrining to display Print dialog box which
             V                     o creates DC
     CMyView::OnBeginPrinting      o Set length of document based on DC
             |                     o Allocate DGI resources
             V
         CDC::StartDoc             o Performed by CView
             |
             V
  |->CMyView::OnPrepareDC          o Change DC attributes
  |          |                     o Check for end of document
  |          V
  |      CDC::StartPage            o Performed by CView before calling OnPrint
  |          |
  |          V
  |  CMyView::OnPrint              o Print specified page, including Headers and Footers
  |          |
  |          V
  -------CDC::EndPage              o Performed by CView after OnPrint returns
             |
             V
         CDC::EndDoc               o Performed by CView after info->m_bContinuePrinting is set to
             |                       false
             V
     CMyView::OnEndPrinting        o Deallocate GDI resources
*/
  void       onBeginPrinting(CDC* cdc, CPrintInfo* pInfo);
  void       onPrepareDC(    CDC* cdc, CPrintInfo* pInfo);
  void       onPrint(        CDC* cdC, CPrintInfo* pInfo);

  // Utilities
  void       startDev() {if (!endPrinting) ntPdToDev.startDev();}

  int        getNoPages();

private:

  void       prepareDC();
  void       setMargins()
                      {ntPdToDev.setVertMgns(printer.topMargin, printer.botMargin);   setHorzMgns();}
  void       setHorzMgns();
  void       findNextPreviewPage(CDC* dc, CPrintInfo* info);
  void       onePageOut();
  bool       isFinishedPrinting(CPrintInfo* info);

  ManagePrinter() : ManageDevBase(*(CScrView*)0, *(NotePad*)0, *(NtPdToDev*)0),
                                                               ntPdToDev(*(NotePad*)0, PrinterID) { }
  };


