// Toolbar Button (i.e. MFC Tool Bar Button)


#pragma once


class TBButton : public CMFCToolBarButton {

uint id;

public:

  TBButton(uint id);
 ~TBButton() { }

  void install(TCchar* caption);
  uint getId() {return id;}
  };

