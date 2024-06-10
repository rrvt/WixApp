// Tool Bar, see the following:
//  -- TBButton:     A Button with a caption
//  -- TBComboBox:   A Button with a drop down list from which selections may be made
//  -- TBEditBox:    An Edit Box embedded in the tool bar
//  -- TBMenu: A drop down menu, suitable only with a Frame (i.e. MainFrame)
//  -- TBPopuMenu:   A drop down combo box that can act like a menu with the right operations
//
// An interesting fact is that the tooltips only work on TBButtons and traditional 16x15 Icon buttons.
// The OnTtnNeedText seems to be called only for IDOK and TBButton buttons.  The other icon buttons
// tooltips magically occur without the modules help.


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




//#include "ExpandableP.h"
//#include "IterT.h"
//#include "TBBtnCtx.h"
//#include "TBComboBox.h"
//struct CbxItem;
#if 0
typedef DatumPtrT<TBBtnCtx, uint> TBBtnCtxP;
class ToolBarBase;
typedef IterT<ToolBarBase, TBBtnCtx>  TlBrIter;

ExpandableP<TBBtnCtx, uint, TBBtnCtxP, 2> data;

data.clear();
#endif
//  void           setWinAttributes(CRect& winRect);
#if 0                                                                  // Add additional style bits as needed
                 // Install the various buttons available on a tool bar
  bool           installBtn(      uint id, TCchar* caption);
  bool           installComboBox( uint id);
  bool           installEditBox(  uint id, int noChars);
  bool           installMenu(     uint id, uint idr, TCchar* caption);
  bool           installMenu(     uint id, uint idr, uint imageIndex);

//  int            commandToIndex(int id) { }
  CMFCToolBarButton* getButton(int iIndex) {return GetButton(iIndex);}

  bool           installPopupMenu(uint id, bool flatMode = false);

  bool           addPopupItems(uint id, const CbxItem* items, int noItems, bool sorted = false);
  bool           addPopupItem( uint id,       CbxItem& item,               bool sorted = false);

                 // Add a resource menu to a popup menu -- has to be called after the popup is installed
                 //  in toolbar
  void           addPopupMenu(   uint id, uint idr, bool sorted = false);
  void           setPopupCaption(uint id, TCchar* caption);
  void           setButtonTxt(uint id, TCchar* caption);
  void           dispatch(       uint id, TCchar* caption); // Dispatch command from popup menu, executed
                                                            // from ON_CBN_SELCHANGE message

                 // Add an array of Items to a combo box with id
  void           addCbxItems(  uint id, CbxItem* items, int nItems, bool sorted = true);

                 // Add a single item to combo box with id
  void           addCbxItem (  uint id, CbxItem& item, bool sorted = true);

                 // Load from Menu Resource idr
  void           addResToCbx(  uint id, uint idr, bool sorted = true);
  void           setCbxCaption(uint id, TCchar* caption);  // Add Caption to combo box,
                                                                        // should be last...
                 // Get current selection or fail
  bool           getCbxSel(uint id, String& s, int & data) {return TBComboBox::getCurSel(id, s, data);}

  bool           getEbxText(uint id, String& txt);
  bool           setEbxText(uint id, TCchar* txt);

  TBMenu*  getMenuBtn(uint id);

#endif
#if 0
  TBBtnCtx&      addCtx(uint id);
  TBBtnCtx*      findCtx(uint id);

  void           adjust(TBBtnCtx& ctx);

  void           getFontDim(TBBtnCtx& ctx);

  // returns either a pointer to data (or datum) at index i in array or zero

  TBBtnCtx* datum(int i) {return 0 <= i && i < nData() ? data[i].p : 0;}

  int       nData()      {return data.end();}                   // returns number of data items in array

  void      removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

  friend typename TlBrIter;
#endif
//  bool           add(TBCbxMenu& button, TCchar* caption);

