// Manage Window Position and Size


#pragma once


struct WinPosData {
int left;
int top;
int width;
int depth;

  WinPosData() : left(0), top(0), width(0), depth(0) { }
 ~WinPosData() { }

  WinPosData& operator= (RECT& r)
           {left = r.left; top = r.top; width = r.right - r.left; depth = r.bottom - r.top; return *this;}

  operator RECT() {return {left, top, width, depth};}

  void load(RECT& defaultRect);

  void save();

  void normalize(int screenWidth, int maxDepth);
  };


class WinPos {

WinPosData data;                                // App window position and size

int        screenWidth;                         // Maximum Screen width and height
int        screenHeight;

public:

  WinPos();
 ~WinPos() {save();}

  // initialize the window to the saved position and size, only call once

  void initialPos(CWnd* wnd, RECT& defaultRect);

  void set(CRect& curPos) {data = curPos;}

private:

  void save() {data.save();}                     // Save the current position and size when app closes.
  };

