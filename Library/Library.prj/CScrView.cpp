// Scroll View

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


#include "stdafx.h"
#include "CScrView.h"
#include "IniFile.h"


int CScrView::lastPos = 0;
const int BigNmbr = 9999;

const TCchar* FontSection = _T("FontSection");
const TCchar* DspScaleKey = _T("DspScale");
const TCchar* PrtScaleKey = _T("PrtScale");



BEGIN_MESSAGE_MAP(CScrView, CScrollView)
  ON_COMMAND(ID_FILE_PRINT,         &CScrollView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()





void CScrView::OnInitialUpdate() {
String dspScale;
double scl;
uint   x;

  CScrollView::OnInitialUpdate();

  if (iniFile.readString(FontSection, DspScaleKey, dspScale, _T("6060.0"))) {
    scl = dspScale.stod(x);  setFontScale(false, scl);
    }
  if (iniFile.readString(FontSection, PrtScaleKey, dspScale, _T("7175.0"))) {
    scl = dspScale.stod(x);  setFontScale(true, scl);
    }
  }


void CScrView::setFont(TCchar* f, double points) {dMgr.setFont(f, points); pMgr.setFont(f, points);}


double CScrView::getFontScale(bool printing)
                                          {return printing ? pMgr.getFontScale() : dMgr.getFontScale();}


void CScrView::setFontScale(bool printing, double scl) {
String s;

  if (printing) {
    pMgr.setFontScale(scl);

    s = scl;   iniFile.writeString(FontSection, PrtScaleKey, s);   return;
    }

  dMgr.setFontScale(scl);

  s = scl;   iniFile.writeString(FontSection, DspScaleKey, s);
  }



void CScrView::setMgns(double left, double top, double right, double bot, CDC* dc)
                                            {if (dc->IsPrinting()) pMgr.setMgns(left, top,  right, bot);}


void CScrView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
                                    {dMgr.setScrollSize(); CScrollView::OnUpdate(pSender, lHint, pHint);}


void CScrView::OnPrepareDC(CDC* dc, CPrintInfo* info) {                       // Override

  CScrollView::OnPrepareDC(dc, info);

  if (dc->IsPrinting() && info) pMgr.OnPrepareDC(dc, info);
  else                          dMgr.OnPrepareDC(dc);
  }


void CScrView::onPrepareOutput(bool printing) {

  if      (printing)  pMgr.startDev();
  else                dMgr.startDev();
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


void CScrView::suppressOutput(bool printing)
                                      {if (printing) pMgr.suppressOutput(); else dMgr.suppressOutput();}


void CScrView::negateSuppress(bool printing)
                                      {if (printing) pMgr.negateSuppress(); else dMgr.negateSuppress();}


void CScrView::disableWrap(bool printing)
                                      {if (printing) pMgr.disableWrap();    else dMgr.disableWrap();}


void CScrView::enableWrap(bool printing)
                                      {if (printing) pMgr.enableWrap();     else dMgr.enableWrap();}


Device& CScrView::getDev(bool printing)
  {return printing ? pMgr.getDev() : dMgr.getDev();}
