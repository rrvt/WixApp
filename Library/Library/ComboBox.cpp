// Combo Box that integrates with a Tool Bar


#include "pch.h"
#include "ComboBox.h"


bool ComboBox::getWindowText(String& s)
                                    {CString t;   GetWindowText(t);   s = t;   return s.length() != 0;}


void ComboBox::clear() {ResetContent();}


bool ComboBox::getCurSel(String& s) {return getText(GetCurSel(), s);}


bool ComboBox::setCurSel(TCchar* tc) {
String s = tc;
String t = s.isEmpty() ? _T(' ') : s;

return SetCurSel(FindStringExact(-1, t)) >= 0;
}


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

