// Status Bar on Bottom of window


#pragma once


class StatusBar : public CStatusBarCtrl {           //

bool ready;                   // Just a boolean to indicate Ready/Not Ready in the status bar

public:

  StatusBar() : CStatusBarCtrl(), ready(false) { }
 ~StatusBar() { }

  bool create(CWnd* wnd, uint id);
  void setReady(bool r = true);
  bool isReady() {return ready;}

  bool setText(TCchar* txt) {return setText(0, txt);}
  bool setText(int index, TCchar* txt);

  void move(CRect& rect);

private:

  bool setParts();
  };

