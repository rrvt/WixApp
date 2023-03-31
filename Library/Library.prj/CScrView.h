// Scroll View


#pragma once
#include "DsplyMgr.h"
#include "NotePadRpt.h"
#include "PrintMgr.h"


extern TCchar*   RptOrietnSect;
extern const int BigNmbr;


class CScrView : public CScrollView {

DECLARE_MESSAGE_MAP()

static int lastPos;

protected:

DsplyMgr   dMgr;
PrintMgr   pMgr;

NotePadRpt dspNote;
NotePadRpt prtNote;

public:

  CScrView() : dMgr(*this), pMgr(*this), dspNote(dMgr.getNotePad()), prtNote(pMgr.getNotePad()) { }
 ~CScrView() { }

  virtual void       OnInitialUpdate();

  virtual void       OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);    // Override

          void       initNoteOrietn();
          void       saveNoteOrietn();
  virtual void       initRptOrietn() { }
  virtual void       saveRptOrietn() { }
    PrtrOrient       getOrientation() {return prtNote.prtrOrietn;}

  virtual void       onDisplayOutput() { }
  virtual void       OnDraw(CDC* pDC) {dMgr.onDraw(pDC);}

  virtual void       displayHeader(DevBase& dev) { }
  virtual void       displayFooter(DevBase& dev) { }

          void       enableDplWrap()  {dMgr.wrapEnabled = true;}
          void       disableDplWrap() {dMgr.wrapEnabled = false;}

          void       invalidate() {Invalidate();}

          void       setFont(TCchar* f, double points) {dMgr.setFont(f, points); pMgr.setFont(f, points);}

          double     getFontScale(bool printing)
                                            {return printing ? pMgr.getFontScale() : dMgr.getFontScale();}
          void       setFontScale(bool printing, double scl)
                                    {if (printing) pMgr.setFontScale(scl);   else dMgr.setFontScale(scl);}

          DevBase&   getDplDevDC(CDC*& dc);
          DevBase&   getPrtDevDC(CDC*& dc);

protected:

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

  virtual BOOL OnPreparePrinting(CPrintInfo* info);
  virtual void OnBeginPrinting(CDC* dc, CPrintInfo* info) {pMgr.onBeginPrinting(dc, info);}
  virtual void OnPrepareDC(    CDC* dc, CPrintInfo* info = 0);                // Display/Printer Override
  virtual void OnPrint(        CDC* dc, CPrintInfo* info) {pMgr.onPrint(dc, info);}
  virtual void OnEndPrinting(  CDC* dc, CPrintInfo* info) { }   //

public:
          void disablePrtWrap()     {pMgr.wrapEnabled = false;}
          void enablePrtWrap()      {pMgr.wrapEnabled = true;}

  virtual void onPreparePrinting(CPrintInfo* info) { }
  virtual void onBeginPrinting() { }               // View link to Begin Printing

  // Printer Overrides

  virtual int  getNoPages() {return pMgr.getNoPages();}
  virtual void printHeader(DevBase& dev, int pageNo) { }
  virtual void printFooter(DevBase& dev, int pageNo) { }

protected:
          void startDev(bool printing) {if (printing) pMgr.startDev(); else dMgr.startDev();}
private:

  virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);    // Override

  afx_msg void onSetupPrinter();
  };






