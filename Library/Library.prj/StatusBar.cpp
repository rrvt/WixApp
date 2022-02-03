// Status Bar on Bottom of window


#include "stdafx.h"
#include "StatusBar.h"


bool StatusBar::create(CWnd* wnd, uint id) {
CRect rect = CRect(0, 0, 0, 0);
int   parts[2];

  if (!wnd) return false;

  if (!Create(WS_CHILD | WS_VISIBLE | CCS_BOTTOM, rect, wnd, id)) return false;

  GetClientRect(&rect);   parts[0] = rect.right/2;  parts[1] =  -1;

  return SetParts(noElements(parts), parts);
  }


void StatusBar::setReady(bool r) {
String s = r ? _T("Ready") : _T("Not Ready");

  ready = r;   setText(0, s);
  }


bool StatusBar::setText(int index, TCchar* txt) {
  return SetText(txt, index, 0);
  }

