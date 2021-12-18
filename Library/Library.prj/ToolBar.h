// Tool Bar, see the following:
//  -- TBButton:     A Button with a caption
//  -- TBComboBox:   A Button with a drop down list from which selections may be made
//  -- TBEditBox:    An Edit Box embedded in the tool bar
//  -- TBMenuButton: A drop down menu, suitable only with a Frame (i.e. MainFrame)
//  -- TBPopuMenu:   A drop down combo box that can act like a menu with the right operations


#pragma once
#include "ExpandableP.h"
#include "IterT.h"
#include "TBBtnCtx.h"
#include "TBComboBox.h"

class  TBMenuButton;
struct CbxItem;


typedef DatumPtrT<TBBtnCtx> TBBtnCtxP;
class               ToolBar;
typedef IterT<ToolBar, TBBtnCtx> TlBrIter;


class ToolBar : public CMFCToolBar {

int avgWidth;                     // width and height in pixels
int height;
int winWidth;
int winHeight;

ExpandableP<TBBtnCtx, TBBtnCtxP, 2> data;

public:

  ToolBar() : avgWidth(0), height(0), winWidth(0), winHeight(0) { }
 ~ToolBar() { }

  void           initialize(CRect& winRect);

                 // Install the various buttons available on a tool bar
  bool           installBtn(      uint id, TCchar* caption);
  bool           installComboBox( uint id);
  bool           installEditBox(  uint id, int noChars);
  bool           installMenu(     uint id, uint idr, TCchar* caption);
  bool           installPopupMenu(uint id, uint idr);

                 // Add a resource menu to a popup menu -- has to be called after the popup is installed
                 //  in toolbar
  void           addPopupMenu(    uint id, uint idr, TCchar* caption, bool sorted = false);
  void           dispatch(uint id, TCchar* caption);        // Dispatch command from popup menu, executed
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

  TBMenuButton*  getMenuBtn(uint id);

private:

  TBBtnCtx&      addCtx(uint id);
  TBBtnCtx*      findCtx(uint id);

  void           adjust(TBBtnCtx& ctx);

  void           getFontDim(TBBtnCtx& ctx);

  // returns either a pointer to data (or datum) at index i in array or zero

  TBBtnCtx* datum(int i) {return 0 <= i && i < nData() ? data[i].p : 0;}

  int       nData()      {return data.end();}                   // returns number of data items in array

  void      removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

  friend typename TlBrIter;
  };

