// Tool Bar to be used with Button and ComboBox


#include "stdafx.h"
#include "ToolBar.h"
#include "TBButton.h"
#include "TBComboBox.h"
#include "TBEditBox.h"
#include "TBMenuButton.h"
#include "TBPopupMenu.h"


void ToolBar::initialize(CRect& winRect) {
  winHeight = winRect.bottom - winRect.top;   winWidth = winRect.right - winRect.left;
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


bool ToolBar::installPopupMenu(uint id, uint idr) {
TBPopupMenu btn(id);
TBBtnCtx&   ctx = addCtx(id);   ctx.initialize(winWidth, winHeight);   getFontDim(ctx);

  btn.install(ctx);

  return ReplaceButton(id, btn) > 0;
  }


// Add a Resource Menu to popup

void ToolBar::addPopupMenu(uint id, uint idr, TCchar* caption, bool sorted) {
TBBtnCtx& ctx = *findCtx(id);   if (!&ctx) return;

  if (TBPopupMenu::addMenu(id, idr, caption, ctx, sorted)) adjust(ctx);
  }



void ToolBar::dispatch(uint id, TCchar* caption)
      {uint cmdID = TBPopupMenu::getCmdId(id, caption);   if (cmdID) PostMessage(WM_COMMAND, cmdID, 0);}


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

  if (TBComboBox::add(id, item, ctx, sorted)) adjust(ctx);
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



#if 0
void ToolBar::setCbxDim(uint id, TBBtnCtx& ctx) {
TBComboBox* cbx = TBComboBox::get(id);

  getFontDim(ctx);

  if (cbx) {ctx.height++;   cbx->setDim(ctx);    ctx.height--;}
  }
#endif



