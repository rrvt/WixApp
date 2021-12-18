// ToolBar Simulated Drop Down Menu -- Useful in dialog boxes


#include "stdafx.h"
#include "TBPopupMenu.h"
#include "TBBtnCtx.h"


TBPopupMenu::TBPopupMenu(uint id) : CMFCToolBarComboBoxButton(id, -1) { }


void TBPopupMenu::install(TBBtnCtx& ctx) {

  m_dwStyle = CBS_DROPDOWN | WS_VSCROLL;   m_iWidth = ctx.getHoriz() + 20;

  m_nDropDownHeight = ctx.getVert();
  }


void TBPopupMenu::setCaption(uint id, TCchar* txt, TBBtnCtx& ctx) {
TBPopupMenu* popup = TBPopupMenu::get(id);

  if (popup) popup->setCaption(txt);
  }


uint TBPopupMenu::getCmdId(uint id, TCchar* caption) {
TBPopupMenu* popup = TBPopupMenu::get(id);   if (!popup) return 0;
uint         cmdID = popup->getCmdId();      if (!cmdID) return 0;

  popup->setCaption(caption);   return cmdID;
  }


void TBPopupMenu::setCaption(TCchar* txt)
                  {CComboBox* cbx = GetComboBox();    if (cbx) {cbx->Clear();  cbx->SetWindowText(txt);}}


uint TBPopupMenu::getCmdId() {
int i = GetCurSel();

  return i >= 0 ? GetItemData(i) : 0;
  }



TBPopupMenu* TBPopupMenu::get(uint id)
                                    {try {return (TBPopupMenu*) GetByCmd(id);}   catch (...) {return 0;}}


bool TBPopupMenu::addMenu(   uint id, uint idr, TCchar* caption, TBBtnCtx& ctx, bool sorted) {
TBPopupMenu* popup = TBPopupMenu::get(id);   if (!popup) return false;

  return popup->addMenu(idr, caption, ctx, sorted);
  }


bool TBPopupMenu::addMenu(uint idr, TCchar* caption, TBBtnCtx& ctx, bool sorted) {
CMenu   menu;   if (!menu.LoadMenu(idr)) return false;
uint    n = menu.GetMenuItemCount();
uint    i;
uint    id;
CString name;

  ctx.setMaxChars(caption);    ctx.incNoLines();

  for (i = 0; i < n; i++) {
    id = menu.GetMenuItemID(i);   menu.GetMenuString(i, name, MF_BYPOSITION);

    ctx.setMaxChars(name);   ctx.incNoLines();

    if (sorted) AddSortedItem(name, id);   else AddItem(name, id);
    }

  setCaption(caption);   setDim(ctx);

  menu.DestroyMenu();  return true;
  }



void TBPopupMenu::setDim(TBBtnCtx& ctx) {
  m_iWidth = ctx.getHoriz() + 20;   m_nDropDownHeight = ctx.getVert();
  }





#if 0
bool TBPopupMenu::setSize(uint idr, int maxChars, int avgWidth, int heightLine) {
CMenu   menu;   if (!menu.LoadMenu(idr)) return false;
uint    n = menu.GetMenuItemCount();
uint    i;
CString name;
int     lng;

  for (i = 0; i < n; i++) {
    menu.GetMenuString(i, name, MF_BYPOSITION);

    lng = name.GetLength(); if (lng > maxChars) maxChars = lng;
    }

  m_iWidth = maxChars * avgWidth + 20;

  SetDropDownHeight((n+1) * heightLine);

  menu.DestroyMenu();  return true;
  }
#endif

