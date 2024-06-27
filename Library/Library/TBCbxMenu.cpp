// Drop Down Menu constructed in a Combo Box

\
#include "pch.h"
#include "TBCbxMenu.h"
#include "CbxItem.h"
#include "ToolBarDim.h"


TBCbxMenu& TBCbxMenu::install(int idr, TCchar* caption) {
CMenu      menu;
uint       i;
uint       n;
Cstring    txt;

  if (!menu.LoadMenu(idr)) return *(TBCbxMenu*)0;

    for (i = 0, maxChars = 0, n = menu.GetMenuItemCount(); i < n; i++)
                {menu.GetMenuString(i, txt, MF_BYPOSITION);   addItem(txt, menu.GetMenuItemID(i));}

  menu.DestroyMenu();   return finInstall(caption);
  }


TBCbxMenu& TBCbxMenu::install(const CbxItem cbxItem[], int n, TCchar* caption) {
int     i;

  for (i = 0, maxChars = 0; i < n; i++)
                                {const CbxItem& item = cbxItem[i];   addItem(item.txt, item.data);}

  return finInstall(caption);
  }


void TBCbxMenu::addItem(TCchar* txt, int data) {

  setMaxChars(txt);

  if (FindItem(txt) < 0) AddItem(txt, data);
  }


TBCbxMenu& TBCbxMenu::finInstall(TCchar* caption) {

  this->caption = caption;   setMaxChars(caption);

  m_iWidth = toolBarDim.getHoriz(maxChars) + 20;

  m_dwStyle = CBS_DROPDOWNLIST | WS_VSCROLL | BS_VCENTER;   SetFlatMode(true);

  return *this;
  }


bool TBCbxMenu::setCaption() {

  if (!getActual()) return false;

  actual->SetText(caption);   return true;
  }


uint TBCbxMenu::getCmdId() {
CMFCToolBarComboBoxButton* cbxBtn = GetByCmd(id);   if (!cbxBtn) return 0;
int                        i      = cbxBtn->GetCurSel();

  return i >= 0 ? cbxBtn->GetItemData(i) : 0;
  }




bool TBCbxMenu::getActual() {if (!actual) actual = GetByCmd(id);   return actual != 0;}



