// Scroll View


#pragma once
#include "DsplyMgr.h"
#include "PrintMgr.h"


extern const int     BigNmbr;
extern const TCchar* FontSection;
extern const TCchar* DspScaleKey;
extern const TCchar* PrtScaleKey;


class CScrView : public CScrollView {

static int lastPos;

protected:

DsplyMgr   dMgr;
PrintMgr   pMgr;

public:

  CScrView() : dMgr(*this), pMgr(*this) { }
 ~CScrView() { }

  virtual void     OnInitialUpdate();

          void     setFont(TCchar* f, double points = 12.0);
          void     setMgns(double left, double top, double right, double bot, CDC* dc);
          double   getFontScale(bool printing);
          void     setFontScale(bool printing, double scl);

  virtual void     OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);    // Override

  virtual void     OnPrepareDC(CDC* dc, CPrintInfo* info = NULL);             // Override

  virtual void     onPrepareOutput(bool printing = false);

  virtual void     OnDraw(CDC* pDC) {dMgr.OnDraw(pDC);}
                                                                            // Override to draw this view
          void     invalidate() {Invalidate();}

          void       setOrientation(PrtrOrient orientation) {pMgr.setOrientation(orientation);}
          bool       setPrntrOrient(HANDLE h, CDC* dc = 0) {return pMgr.setOrient(h, dc);}
          PrtrOrient getPrntrOrient(HANDLE h)              {return pMgr.getOrient(h);}
          int        noLinesPrPg() {return pMgr.noLinesPrPg();}   // Determine no lines per printed page

          void     suppressOutput(bool printing);
          void     negateSuppress(bool printing);
          void     disableWrap(bool printing);
          void     enableWrap(bool printing);
          Device&  getDev(bool printing);

  // Printer Overrides

  virtual void     trialRun(int& maxLines, int& noPages) {pMgr.trialRun(maxLines, noPages);}
  virtual void     printFooter(Device& dev, int pageNo) {pMgr.printFooter(dev, pageNo);}

protected:

  virtual void     OnPrint(CDC* dc, CPrintInfo* info) {pMgr.OnPrint(dc, info);}       // Override

  virtual BOOL     OnPreparePrinting(CPrintInfo* info) {return pMgr.OnPreparePrinting(info);} // Override
          bool     finPreparePrinting(bool rslt)       {return pMgr.finPreparePrinting(rslt);}

  virtual void     OnEndPrinting(CDC* dc, CPrintInfo* info)                           // Override
                                                  {CScrollView::OnEndPrinting(dc, info); pMgr.endPrint();}

private:

  virtual BOOL     OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);    // Override

protected:

  DECLARE_MESSAGE_MAP()
  };





