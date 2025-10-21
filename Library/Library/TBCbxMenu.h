// Drop Down Menu constructed in a Combo Box


#pragma once
#include "CbxItem.h"
#include "ToolBarDim.h"


class TBCbxMenu : public CMFCToolBarComboBoxButton {

uint                       id;
int                        maxChars;
int                        percent;                           // width = maxChars * percent / 100;
CMFCToolBarComboBoxButton* actual;

public:

String caption;

  TBCbxMenu(uint id) :
                CMFCToolBarComboBoxButton(id, -1), id(id), maxChars(0), percent(100), actual(0) { }
 ~TBCbxMenu() { }

  TBCbxMenu& install(int           idr,              TCchar* caption);
  TBCbxMenu& install(const CbxItem cbxItem[], int n, TCchar* caption);

  void       setWthPercent(int prcnt) {percent = prcnt ? prcnt : 1;}
                                                               // width = maxChars * percent / 100;
  bool       setCaption();
  uint       getId() {return id;}
  uint       getCmdId();

  void       setWidth();
  void       setHeight();

private:

  bool        getActual();
  int         getWidth() {return toolBarDim.getHoriz(maxChars) * percent / 100 + 20;}

  void        addItem(TCchar* txt, int data);
  TBCbxMenu&  finInstall(TCchar* caption);
  void        setMaxChars(TCchar* txt) {int t;   t = _tcslen(txt);  if (t > maxChars) maxChars = t;}
  };

