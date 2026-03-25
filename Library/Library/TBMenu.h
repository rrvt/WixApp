// ToolBar Drop Down Menu, not useful in a dialog box -- see TBPopupMenu


#pragma once
#include "CbxItem.h"


class TBMenu :  public CMFCToolBarMenuButton {

uint id;
int  maxChars;

public:

String caption;

  TBMenu(uint id) : CMFCToolBarMenuButton(id, 0, 0), id(id), maxChars(0) { }
 ~TBMenu() { }

  TBMenu* install(uint idr,                  TCchar* caption);
  TBMenu* install(uint idr,                  uint    imageIndex);
  TBMenu* install(CCbxItem cbxItem[], int n, TCchar* caption);
  TBMenu* install(CCbxItem cbxItem[], int n, uint    imageIndex);

  void    setImage(int imageIndex) {SetImage(imageIndex);}
  bool    setCaption();
  uint    getId() {return id;}

private:

  void    setMaxChars(TCchar* txt) {int t;   t = tcslen(txt);   if (t > maxChars) maxChars = t;}
  };



