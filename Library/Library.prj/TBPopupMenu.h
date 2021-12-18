// ToolBar Simulated Drop Down Menu -- Useful in dialog boxes


#pragma once

struct TBBtnCtx;


class TBPopupMenu : public CMFCToolBarComboBoxButton {

public:

  TBPopupMenu(uint id);
  TBPopupMenu(TBPopupMenu& popup) {CopyFrom(popup);}
 ~TBPopupMenu() { }

         TBPopupMenu& operator= (TBPopupMenu& popup) {CopyFrom(popup); return *this;}

         void         install(TBBtnCtx& ctx);                        //uint idr, TBBtnCtx& ctx

  static bool         addMenu(   uint id, uint idr, TCchar* caption, TBBtnCtx& ctx, bool sorted);
  static void         setCaption(uint id, TCchar* txt, TBBtnCtx& ctx);

  static uint         getCmdId(uint id, TCchar* caption);

  static TBPopupMenu* get(uint id);                 // Get the popup menu given the toolbar id
                                                    // menu given by the toolbar id.
private:
         void         setCaption(TCchar* txt);
         uint         getCmdId();                   // Get command ID in current selection of drop down

         bool         addMenu(uint idr, TCchar* caption, TBBtnCtx& ctx, bool sorted);

         void         setDim(TBBtnCtx& ctx);
  };

