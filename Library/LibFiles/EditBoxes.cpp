// Edit Box
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.


#include "stdafx.h"
#include "EditBoxes.h"


EditBoxes editBoxes;


void EditBoxes::create(int vPos, int editBoxX, int x, int y, int width, int height) {
CPoint scl       = ((CScrollView*)view)->GetScrollPosition();
int    yScrolled = y - scl.y;

CRect  rect(x, yScrolled, x+width, yScrolled+height);

  boxes[editBoxX].p->create(vPos, rect, view, editBoxX);
  }


void EditBox::create(int vPos, RECT& rect, CWnd* view, int id, bool integerOnly) {
DWORD flags = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER;

  vertPos = vPos;

  if (!created) {

    if (integerOnly) flags |= ES_NUMBER;

    if (box->Create(flags, rect, view, 1000 + id)) {
      created = true;

      if (!initialVal.empty()) box->SetWindowText(initialVal);
      }
    }
  }


void EditBox::getRect(RECT& rect) {if (created) box->GetRect(&rect);}


void EditBox::getWindowPos(HWND hWnd, int* x, int* y) {
HWND  hWndParent = GetParent(hWnd);
POINT p          = {0};

  MapWindowPoints(hWnd, hWndParent, &p, 1);

  (*x) = p.x;
  (*y) = p.y;
  }


void EditBox::getLine(String& line) {
Tchar   stg[100];
int    n;

  if (!created) {line = initialVal; return;}

  n = box->GetLine(0, stg, 100);

  if (0 <= n && n < noElements(stg) - 1) {stg[n] = 0;  line = stg; return;}

  line = "input to big";
  }


void EditBoxes::clear() {
int i;

  for (i = 0; i <  boxes.end(); i++) boxes[i].p->clearBox();
  }



void EditBoxX::setFocus() {if (x > -1) editBoxes.setFocus(x);}

