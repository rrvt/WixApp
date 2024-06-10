// Status Bar on Bottom of window


#include "pch.h"
#include "StatusBar.h"
#include "WinPos.h"


bool StatusBar::create(CWnd* wnd, uint id) {
CRect rect = CRect(0, 0, 0, 0);

  if (!wnd) return false;

  if (!Create(WS_CHILD | WS_VISIBLE | CCS_BOTTOM, rect, wnd, id)) return false;

  return setParts();
  }


void StatusBar::setReady(bool r)
                            {String s = r ? _T("Ready") : _T("Not Ready");   ready = r;   setText(0, s);}


bool StatusBar::setText(int index, TCchar* txt) {return SetText(txt, index, 0);}


void StatusBar::move(CRect& winRect) {
CRect rect;
int   vDelta = winPos.dluToScreen(10, false);

  GetWindowRect(&rect);

  rect.top    = winRect.bottom - vDelta;
  rect.bottom = winRect.bottom;
  rect.right  = rect.left + winRect.right - winRect.left;

  ScreenToClient(rect);   MoveWindow(&rect);   setParts();
  }


bool StatusBar::setParts() {
CRect rect;
int   parts[2];

  GetClientRect(&rect);   parts[0] = rect.right/2;  parts[1] =  -1;

  return SetParts(noElements(parts), parts);
  }




