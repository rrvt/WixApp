// Drop Down Menu constructed in a Combo Box


#pragma once
#include "CbxItem.h"


class TBCbxMenu : public CMFCToolBarComboBoxButton {

uint                       id;
int                        maxChars;
CMFCToolBarComboBoxButton* actual;

public:

String caption;

  TBCbxMenu(uint id) : CMFCToolBarComboBoxButton(id, -1), id(id), maxChars(0), actual(0) { }
 ~TBCbxMenu() { }

  TBCbxMenu& install(int           idr,              TCchar* caption);
  TBCbxMenu& install(const CbxItem cbxItem[], int n, TCchar* caption);
  bool       setCaption();
  uint       getId() {return id;}
  uint       getCmdId();

private:

  bool        getActual();

  void       addItem(TCchar* txt, int data);
  TBCbxMenu& finInstall(TCchar* caption);
  void       setMaxChars(TCchar* txt) {int t;   t = _tcslen(txt);   if (t > maxChars) maxChars = t;}
  };

