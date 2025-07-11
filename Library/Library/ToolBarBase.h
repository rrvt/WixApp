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
#include "TBCboBx.h"
#include "TBCbxMenu.h"                    // Use for Menus on Dialog Box Tool Bars
#include "TBEditBox.h"
#include "ToolBarDim.h"

class  TBButton;
class  TBMenu;


class ToolBarBase : public CMFCToolBar {
typedef CMFCToolBarButton ButtonBase;

CWnd*    window;

public:

  ToolBarBase() : window(0) { }
 ~ToolBarBase() {  }

  bool           create(CWnd* wnd, uint id, DWORD style = 0);
                                                          // Create toolbar with flyby tooltips.

  void           move(CRect& rect);                       // Useful in Dialog Apps only

  void           set(CRect& dlgRect) {toolBarDim.set(dlgRect, this);}

  bool           OnTtnNeedText(NMHDR* pNMHDR);            // ToolTips -- See definition for details

  void           setSeparator(int index);// {SetButtonStyle(index, TBBS_SEPARATOR);}

protected:

  void           getFontDim(FontDim& fontDim);
  bool           add(TBButton&   button, uint id, TCchar* caption);
  bool           add(TBEditBox&  button, uint id, int     noChars);
  CString        getText(TBEditBox&  editBox) {return editBox.getText();}

  // The following Menu items are usefule only in Doc/View (e.g. MainFrame) apps

  bool           add(TBMenu&  button, uint id, int           idr,              TCchar* caption);
  bool           add(TBMenu&  button, uint id, int           idr,              int     index);
  bool           add(TBMenu&  button, uint id, const CbxItem cbxItem[], int n, TCchar* caption);

  // The following Menu items are useful only in Dialog Boxes

  void           setWthPercent(TBCbxMenu& button, int prcnt) {button.setWthPercent(prcnt);}
  bool           add(TBCbxMenu&  button, uint id, int           idr,              TCchar* caption);
  bool           add(TBCbxMenu&  button, uint id, const CbxItem cbxItem[], int n, TCchar* caption);
  void           setWidth(TBCbxMenu& button) {button.setWidth();   AdjustLayout();}
  void           setHeight(TBCbxMenu& button) {button.setHeight();}
  void           dispatch(TBCbxMenu& deleteMenu);

  void           clear(TBCboBx& button) {button.clear();}
  bool           add(TBCboBx&   button, uint id, int noChars);
  bool           add(TBCboBx&   button, uint id, int           idr,              TCchar* caption);
  bool           add(TBCboBx&   button, uint id, const CbxItem cbxItem[], int n, TCchar* caption);

  bool           addItem(      TBCboBx& button, TCchar* txt, int data)
                                                                {return button.addItem(txt, data);}
  bool           addItemSorted(TBCboBx& button, TCchar* txt, int data)
                                                          {return button.addItemSorted(txt, data);}

  int            find(TBCboBx& button, TCchar* txt)      {return button.find(txt);}
  bool           setCurSel(TBCboBx& button, int index)   {return button.setCurSel(index);}
  bool           setCurSel(TBCboBx& button, TCchar* txt) {return button.setCurSel(txt);}

  void           setCaption(   TBCboBx& button, TCchar* caption) {button.setCaption(caption);}
  void           setWthPercent(TBCboBx& button, int prcnt)       {button.setWthPercent(prcnt);}
  void           setWidth(     TBCboBx& button) {button.setWidth();   AdjustLayout();}
  void           setHeight(    TBCboBx& button) {button.setHeight();}

  void*          getCbxData(   TBCboBx& button, int index) {return button.getData(index);}

  bool           getCurSel(    TBCboBx& button, String& s, void*& data)
                                                                {return button.getCurSel(s, data);}
  int            getCurSel(    TBCboBx& button) {return button.getCurSel();}

private:
  void           OnFillBackground(CDC* pDC);
  bool           getMouseHover(ButtonBase*& btn);
  };


