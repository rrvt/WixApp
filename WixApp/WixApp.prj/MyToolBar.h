// My ToolBar


#pragma once
#include "TBButton.h"
#include "ToolBarBase.h"


struct CbxItem;


class MyToolBar : public ToolBarBase {
TBButton  button;
TBButton  button1;
TBButton  button2;
TBButton  button3;

public:

  MyToolBar();
 ~MyToolBar() { }

  bool    addButton( uint id, TCchar* caption);
  };



/////////-----------------

//#define DocViewTB
#if 0       //def DocViewTB
#include "TBMenu.h"
#else
#endif
#if 0  //def DocViewTB

TBMenu    menu;
TBMenu    menu1;

#else
#endif
#if 0    //def DocViewTB

TBMenu    saveMenu;

#else
#endif
#if 0     //def DocViewTB

  bool    addMenu(   uint id, int     idr, TCchar* caption);
  bool    addMenu(   uint id, int     idr, int     index);
  bool    addMenu(   uint id, CbxItem cbxItem[], int n, TCchar* caption);

#else
#endif
//#define MyButtonDefs
#if 0    //def ButtonDefs
#include "TBButton.h"
#include "TBEditBox.h"
#include "TBCbxMenu.h"
#include "TBCboBx.h"
#endif
#if 0     //def ButtonDefs

TBButton  button;
TBEditBox editBox;


TBCbxMenu cbxMenu;
TBCbxMenu cbxMenu1;


TBCboBx   cboBx;


TBCboBx   cboBx1;

#endif
//  bool    addEditBox(uint id, int     noChars);
//  CString getText(uint id);


//  bool    addMenu(   uint id, int     idr, TCchar* caption);
//  bool    addMenu(   uint id, CbxItem cbxItem[], int n, TCchar* caption);
//  void    dispatch(uint id);


//  bool    addCBx(    uint id);
#if 0
  bool    addCBx(    uint id, int     idr, TCchar* caption);
  bool    addCbxItem(uint id, TCchar* txt, int data);
  bool    addCBx(    uint id, CbxItem cbxItem[], int n, TCchar* caption);
  bool    addCbxItemSorted(uint id, TCchar* txt, int data);
  void    setCaption(uint id, TCchar* caption);
  void    setWidth(uint id);
  bool    getCurSel( uint id, String& s, int& data);
#endif

