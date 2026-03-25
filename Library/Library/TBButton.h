// Toolbar Button (i.e. MFC Tool Bar Button)


#pragma once


class TBButton : public MfcTBBtn {

uint      id;

public:

  TBButton(uint id) : MfcTBBtn(id, -1), id(id) { }
 ~TBButton() { }

  uint getId() {return id;}
  void install(TCchar* caption);
  };

