// Toolbar Button (i.e. MFC Tool Bar Button)


#pragma once


class TBButton : public CMFCToolBarButton {
public:

  TBButton(uint id);
 ~TBButton() { }

  void install(TCchar* caption);
  };
