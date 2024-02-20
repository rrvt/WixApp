


#include "pch.h"
#include "Static.h"


bool Static::create(TCchar* text, DWORD style, RECT screenRect, CWnd* parent, uint id) {
bool rslt;

  wnd = parent;  if (!wnd) return false;

  screenToClient(screenRect);   rslt = CStatic::Create(text, style, screenRect, wnd, id);

  SetFont(wnd->GetFont());   return rslt;
  }



void Static::moveWindow(RECT screenRect, bool rePaint) {

  screenToClient(screenRect);   MoveWindow(&screenRect, rePaint);
  }



