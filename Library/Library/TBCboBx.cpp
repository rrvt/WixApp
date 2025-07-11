


#include "pch.h"
#include "TBCboBx.h"
#include "CbxItem.h"
#include "ToolBarDim.h"

#include "MessageBox.h"


TBCboBx::TBCboBx(uint myId) :
              CMFCToolBarComboBoxButton(myId, -1), id(myId), maxChars(0), percent(1), actual(0) { }


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

  for (i = 0, maxChars = 0; i < n; i++)
                                    {const CbxItem& item = cbxItem[i];   add(item.txt, item.data);}
  return finInstall(caption);
  }


bool TBCboBx::setCaption() {

  if (!getActual()) return false;

  actual->SetText(caption);   setMaxChars(caption);   return true;
  }


bool TBCboBx::addItem(TCchar* txt, int val) {
String s = txt;

  if (!actual) {actual = GetByCmd(id);  if (!actual || !txt) return false;}

  if (actual->FindItem(txt) >= 0) return true;

  setMaxChars(s);

  return actual->AddItem(s, val) >= 0;
  }


bool TBCboBx::addItemSorted(TCchar* txt, int val) {
String tgt = txt;
int    index;

  if (!getActual() || !txt) return false;

  index = actual->FindItem(tgt);

  if (index >= 0) tgt = findNext(index);

  setMaxChars(tgt);   return actual->AddSortedItem(tgt, val) >= 0;
  }


String TBCboBx::findNext(int index) {
int     n   = actual->GetCount();
String  tgt = actual->GetItem(index);
int     lng = tgt.length();
int     i;
String  s;

  for (++index, i = 1; index < n; index++, i++) {
    s = actual->GetItem(index);

    if (tgt != s.substr(0, lng)) break;
    }

  return s.format(_T("%s -- %i"), tgt.str(), i);
  }



void TBCboBx::setWidth() {

  if (!getActual()) return;

  ((TBCboBx*)actual)->m_iWidth  = toolBarDim.getHoriz(maxChars) * percent / 100 + 20;
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


void* TBCboBx::getData(int index) {return (void*) (getActual() ? actual->GetItemData(index) : 0);}


bool TBCboBx::getCurSel(String& s, void*& data) {
int i = getCurSel();   if (i < 0) return false;

  s = actual->GetItem(i);  data = (void*) actual->GetItemData(i);  return true;
  }


int TBCboBx::getCurSel() {return getActual() ? actual->GetCurSel() : -1;}


int TBCboBx::find(TCchar* tc) {
  if (!getActual()) return -1;

  return actual->FindItem(tc);
  }


bool TBCboBx::setCurSel(int index) {
  if (!getActual()) return false;

  return actual->SelectItem(index, false);
  }


bool TBCboBx::setCurSel(TCchar* tc) {
  if (!getActual()) return false;

  return actual->SelectItem(tc);
  }


//bool TBCboBx::getActual()


bool TBCboBx::add(TCchar* txt, int data) {

  if (!txt) return false;

  if (FindItem(txt) >= 0) return true;

  setMaxChars(txt);

  return AddItem(txt, data) >= 0;
  }                                                                  //BS_VCENTER


TBCboBx* TBCboBx::finInstall(TCchar* caption) {

  this->caption = caption;   setMaxChars(caption);

  m_iWidth  = toolBarDim.getHoriz(maxChars) + 20;

  m_dwStyle =  CBS_DROPDOWNLIST | WS_VSCROLL;   SetFlatMode(true);

  return this;
  }


