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
#include "TBEditBox.h"
#include "ToolBarDim.h"

class  TBButton;
class  TBMenu;
class  TBCbxMenu;


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

  bool           add(TBCbxMenu&  button, uint id, int           idr,              TCchar* caption);
  bool           add(TBCbxMenu&  button, uint id, const CbxItem cbxItem[], int n, TCchar* caption);
  void           dispatch(TBCbxMenu& cbxMenu);

  bool           add(TBCboBx& button, uint id, int noChars);
  bool           add(TBCboBx& button, uint id, int           idr,              TCchar* caption);
  bool           add(TBCboBx& button, uint id, const CbxItem cbxItem[], int n, TCchar* caption);

  bool           addItem(      TBCboBx& button, TCchar* txt, int data)
                                                         {return button.addItem(txt, data);}
  bool           addItemSorted(TBCboBx& button, TCchar* txt, int data)
                                                         {return button.addItemSorted(txt, data);}
  void           setCaption(TBCboBx& button, TCchar* caption) {button.setCaption(caption);}
  void           setWidth(  TBCboBx& button) {button.setWidth();   AdjustLayout();}
  void           setHeight( TBCboBx& button) {button.setHeight();}
  bool           getCurSel(TBCboBx& button, String& s, int& data)
                                                                {return button.getCurSel(s, data);}

private:
  void           OnFillBackground(CDC* pDC);
  bool           getMouseHover(ButtonBase*& btn);
  };


