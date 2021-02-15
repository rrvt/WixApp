// ToolBar Edit Box


#pragma once




class TBEditBox : public CMFCToolBarEditBoxButton {

public:

  TBEditBox() { }
  TBEditBox(uint id, int image, DWORD style = ES_AUTOHSCROLL, int width = 0) :
                                                    CMFCToolBarEditBoxButton(id, image, style, width) { }
 ~TBEditBox() { }

  static TBEditBox* get(int id);
  };

