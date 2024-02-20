// Static Control


#pragma once


class Static : public CStatic {

CWnd* wnd;

public:

  Static() : CStatic(), wnd(0) { }
 ~Static() { }

  bool create(TCchar* text, DWORD style, RECT screenRect, CWnd* parent, uint id = 0xffff);

  void moveWindow(RECT screenRect, bool rePaint = true);

private:

  void screenToClient(RECT& rect) {::MapWindowPoints(0, wnd->m_hWnd, (POINT*)&rect, 2);}
  };

