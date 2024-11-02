// SFTPView.cpp : implementation of the SFTPView class


#include "pch.h"
#include "SFTPView.h"
#include "SFTP.h"
#include "SFTPDoc.h"
#include "ClipLine.h"
#include "IniFile.h"
#include "OptionsDlg.h"
#include "Resource.h"
#include "Resources.h"
#include "RptOrientDlgTwo.h"
#include "SendMsg.h"


static TCchar* StrOrietnKey = _T("Store");


IMPLEMENT_DYNCREATE(SFTPView, CScrView)

BEGIN_MESSAGE_MAP(SFTPView, CScrView)

  ON_COMMAND(ID_Options,     &onOptions)
  ON_COMMAND(ID_Orientation, &onRptOrietn)

  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONDBLCLK()

END_MESSAGE_MAP()


SFTPView::SFTPView() noexcept : scrollWdw(false) {
ResourceData res;
String       pn;
  if (res.getProductName(pn)) prtNote.setTitle(pn);
  }


BOOL SFTPView::PreCreateWindow(CREATESTRUCT& cs) {return CScrView::PreCreateWindow(cs);}


void SFTPView::OnInitialUpdate() {CScrView::OnInitialUpdate();}


void SFTPView::onOptions() {
OptionsDlg dlg;

  if (printer.name.isEmpty()) printer.load(0);

  if (dlg.DoModal() == IDOK) pMgr.setFontScale(printer.scale);
  }


void SFTPView::onRptOrietn() {
RptOrietnDlg dlg;

  dlg.lbl01 = _T("Store:");
  dlg.ntpd = printer.toStg(prtNote.prtrOrietn);

  if (dlg.DoModal() == IDOK) {
    prtNote.prtrOrietn  = printer.toOrient(dlg.ntpd);

    saveRptOrietn();
    }
  }


void SFTPView::initRptOrietn() { }


void SFTPView::saveRptOrietn() {
  saveNoteOrietn();

//  iniFile.write(RptOrietnSect, StrOrietnKey,  (int) prtStore.prtrOrietn);
  }



void SFTPView::onPreparePrinting(CPrintInfo* info) {

  switch(doc()->dataSrc()) {
    case NotePadSrc : prtNote.onPreparePrinting(info);     break;
    }
  }


// Perpare output (i.e. report) then start the output with the call to SCrView

void SFTPView::onBeginPrinting() {

  switch(doc()->dataSrc()) {
    case NotePadSrc : prtNote.onBeginPrinting(*this);  break;
    }
  }


void SFTPView::onDisplayOutput() {

  switch(doc()->dataSrc()) {
    case NotePadSrc : dspNote.display(*this); break;
    }

  }


void SFTPView::displayHeader(DevBase& dev) {
  switch(doc()->dataSrc()) {
    case NotePadSrc   : dspNote.dspHeader(dev);   break;
    }
  }


void SFTPView::displayFooter(DevBase& dev) {
  switch(doc()->dataSrc()) {
    case NotePadSrc   : dspNote.dspFooter(dev);   break;
    }

  if (scrollWdw) sendMsg(WM_VSCROLL, SB_BOTTOM, 0);
  }


void SFTPView::printHeader(DevBase& dev, int pageNo) {
  switch(doc()->dataSrc()) {
    case NotePadSrc: prtNote.prtHeader(dev, pageNo);   break;
    }
  }


// The footer is injected into the printed output, so the output goes directly to the device.
// The output streaming functions are very similar to NotePad's streaming functions so it should not
// be a great hardship to construct a footer.

void SFTPView::printFooter(DevBase& dev, int pageNo) {
  switch(doc()->dataSrc()) {
    case NotePadSrc : prtNote.prtFooter(dev, pageNo);  break;
    }
  }



void SFTPView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) {

  switch(doc()->dataSrc()) {
    case NotePadSrc : break;
    case StoreSrc   : break;
    }
  }


void SFTPView::OnSetFocus(CWnd* pOldWnd) {

  CScrView::OnSetFocus(pOldWnd);

  switch(doc()->dataSrc()) {
    case NotePadSrc : break;
    case StoreSrc   : break;
    }
  }


void SFTPView::OnLButtonDown(UINT nFlags, CPoint point)
                        {clipLine.set(point);   invalidate();   CScrView::OnLButtonDown(nFlags, point);}


void SFTPView::OnLButtonDblClk(UINT nFlags, CPoint point)
  {clipLine.set(point);   RedrawWindow();   clipLine.load();   CScrView::OnLButtonDblClk(nFlags, point);}


void SFTPView::OnContextMenu(CWnd* /*pWnd*/, CPoint point) {
CRect  rect;
CMenu* popup;
CWnd*  pWndPopupOwner = this;

  if (point.x == -1 && point.y == -1)
            {GetClientRect(rect);  ClientToScreen(rect);  point = rect.TopLeft();  point.Offset(5, 5);}

  while (pWndPopupOwner->GetStyle() & WS_CHILD) pWndPopupOwner = pWndPopupOwner->GetParent();

  popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
  }


void SFTPView::onCopy() {clipLine.load();  invalidate();}


void SFTPView::onPup1() {  }


void SFTPView::onPup2() {  }


// SFTPView diagnostics

#ifdef _DEBUG

void SFTPView::AssertValid() const {CScrollView::AssertValid();}

void SFTPView::Dump(CDumpContext& dc) const {CScrollView::Dump(dc);}     // non-debug version is inline
                                             // non-debug version is inline
SFTPDoc* SFTPView::GetDocument() const
  {ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(SFTPDoc))); return (SFTPDoc*)m_pDocument;}

#endif //_DEBUG



