// Manage Window Position and Size


#pragma once


class WindowPos {
int   maxWidth;                         // Maximum Screen width and height
int   maxHeight;
CWnd* window;
int   dfltLeft;
int   dfltTop;
int   dfltWidth;
int   dfltHeight;

RECT winRect;                           // Holds current window dimensions

public:

  WindowPos();
 ~WindowPos();

  void getScreenData(int& width, int& height) {
    RECT rsys;   SystemParametersInfo(SPI_GETWORKAREA, 0, &rsys, 0);
    width = rsys.right; height = rsys.bottom;
    }

  void setDefaultPos(int  width, int  height)
                              {dfltLeft = 0; dfltTop = 0; dfltWidth = width; dfltHeight = height;}

  void set(CWnd* w);              // Call with "this" from end of MainFrame::OnCreate
                                  // to initialize the window

  void getRect() {getWindow(); if (window) window->GetWindowRect(&winRect);}
                                  // Call from ManFrame::OnMove and OnSize

private:

  void normalizeWindowRect(int& left, int& top, int& width, int& height);

  CWnd* getWindow() {if (!window) window = AfxGetApp()->m_pMainWnd; return window;}
                                      // Position and Size to the same as at the last closure
  void save();                        // Save the current position and size when app closes.
  };



extern WindowPos windowPos;
