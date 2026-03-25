// My ToolBar


#pragma once
#include "TBButton.h"
#include "TBEditBox.h"
#include "TBCboBoxes.h"
#ifdef DocView
#include "TBMenu.h"
#endif

#include "ToolBarBase.h"

struct CbxItem;


class MyToolBar : public ToolBarBase {

TBButton  button;
TBButton  button1;
//TBButton  button2;
TBButton  button3;
TBCboBox& saveMenu;

#if 0
TBButton  button;
TBEditBox editBox;
#ifdef DocView
TBMenu    menu;
TBMenu    menu1;
TBMenu    saveMenu;
#else
TBCboBox& cboMenu1;
#endif
TBCboBox& cboBox;
TBCboBox& cboBox1;
#endif

public:

  MyToolBar();
 ~MyToolBar() { }

  bool    addButton( uint id, TCchar* caption);

  bool    addEditBox(uint id, int     noChars);
  CString getText(uint id);

#ifdef DocView
  bool    addMenu(uint id, int idr, TCchar* caption);         // uses icon at index on toolbar
  bool    addMenu(uint id, int idr, int index);               // uses icon at index on toolbar
  bool    addMenu(uint id, CCbxItem cbxItem[], int n, TCchar* caption);
#endif

  void    clearCbo(      uint id)              {ToolBarBase::clearCbo(id);}
  bool    addCboBox(     uint id)              {return ToolBarBase::addCboBox(id, 12);}
  bool    setCboItems(   uint id, uint    idr) {return ToolBarBase::setCboItems(id, idr);}
  bool    setCboItems(   uint id, CCbxItem cbxItem[], int n)
                                               {return ToolBarBase::setCboItems(id, cbxItem, n);}
  void    setCboCaption( uint id, TCchar* txt);
  void    setImage(      uint id, int toolBarIndex);
  void    setCboWthPrct( uint id, int percent);
  void    setCboHeight(  uint id);
  void    dispatch(uint id);

  // Manipulate tuple in the ComboBox

  bool    addCboItem(    uint id, TCchar* txt, ulongP data = 0);
  bool    addCboItemSrtd(uint id, TCchar* txt, ulongP data = 0);

  bool    setCurSel(uint id, int index);

  bool    setCurSel(uint id, TCchar* tc);

  int     getCurSel(uint id);
  bool    getCurSel(uint id, String& s, ulongP& data);
  ulongP  getData(uint id, int index);

  int     find(uint id, TCchar* tc);

  void    adjustLayout() {ToolBarBase::adjustLayout();}
  };


