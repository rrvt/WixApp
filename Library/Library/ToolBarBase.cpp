// Tool Bar to be used with Button and ComboBox


#include "pch.h"
#include "ToolBarBase.h"
#include "TBButton.h"
#include "TBCbxMenu.h"                    // Use for Menus on Dialog Box Tool Bars
#include "TBMenu.h"                       // Use for Menus in Doc/View applications
#include "WinPos.h"
#ifdef DsplyHistory
#include "History.h"                      // Debug Only
#endif


static const DWORD TBStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY |
AFX_DEFAULT_TOOLBAR_STYLE;


bool ToolBarBase::create(CWnd* wnd, uint id, DWORD style) {
CRect rect;

  if (!CreateEx(wnd, TBSTYLE_FLAT, TBStyle | style)) return false;

  if (!LoadToolBar(id, 0, 0, TRUE)) return false;

  wnd->RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, AFX_IDW_TOOLBAR);

  window = wnd;   return true;
  }


void ToolBarBase::move(CRect& winRect) {
CRect rect;
int   hDelta = winPos.dluToScreen(7);
int   vDelta = winPos.dluToScreen(10, false);

  GetWindowRect(&rect);

  rect.bottom  = rect.top  + vDelta;
  rect.left   += hDelta;
  rect.right   = rect.left + winRect.right - winRect.left - 2*hDelta;

  ScreenToClient(rect);   MoveWindow(&rect);
  }


void ToolBarBase::OnFillBackground(CDC* pDC)
                      {CRect rect;   GetClientRect(&rect);   pDC->FillSolidRect(&rect, RGB(255,255,255));}


void ToolBarBase::getFontDim(FontDim& fontDim) {
CDC*       dc;
CWnd*      wnd;
TEXTMETRIC metric;

  for (dc = GetDC(), wnd = 0; !dc; dc = wnd->GetDC()) {
    wnd = wnd ? wnd->GetParent() : GetParent();   if (!wnd) break;
    }

  if (!dc) {toolBarDim.fontDim.width = 12; toolBarDim.fontDim.height = 21; return;}

  dc->GetTextMetrics(&metric);

  fontDim.width = (3 * metric.tmAveCharWidth + metric.tmMaxCharWidth) / 4;

  fontDim.height = metric.tmHeight * 1333 / 1000;
  }


bool ToolBarBase::add(TBButton& button, uint id, TCchar* caption) {
  button.install(caption);   return ReplaceButton(id, button) > 0;
  }


bool ToolBarBase::add(TBEditBox& button, uint id, int noChars) {
  button.install(noChars);   return ReplaceButton(id, button) > 0;
  }


bool ToolBarBase::add(TBMenu&  button, uint id, int idr, TCchar* caption) {
  bool rslt = ReplaceButton(id, *button.install(idr, caption)) > 0;   return rslt;
  }


bool ToolBarBase::add(TBMenu&  button, uint id, int idr, int index) {
  bool rslt = ReplaceButton(id, *button.install(idr, index)) > 0;   return rslt;
  }



bool ToolBarBase::add(TBMenu&  button, uint id, const CbxItem cbxItem[], int n, TCchar* caption) {
  bool rslt = ReplaceButton(id, *button.install(cbxItem, n, caption)) > 0;   return rslt;
  }


bool ToolBarBase::add(TBCbxMenu& button, uint id, int idr, TCchar* caption) {
bool rslt = ReplaceButton(id, button.install(idr, caption)) > 0;

  button.setCaption();   return rslt;
  }


bool ToolBarBase::add(TBCbxMenu& button, uint id, const CbxItem cbxItem[], int n, TCchar* caption) {
bool rslt = ReplaceButton(id, button.install(cbxItem, n, caption)) > 0;

  if (rslt) button.setCaption();   return rslt;
  }


bool ToolBarBase::add(TBCboBx& button, uint id, int noChars) {
  return ReplaceButton(id, *button.install(noChars)) > 0;
  }


bool ToolBarBase::add(TBCboBx& button, uint id, int idr, TCchar* caption) {
bool rslt = ReplaceButton(id, *button.install(idr, caption)) > 0;

  if (rslt) button.setCaption();   return rslt;
  }


bool ToolBarBase::add(TBCboBx& button, uint id, const CbxItem cbxItem[], int n, TCchar* caption) {
bool rslt = ReplaceButton(id, *button.install(cbxItem, n, caption)) > 0;

  if (rslt) {button.setCaption();   button.setHeight();}   return rslt;
  }




void ToolBarBase::dispatch(TBCbxMenu& cbxMenu) {
uint cmdID = cbxMenu.getCmdId();

  if (cmdID) PostMessage(WM_COMMAND, cmdID, 0);

  cbxMenu.setCaption();
  }




// Button Flying Tips are implemented here for applications without a Framework (i.e. MainFrame)
// Add the following to message map
//   ON_NOTIFY_EX( TTN_NEEDTEXT, 0,   &OnTtnNeedText)
// and add the following afx_msg to the application somewhere (e.g. a dialog box)
// BOOL MyApp::OnTtnNeedText(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
//                                                                {return toolBar.OnTtnNeedText(pNMHDR);}

bool ToolBarBase::OnTtnNeedText(NMHDR* pNMHDR) {
NMTTDISPINFO* pTTT = (NMTTDISPINFO*) pNMHDR;
ButtonBase*   btn;                           // Base class of all buttons
Cstring       s;                             // CString in which resource string with button ID is stored

#ifdef DsplyHistory
HistData&     h = history.nextData();   h.flags = pTTT->uFlags;
#endif

  if (pTTT->uFlags & TTF_CENTERTIP) return false;     // Do not process center tooltip requiests

  if ((pTTT->uFlags & 0x040) == 0) return false;      // Tooltips appear to have this bit define as one
                                                      // for mouse pointer tooltips
  if (!getMouseHover(btn)) return false;

  s.loadRes(btn->m_nID);   if (s.isEmpty()) return false;

#ifdef DsplyHistory
h.id = btn->m_nID;
#endif

  _stprintf_s(pTTT->szText, noElements(pTTT->szText), _T("%s"), s.str());   return true;
  }


bool ToolBarBase::getMouseHover(ButtonBase*& btn) {
CPoint point;                                         // point relative to toolbar client area
int    n;                                             // number of buttons
int    i;
CRect  rect;                                          // button rectangle relative to toolbar client area
                                                      // for mouse pointer tooltips
  GetCursorPos(&point);   ScreenToClient(&point);     // point relative to toolbar cliend area

  for (i = 0, n = GetCount(), btn = 0; i < n; i++) {
    GetItemRect(i, &rect);

    if (rect.left <= point.x && point.x < rect.right) {btn = GetButton(i);   return btn != 0;}
    }

  return false;
  }

