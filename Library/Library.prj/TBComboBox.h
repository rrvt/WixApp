// ToolBar Combo Box -- Useful for data and menu's


#pragma once
#include "CbxItem.h"

struct TBBtnCtx;


class TBComboBox : public CMFCToolBarComboBoxButton {
public:

  TBComboBox(uint id);
  TBComboBox(TBComboBox& popup) {CopyFrom(popup);}
 ~TBComboBox() { }

         TBComboBox& operator= (TBComboBox& popup) {CopyFrom(popup); return *this;}

         void        install(TBBtnCtx& ctx);

  static bool        addItems(  uint id, const CbxItem* items, int noItems, TBBtnCtx& ctx, bool sorted);
  static bool        addItem(   uint id,       CbxItem& item,               TBBtnCtx& ctx, bool sorted);
  static bool        addRes(    uint id,       uint     idr,                TBBtnCtx& ctx, bool sorted);
  static bool        setCaption(uint id,       TCchar*  txt,                TBBtnCtx& ctx);

  static uint        getCmdId(uint id, TCchar* caption);         // Used to execute a command
  static bool        getCurSel(uint id, String& s, int& data);   // Returns current selection when true
  static TBComboBox* get(      uint id);
  static CComboBox*  getCBx(   uint id);

         int         find(TCchar* tc) {String s = tc; return find(s);}
         int         find(String& s);

private:
         bool        addItems(const CbxItem* items, int noItems, TBBtnCtx& ctx, bool sorted);
         void        addItem( const CbxItem& item, TBBtnCtx& ctx, bool sorted);
         bool        addRes(        uint     idr,  TBBtnCtx& ctx, bool sorted);
         void        setCaption(TCchar* txt);
         uint        getCmdId();                      // Get command ID in current selection of drop down
         bool        getCurSel(String& s, int& data); // Returns current selection of if none faile

         void        setDim(TBBtnCtx& ctx);
  };

