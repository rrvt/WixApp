// Tool Bar ComboBoxes



#include "pch.h"
#include "TBCboBoxes.h"


TBCboBox& TBCboBoxes::create(uint id) {
TBCboBox* cbo = data.find(id);
NewAlloc(TBCboBox);

  if (!cbo) data = cbo = AllocNodeI1(id);

  return *cbo;
  }


TBCboBox* TBCboBoxes::install(uint id, int noChars) {
TBCboBox* cbo = data.find(id);   if (!cbo) return 0;

  cbo->set(noChars);   return cbo;
  }


void TBCboBox::set(TCchar* caption) {this->caption = caption;   set(tcslen(caption));}


void TBCboBox::set(int noChars) {if (noChars > maxChars) maxChars = noChars;}


void TBCboBox::setHeight() {
int count;
int pixels;
int maxHeight = (toolBarDim.height/25 - 3) * 25;

  count = (int) (*this)()->GetCount();   if (!count) return;

  pixels = count * 25;   pixels = pixels < 150 ? 150 : pixels > maxHeight ? maxHeight : pixels;

  (*this)()->SetDropDownHeight(pixels);
  }


// Set Cbo Items from a resource menu

bool TBCboBox::setItems(int idr) {
CMenu      menu;
uint       i;
uint       n;
Cstring    txt;
uint       data;
bool       rslt = true;

  if (!menu.LoadMenu(idr)) return false;

    for (i = 0, n = menu.GetMenuItemCount(); i < n; i++) {

      menu.GetMenuString(i, txt, MF_BYPOSITION);   data = menu.GetMenuItemID(i);

      rslt = addItem(txt, data);   if (!rslt) break;
      }

  menu.DestroyMenu();   setHeight();   return rslt;
  }


// Set Cbo Items from an vector of items

bool TBCboBox::setItems(CCbxItem cbxItem[], int n) {
int  i;

  for (i = 0, maxChars = 0; i < n; i++)
                  {CCbxItem& item = cbxItem[i];   if (!addItem(item.txt, item.data)) return false;}
  setHeight();   return true;
  }


bool TBCboBox::addItem(TCchar* txt, ulongP data) {

  if (!txt) return false;

  if ((*this)()->FindItem(txt) >= 0) return true;

  set(tcslen(txt));   return (*this)()->AddItem(txt, data) >= 0;
  }


bool TBCboBox::addItemSrtd(TCchar* txt, ulongP data) {

  if (!txt) return false;

  if ((*this)()->FindItem(txt) >= 0) return true;

  set(tcslen(txt));   return (*this)()->AddSortedItem(txt, data) >= 0;
  }


void TBCboBox::update() {

  (*this)()->m_iWidth = toolBarDim.getHoriz(maxChars) * percent / 100 + 20;

  (*this)()->SetText(caption);

  (*this)()->m_dwStyle =  CBS_DROPDOWNLIST | WS_VSCROLL;   SetFlatMode(true);
  }


// Manipulate tuple in the ComboBox

bool TBCboBox::getCurSel(String& s, ulongP& data) {
int i = getCurSel();   if (i < 0) return false;

  s = (*this)()->GetItem(i);  data = getData(i);  return true;
  }


ulongP TBCboBox::getCmdId() {int i = getCurSel();   return i >= 0 ? getData(i) : 0;}



