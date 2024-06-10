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

  TBMenu* install(uint idr,                       TCchar* caption);
  TBMenu* install(uint idr,                       uint    imageIndex);
  TBMenu* install(const CbxItem cbxItem[], int n, TCchar* caption);
  TBMenu* install(const CbxItem cbxItem[], int n, uint    imageIndex);
  bool    setCaption();
  uint    getId() {return id;}
  uint    getCmdId();

private:

  void    addItem(TCchar* txt, int data);
  TBMenu* finInstall(TCchar* caption);
  void    setMaxChars(TCchar* txt) {int t;   t = _tcslen(txt);   if (t > maxChars) maxChars = t;}
  };



