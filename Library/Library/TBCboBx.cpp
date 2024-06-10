


#include "pch.h"
#include "TBCboBx.h"
#include "CbxItem.h"
#include "ToolBarDim.h"


TBCboBx* TBCboBx::install(int noChars) {maxChars = noChars;   return finInstall(_T(""));}



TBCboBx* TBCboBx::install(int idr, TCchar* caption) {
CMenu      menu;
uint       i;
uint       n;
Cstring    txt;
uint       data;
bool       rslt = true;

  if (!menu.LoadMenu(idr)) return 0;

    for (i = 0, maxChars = 0, n = menu.GetMenuItemCount(); i < n; i++) {
      menu.GetMenuString(i, txt, MF_BYPOSITION);   data = menu.GetMenuItemID(i);
      rslt = add(txt, data);   if (!rslt) break;
      }

  menu.DestroyMenu();   return rslt ? finInstall(caption) : 0;
  }


TBCboBx* TBCboBx::install(const CbxItem cbxItem[], int n, TCchar* caption) {
int     i;

  for (i = 0, maxChars = 0; i < n; i++) {const CbxItem& item = cbxItem[i];   add(item.txt, item.data);}

  return finInstall(caption);
  }


bool TBCboBx::setCaption() {

  if (!getActual()) return false;

#if 1
  actual->SetText(caption);   return true;
#else
  CEdit* edit;
  edit   = actual->GetEditCtrl();   if (!edit) return false;

  edit->ModifyStyle(0, ES_CENTER);   edit->SetWindowText(caption);   return true;
#endif
  }


bool TBCboBx::addItem(TCchar* txt, int val) {
String s = txt;

  if (!actual) {actual = GetByCmd(id);  if (!actual || !txt) return false;}

  if (actual->FindItem(txt) >= 0) return true;

  setMaxChars(s);

  return actual->AddItem(s, val) >= 0;
  }


bool TBCboBx::addItemSorted(TCchar* txt, int val) {

  if (!getActual() || !txt) return false;

  if (actual->FindItem(txt) >= 0) return true;

  setMaxChars(txt);   return actual->AddSortedItem(txt, val) >= 0;
  }


void TBCboBx::setWidth() {

  if (!getActual()) return;

  ((TBCboBx*)actual)->m_iWidth  = toolBarDim.getHoriz(maxChars) + 20;
  }


void TBCboBx::setHeight() {
int count;
int pixels;
int maxHeight = (toolBarDim.height/25 - 3) * 25;

  if (!getActual()) return;

  count = ((TBCboBx*)actual)->GetCount();   if (!count) return;

  pixels = count * 25;   pixels = pixels < 150 ? 150 : pixels > maxHeight ? maxHeight : pixels;

  ((TBCboBx*)actual)->SetDropDownHeight(pixels);
  }


bool TBCboBx::getCurSel(String& s, int& data) {
int i;

  if (!getActual()) return false;

  i = actual->GetCurSel();    if (i < 0) return false;

   s = actual->GetItem(i);  data = actual->GetItemData(i);  return true;

  }


bool TBCboBx::getActual() {if (!actual) actual = GetByCmd(id);   return actual != 0;}


bool TBCboBx::add(TCchar* txt, int data) {

  if (!txt) return false;

  if (FindItem(txt) >= 0) return true;

  setMaxChars(txt);

  return AddItem(txt, data) >= 0;
  }


TBCboBx* TBCboBx::finInstall(TCchar* caption) {

  this->caption = caption;   setMaxChars(caption);

  m_iWidth  = toolBarDim.getHoriz(maxChars) + 20;

  m_dwStyle =  CBS_DROPDOWNLIST | WS_VSCROLL | BS_VCENTER;   SetFlatMode(true);

  return this;
  }


