// Status Bar consistent with MainFrame
// The default indicators are the first pane (tool tips) and the last two panes (Caps, Numlock)

#pragma once


const int MaxStsBarInd = 10;

class CMFStatusBar : public CMFCStatusBar {
int    n;
int    nFree;
int    indicators[MaxStsBarInd];
int    percents[MaxStsBarInd];
String text[MaxStsBarInd];

public:

  CMFStatusBar(int nUser);

  bool create(CWnd* mainFrame);
  void setup();
  void setText(int i, TCchar* txt)
                                {int j = i + 1; if (j < n) {SetPaneText(j, txt);   text[i] = txt;}}

private:

  CMFStatusBar() { }
  };

