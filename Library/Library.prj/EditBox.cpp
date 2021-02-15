// Edit Box
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.


#include "stdafx.h"
#include "EditBox.h"


void EditBox::create(int vPos, RECT& rect, CWnd* view, int id, bool integerOnly) {
DWORD flags = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL;

  vertPos = vPos;

  if (!created) {

    if (integerOnly) flags |= ES_NUMBER;

    if (Create(flags, rect, view, id)) {
      created = true;

      if (!initialVal.empty()) SetWindowText(initialVal);
      }
    }
  }


void EditBox::getRect(RECT& rect) {if (created) GetRect(&rect);}


void EditBox::getWindowPos(HWND hWnd, int* x, int* y) {
CWnd* hWndParent = GetParent();                                     //HWND hWnd
POINT p          = {0};

  MapWindowPoints(hWndParent, &p, 1);                               // hWnd,

  (*x) = p.x;
  (*y) = p.y;
  }


void EditBox::getLine(String& line) {
Tchar stg[100];
int   n;

  if (!created) {line = initialVal; return;}

  n = GetLine(0, stg, noElements(stg));

  if (0 <= n && n < noElements(stg) - 1) {stg[n] = 0;  line = stg; return;}

  line = "input to big";
  }


void EditBox::setFocus(bool home) {
int pos = home ? 0 : -1;

  if (created) {
    SetFocus();
    SetSel(0, pos);
    SetSel(pos);
    }
  }

//  void setHome()  {if (created) {SetFocus(); SetSel(0,  0); SetSel(-1);}}

