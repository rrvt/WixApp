// Tool Bar, see the following:
//  -- TBButton:     A Button with a caption
//  -- TBComboBox:   A Button with a drop down list from which selections may be made
//  -- TBEditBox:    An Edit Box embedded in the tool bar
//  -- TBMenu: A drop down menu, suitable only with a Frame (i.e. MainFrame)
//  -- TBPopuMenu:   A drop down combo box that can act like a menu with the right operations
//
// An interesting fact is that the tooltips only work on TBButtons and traditional 16x15 Icon
// buttons.  The OnTtnNeedText seems to be called only for IDOK and TBButton buttons.  The other
// icon buttons tooltips magically occur without the modules help.


#pragma once
#include "CbxItem.h"
#include "TBButton.h"
#include "TBCboBoxes.h"
#include "TBEditBox.h"
#include "TBMenu.h"                       // Use for Menus in Doc/View applications
#include "ToolBarDim.h"


class ToolBarBase : public CMFCToolBar {
typedef CMFCToolBarButton ButtonBase;

CWnd*      window;

protected:

TBCboBoxes tbCboBoxes;

public:

  ToolBarBase() : window(0) { }
 ~ToolBarBase() {  }

  bool      create(CWnd* wnd, uint id, DWORD style = 0);
                                                  // Create toolbar with flyby tooltips.

  void      move(CRect& rect);                       // Useful in Dialog Apps only

  void      set(CRect& dlgRect) {toolBarDim.set(dlgRect, this);}

  bool      OnTtnNeedText(NMHDR* pNMHDR);            // ToolTips -- See definition for details

  void      setSeparator(int index);// {SetButtonStyle(index, TBBS_SEPARATOR);}
  void      adjustLayout() {AdjustLayout();}

protected:

  void      getFontDim(FontDim& fontDim);

  bool      add(TBButton&   button, uint id, TCchar* caption);

  bool      add(TBEditBox&  button, uint id, int     noChars);
  CString   getText(TBEditBox&  editBox) {return editBox.getText();}

  // The following Menu items are usefule only in Doc/View (e.g. MainFrame) apps
  bool      add(TBMenu&  mnu, uint id, int      idr,              TCchar* caption);
  bool      add(TBMenu&  mnu, uint id, int      idr,              int     index);
  bool      add(TBMenu&  mnu, uint id, CCbxItem cbxItem[], int n, TCchar* caption);

  // Combo Box Features (Doc/View and Dialog Box Apps)

  TBCboBox& createCboBox(uint id) {return tbCboBoxes.create(id);}
  void      clearCbo(    uint id);
  bool      addCboBox(   uint id, int noChars);
  bool      setCboItems( uint id, uint idr);
  bool      setCboItems( uint id, CCbxItem cbxItem[], int n);

  void      setCboCaption(TBCboBox& cbo, TCchar* caption)
                                              {cbo.set(caption);   cbo.update();   AdjustLayout();}
  void      setImage(TBCboBox& cbo, int toolBarIndex)
                                    {cbo.setImage(toolBarIndex);   cbo.update();   AdjustLayout();}
  void      setImage(TBMenu&   mnu, int toolBarIndex)
                                    {mnu.setImage(toolBarIndex); /*mnu.update();*/ AdjustLayout();}

  void      setCboWthPrct(TBCboBox& cbo, int prcnt)
                              {cbo.setWthPercent(prcnt);   cbo.update();   AdjustLayout();}
  void      setCboHeight( TBCboBox& cbo) {cbo.setHeight();}

  void      dispatch(TBCboBox& cbo);

  // Manipulate tuple in the ComboBox

  bool      addCboItem(TBCboBox& cbo, TCchar* txt, ulongP data) {return cbo.addItem(txt, data);}
  bool      addCboItemSrtd(TBCboBox& cbo, TCchar* txt, ulongP data)
                                                       {return cbo.addItemSrtd(txt, data);}

  bool      setCurSel(TBCboBox& cbo, int index)  {return cbo.setCurSel(index);}

  bool      setCurSel(TBCboBox& cbo, TCchar* tc) {return cbo.setCurSel(tc);}

  int       getCurSel(TBCboBox& cbo)             {return cbo.getCurSel();}
  bool      getCurSel(TBCboBox& cbo, String& s, ulongP& data) {return cbo.getCurSel(s, data);}
  ulongP    getData(TBCboBox& cbo, int index)    {return cbo.getData(index);}

  int       find(TBCboBox& cbo, TCchar* tc)      {return cbo.find(tc);}

private:

  void      OnFillBackground(CDC* pDC);
  bool      getMouseHover(ButtonBase*& btn);
  };



