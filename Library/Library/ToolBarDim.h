// ToolBar Critical Dimensions (Font and Window)


#pragma once

class ToolBarBase;


struct FontDim {
int width;
int height;

  FontDim() : width(0), height(0) { }

  void set(CDC* dc);
  int getHoriz(int noChars) {return width * noChars;}
  };


struct ToolBarDim {
int      nBtns;
int      nIcns;
int      width;                   // Window width & height
int      height;
int      maxBtnWidth;
FontDim  fontDim;

  ToolBarDim() : nBtns(0), nIcns(0), width(0), height(0), maxBtnWidth(0), fontDim() { }

  void initialize(int nButtons, int nCmds) {nBtns = nButtons;   nIcns = nCmds - nButtons;}

  void set(CRect& dlgRect, ToolBarBase* tb);

  int getHoriz(int noChars);
  };


extern ToolBarDim toolBarDim;
