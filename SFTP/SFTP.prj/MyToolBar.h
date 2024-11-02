// My ToolBar


#pragma once

//#define ButtonDefs
//#define DocViewTB


#ifdef ButtonDefs
#include "TBButton.h"
#include "TBEditBox.h"
#ifdef DocViewTB
#include "TBMenu.h"
#else
#include "TBCbxMenu.h"
#endif
#include "TBCboBx.h"
#endif
#include "ToolBarBase.h"


struct CbxItem;


class MyToolBar : public ToolBarBase {

#ifdef ButtonDefs

TBButton  button;
TBEditBox editBox;

#ifdef DocViewTB

TBMenu    menu;
TBMenu    menu1;

#else

TBCbxMenu cbxMenu;
TBCbxMenu cbxMenu1;

#endif

TBCboBx   cboBx;

#ifdef DocViewTB

TBMenu    saveMenu;

#else

TBCboBx   cboBx1;

#endif
#endif

public:

  MyToolBar();
 ~MyToolBar() { }

  bool    addButton( uint id, TCchar* caption);
  bool    addEditBox(uint id, int     noChars);
  CString getText(uint id);

#ifdef DocViewTB

  bool    addMenu(   uint id, int     idr, TCchar* caption);
  bool    addMenu(   uint id, int     idr, int     index);
  bool    addMenu(   uint id, CbxItem cbxItem[], int n, TCchar* caption);

#else

  bool    addMenu(   uint id, int     idr, TCchar* caption);
  bool    addMenu(   uint id, CbxItem cbxItem[], int n, TCchar* caption);
  void    dispatch(uint id);

#endif

  bool    addCBx(    uint id);
  bool    addCBx(    uint id, int     idr, TCchar* caption);
  bool    addCbxItem(uint id, TCchar* txt, int data);
  bool    addCBx(    uint id, CbxItem cbxItem[], int n, TCchar* caption);
  bool    addCbxItemSorted(uint id, TCchar* txt, int data);
  void    setCaption(uint id, TCchar* caption);
  void    setWidth(uint id);
  bool    getCurSel( uint id, String& s, int& data);
  };

