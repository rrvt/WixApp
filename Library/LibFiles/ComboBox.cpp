// Combo Box that integrates with a Tool Bar


#include "stdafx.h"
#include "ComboBox.h"


bool ComboBox::create(ToolBar& toolBar, int toolBarItemID, int itemID, int width,
                                                                           int dropHeight, ulong style) {
ulong     styl = CBS_AUTOHSCROLL | WS_TABSTOP | WS_VISIBLE | WS_CHILD | style;
CRect     rect;
int       nIdx;

  if (::IsWindow(m_hWnd)) return false;

  nIdx = toolBar.CommandToIndex(toolBarItemID);    ASSERT(nIdx >= 0);

  toolBar.SetButtonInfo(nIdx, itemID, TBBS_SEPARATOR, width);

  toolBar.GetItemRect(nIdx, &rect);   rect.top = 1;   rect.bottom = rect.top + dropHeight;

  if (!Create(styl, rect, &toolBar, itemID))
                                        {TRACE0("Failed to create combobox in ToolBar\n"); return false;}
  SetFont(&toolBar.font); return true;                        // update the combo with a font.
  }


bool ComboBox::getWindowText(String& s)
                                    {CString t;   GetWindowText(t);   s = t;   return s.length() != 0;}


void ComboBox::clear() {ResetContent();}


bool ComboBox::getCurSel(String& s) {return getText(GetCurSel(), s);}


bool ComboBox::setCurSel(String& s)
                {String t = s.isEmpty() ? _T(' ') : s;   return SetCurSel(FindStringExact(-1, t)) >= 0;}


bool ComboBox::getText(int i, String& s) {
CString t;

  s.clear();

  if (i < 0) return false;

  GetLBText(i, t);   s = t;   return true;
  }


String* ComboBox::getDatum(int i, String& s) {
CString t;

  if (0 <= i && i < nData()) {GetLBText(i, t);   s = t;   return &s;}

  return 0;
  }


void ComboBox::del(String& s)
                      {int i;   if (!s.empty()) {i = find(s);   if (i >= 0) DeleteString(i);}   return;}


void ComboBox::add(String& s) {if (!s.empty() && find(s) < 0) AddString(s);}


int ComboBox::find(TCchar* s) {return FindStringExact(-1, s);}

