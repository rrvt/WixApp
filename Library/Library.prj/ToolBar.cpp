// Tool Bar to be used with Button and ComboBox


#include "stdafx.h"
#include "ToolBar.h"
#include "TBButton.h"
#include "TBComboBox.h"
#include "TBEditBox.h"
#include "TBMenuButton.h"
#ifdef DsplyHistory
#include "History.h"                      // Debug Only
#endif


void ToolBar::initialize(CRect& winRect) {
  winHeight = winRect.bottom - winRect.top;   winWidth = winRect.right - winRect.left;
  }

static const DWORD TBStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY;


bool ToolBar::create(CWnd* wnd, uint id, DWORD style) {

  if (!CreateEx(wnd, TBSTYLE_FLAT, TBStyle | style)) return false;

  if (!LoadToolBar(id, 0, 0, TRUE)) return false;

  wnd->RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, AFX_IDW_TOOLBAR);   return true;
  }


bool ToolBar::installBtn(uint id, TCchar* caption) {
TBButton  btn(id);
TBBtnCtx& ctx = addCtx(id);   ctx.initialize(winWidth, winHeight);   getFontDim(ctx);

  btn.install(caption);   return ReplaceButton(id, btn) > 0;
  }


bool ToolBar::installComboBox(uint id) {
TBComboBox btn(id);
TBBtnCtx&  ctx = addCtx(id);   ctx.initialize(winWidth, winHeight);   getFontDim(ctx);

  btn.install(ctx);

  return ReplaceButton(id, btn) > 0;
  }


bool ToolBar::installEditBox(uint id, int noChars) {
TBEditBox btn(id);
TBBtnCtx& ctx = addCtx(id);   ctx.initialize(winWidth, winHeight);   getFontDim(ctx);

  ctx.maxChars = noChars;

  btn.install(ctx);   return ReplaceButton(id, btn) > 0;
  }


bool ToolBar::installPopupMenu(uint id) {
TBComboBox btn(id);
TBBtnCtx&   ctx = addCtx(id);   ctx.initialize(winWidth, winHeight);   getFontDim(ctx);

  btn.install(ctx);

  return ReplaceButton(id, btn) > 0;
  }


bool ToolBar::addPopupItems(uint id, const CbxItem* items, int noItems, bool sorted) {
TBBtnCtx& ctx = *findCtx(id);   if (!&ctx) return false;

  if (TBComboBox::addItems(id, items, noItems, ctx, sorted)) adjust(ctx);

  return true;
  }


bool ToolBar::addPopupItem(uint id, CbxItem& item, bool sorted) {
TBBtnCtx& ctx = *findCtx(id);   if (!&ctx) return false;

  if (TBComboBox::addItem(id, item, ctx, sorted)) adjust(ctx);

  return true;
  }


// Add a Resource Menu to popup

void ToolBar::addPopupMenu(uint id, uint idr, bool sorted) {
TBBtnCtx& ctx = *findCtx(id);   if (!&ctx) return;

  if (TBComboBox::addRes(id, idr, ctx, sorted)) adjust(ctx);
  }


void ToolBar::setPopupCaption(uint id, TCchar* caption) {
TBBtnCtx& ctx = *findCtx(id);   if (!&ctx) return;

  TBComboBox::setCaption(id, caption, ctx);   adjust(ctx);
  }



void ToolBar::dispatch(uint id, TCchar* caption)
      {uint cmdID = TBComboBox::getCmdId(id, caption);   if (cmdID) PostMessage(WM_COMMAND, cmdID, 0);}


bool ToolBar::installMenu(uint id, uint idr, TCchar* caption) {
TBMenuButton btn(id);

  btn.install(idr, caption);   return ReplaceButton(id, btn) > 0;
  }


void ToolBar::addCbxItems(uint id, CbxItem* items, int nItems, bool sorted) {
TBBtnCtx& ctx = *findCtx(id);   if (!&ctx) return;

  if (TBComboBox::addItems(id, items, nItems, ctx, sorted)) adjust(ctx);
  }


// Add a single item to combo box

void ToolBar::addCbxItem (uint id, CbxItem& item, bool sorted) {
TBBtnCtx& ctx = *findCtx(id);   if (!&ctx) return;

  if (TBComboBox::addItem(id, item, ctx, sorted)) adjust(ctx);
  }


// Add Caption to combo box, should be last...

void ToolBar::setCbxCaption(uint id, TCchar* caption) {
TBBtnCtx& ctx = *findCtx(id);   if (!&ctx) return;

  if (TBComboBox::setCaption(id, caption, ctx)) adjust(ctx);

  TBComboBox::setCaption(id, caption, ctx);
  }


void ToolBar::addResToCbx(uint id, uint idr, bool sorted) {
TBBtnCtx& ctx = *findCtx(id);   if (!&ctx) return;

  if (TBComboBox::addRes(id, idr, ctx, sorted)) adjust(ctx);
  }


bool ToolBar::setEbxText(uint id, TCchar* txt) {
TBEditBox* ebx = TBEditBox::get(id);   if (!ebx) return false;

  ebx->setText(txt);   return true;
  }


bool ToolBar::getEbxText(uint id, String& txt) {
TBEditBox* ebx = TBEditBox::get(id);   if (!ebx) return false;

  ebx->getText(txt);   return true;
  }


TBMenuButton* ToolBar::getMenuBtn(uint id) {
int  n = GetCount();
int  i;

  for (i = 0; i < n; i++) if (GetItemID(i) == id) return (TBMenuButton*) GetButton(i);

  return 0;
  }


// Adjust the tool bar after changing dim of a button

void ToolBar::adjust(TBBtnCtx& ctx) {if (ctx.dirty) {AdjustLayout();   ctx.dirty = false;}}



void ToolBar::getFontDim(TBBtnCtx& ctx) {     //
CDC*       dc;
CWnd*      wnd;
TEXTMETRIC metric;

  if (!avgWidth) {

    for (dc = GetDC(), wnd = 0; !dc; dc = wnd->GetDC()) {
      wnd = wnd ? wnd->GetParent() : GetParent();   if (!wnd) break;
      }

    if (!dc) {avgWidth = 12; height = 21; return;}

    dc->GetTextMetrics(&metric);

    avgWidth = (3 * metric.tmAveCharWidth + metric.tmMaxCharWidth) / 4;

    height = metric.tmHeight * 1333 / 1000;
    }

  ctx.avgWidth = avgWidth;   ctx.height    = height;
  ctx.winWidth = winWidth;   ctx.winHeight = winHeight;
  }


TBBtnCtx& ToolBar::addCtx(uint id) {
TBBtnCtx* ctx = findCtx(id);

  if (ctx) return *ctx;

  ctx = data.allocate();   data = ctx;   ctx->id = id;  return *ctx;
  }


TBBtnCtx* ToolBar::findCtx(uint id) {
TlBrIter  iter(*this);
TBBtnCtx* ctx;

  for (ctx = iter(); ctx; ctx = iter++) if (ctx-> id == id) return ctx;

  return 0;
  }


// ToolBar Button Flying Tips are implemented hereit for applications without a Framework (i.e. MainFrame)
// Add the following to message map
//   ON_NOTIFY_EX( TTN_NEEDTEXT, 0,   &OnTtnNeedText)
// and add the following afx_msg to the application somewhere (e.g. a dialog box)
// BOOL MyApp::OnTtnNeedText(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
//                                                                {return toolBar.OnTtnNeedText(pNMHDR);}

bool ToolBar::OnTtnNeedText(NMHDR* pNMHDR) {
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


bool ToolBar::getMouseHover(ButtonBase*& btn) {
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

