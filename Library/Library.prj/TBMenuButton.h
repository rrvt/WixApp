// ToolBar Drop Down Menu, not useful in a dialog box -- see TBPopupMenu


#pragma once


class TBMenuButton :  public CMFCToolBarMenuButton {
public:

  TBMenuButton(uint id);
 ~TBMenuButton() { }

  bool install(uint idr, TCchar* caption);
  bool install(uint idr, uint imageIndex);
  };

