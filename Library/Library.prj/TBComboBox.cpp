// ToolBar Combo Box


#include "stdafx.h"
#include "TBComboBox.h"
#include "TBBtnCtx.h"


TBComboBox::TBComboBox(uint id) : CMFCToolBarComboBoxButton(id, -1) { }


void TBComboBox::install(TBBtnCtx& ctx) {

  SetFlatMode(false);   SetCenterVert(true);   m_dwStyle = CBS_DROPDOWN | WS_VSCROLL;

  setDim(ctx);
  }


bool TBComboBox::addItems(uint id, const CbxItem* items, int noItems, TBBtnCtx& ctx, bool sorted) {
TBComboBox* cbx = TBComboBox::get(id);    if (!cbx || !items) return false;

  cbx->addItems(items, noItems, ctx, sorted);   cbx->setDim(ctx);   return true;
  }


bool TBComboBox::addItems(const CbxItem* items, int noItems, TBBtnCtx& ctx, bool sorted) {
int i;

  ctx.incNoLines();

  for (i = 0; i < noItems; i++) {const CbxItem& item = items[i]; addItem(item, ctx, sorted);}

  setDim(ctx);   return noItems > 0;
  }


bool TBComboBox::addRes(uint id, uint idr, TBBtnCtx& ctx, bool sorted) {
TBComboBox* cbx = TBComboBox::get(id);    if (!cbx) return false;

  if (!cbx->addRes(idr, ctx, sorted)) return false;

  cbx->setDim(ctx);   return true;
  }


bool TBComboBox::addRes(uint idr, TBBtnCtx& ctx, bool sorted) {
CMenu   menu;   if (!menu.LoadMenu(idr)) return false;
uint    n = menu.GetMenuItemCount();
uint    i;
uint    id;
CString name;

  ctx.incNoLines();

  for (i = 0; i < n; i++) {
    id = menu.GetMenuItemID(i);   menu.GetMenuString(i, name, MF_BYPOSITION);

    CbxItem item(name, id);   addItem(item, ctx, sorted);
    }

  setDim(ctx);

  menu.DestroyMenu();  return true;
  }


bool TBComboBox::addItem(uint id, CbxItem& item, TBBtnCtx& ctx, bool sorted) {
TBComboBox* cbx = TBComboBox::get(id);    if (!cbx) return false;

  cbx->addItem(item, ctx, sorted);   cbx->setDim(ctx);  return true;
  }


//Add Unique item into combo box, sorted or as they arrive

void TBComboBox::addItem(const CbxItem& item, TBBtnCtx& ctx, bool sorted) {

  if (find(item.txt) < 0) {

    ctx.setMaxChars(item.txt);   ctx.incNoLines();

    if (sorted) AddSortedItem(item.txt, item.data);   else AddItem(item.txt, item.data);
    }
  }


bool TBComboBox::setCaption(uint id, TCchar* txt, TBBtnCtx& ctx) {
TBComboBox* cbx = TBComboBox::get(id);   if (!cbx) return false;

  ctx.setMaxChars(txt);

  cbx->setCaption(txt);   cbx->setDim(ctx);   return true;
  }



uint TBComboBox::getCmdId(uint id, TCchar* caption) {
TBComboBox* cbx   = TBComboBox::get(id);   if (!cbx)   return 0;
uint        cmdID = cbx->getCmdId();       if (!cmdID) return 0;

  cbx->setCaption(caption);   return cmdID;
  }


uint TBComboBox::getCmdId() {
int i = GetCurSel();

  return i >= 0 ? GetItemData(i) : 0;
  }


void TBComboBox::setCaption(TCchar* txt) {
CComboBox* cbx = GetComboBox();    if (!cbx) return;

  cbx->SetCurSel(-1);   cbx->Clear();   cbx->SetWindowText(txt);
  }


void TBComboBox::setDim(TBBtnCtx& ctx)
                                   {m_iWidth = ctx.getHoriz() + 20;   m_nDropDownHeight = ctx.getVert();}


CComboBox* TBComboBox::getCBx(uint id) {TBComboBox* tb = get(id);   return tb ? tb->GetComboBox() : 0;}


// Returns current selection when true

bool TBComboBox::getCurSel(uint id, String& s, int& data) {
TBComboBox* cbx = TBComboBox::get(id);    if (!cbx) return false;

  return cbx->getCurSel(s, data);
  }


TBComboBox* TBComboBox::get(uint id)
                              {try {return (TBComboBox*) GetByCmd((uint) id);}  catch (...) {return 0;}}


// Returns current selection of if none fail

bool TBComboBox::getCurSel(String& s, int& data) {
int i = GetCurSel();    if (i < 0) return false;

  s = GetItem(i);  data = GetItemData(i);  return true;
  }


int TBComboBox::find(String& s) {
int n = GetCount();
int i;

int x = FindItem(s);                                 // Test this

  for (i = 0; i < n; i++) if (s == GetItem(i)) return i;

  return -1;
  }

