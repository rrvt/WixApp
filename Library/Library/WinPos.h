// Manage Window Position and Size


#pragma once


struct WinPosData {
int left;                     // Window coordinates in pixels including hidden border, i.e. from
int top;                      // GetWindowRect
int width;
int depth;

int invXbdr;                  // Invisible borders
int invYbdr;

int defWidth;                 // Width and depth when loaded
int defDepth;
int minWidth;                 // Minimums in pixels
int minDepth;


  WinPosData();
 ~WinPosData() { }

  WinPosData& set(CWnd* wnd, CRect& r);                       // Call with rectangle from GetWindowRect
  void        set(CWnd* wnd, int& cx, int& cy);               // Call from OnSize message function
  void        get(CRect& r) {r.right = r.left + width;   r.bottom = r.top + depth;}

  void        setInvBdrs(CRect& winRect, int cx, int cy);

  operator    CRect() {return {left, top, width, depth};}

  void        load(CRect& defaultRect);

  bool        setPos(CWnd* wnd);

  void        save();

  void        normalize(int screenWidth, int maxDepth);

private:

  void getWidthDepth(CWnd* wnd, int w, int d);  // width and depth in pixels including invisible boarders

  void rationalize();

  void display(TCchar* tgt, int d);
  };


class WinPos {

WinPosData data;                                  // App window position and size in pixels

CWnd*      wnd;                                   // MainFrame or Dialog Window
double     hDLUtoPxls;                            // Factor (dlu * factore = pixels) to convert DLUs to
double     vDLUtoPxls;                            // pixelse, different for horizontal and vertical
int        defWidth;                              // Width and depth in DLUs specified in Resource File
int        defDepth;
int        screenWidth;                           // Maximum Screen width and height
int        screenHeight;

public:

  WinPos();
 ~WinPos() {if (wnd) save();  wnd = 0;}

  void setDLUToPxls(CRect& rect, int hDLU, int vDLU);   // Window Rectangle in pixels and
                                                        // Dialog box h&v Device Logical units
                                                        // Window Rectangle in pixels
  int  dluToScreen(int dlu, bool horiz = true);

  // initialize the window to the saved position and size, only call once

  void initialPos(CWnd* wnd, CRect& defaultRect);
  bool setPos() {return data.setPos(wnd);}

  void set(CRect& curPos)    {data.set(wnd, curPos);}
  void set(int& cx, int& cy) {data.set(wnd, cx, cy);}

  void setInvBdrs(CRect& winRect, int cx, int cy) {data.setInvBdrs(winRect, cx, cy);}

  CRect get() {return data;}


private:

  void save() {data.save();}                // Save the current position and size when app closes.
  };


extern WinPos winPos;
