// Display Manager -- needed to isolate variables from Print Management


#pragma once
#include "ShowMgr.h"


class CScrView;


class DsplyMgr : public ShowMgr {

NotePad npd;

public:

  DsplyMgr(CScrView& view) : ShowMgr(_T("Dsply"), view, npd) { }
 ~DsplyMgr() { }

  void OnPrepareDC(CDC* dc);
  void startDev() {dspDev.startDev();}
  void OnDraw(CDC* pDC);
  void setScrollSize();

  void suppressOutput() {dspDev.suppressOutput();}
  void negateSuppress() {dspDev.negateSuppress();}

  void disableWrap()    {dspDev.disableWrap();}
  void enableWrap()     {dspDev.enableWrap();}

private:
  DsplyMgr() : ShowMgr(_T(""), *(CScrView*)0, *(NotePad*)0) { }
  };

