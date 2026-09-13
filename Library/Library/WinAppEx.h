// WinAppEx -- provides common data for Dialog and DocView Apps


#pragma once

class PrintDialog;



class WinAppEx : public CWinAppEx {

HANDLE hdlDevMode{0};
HANDLE hdlDevNames{0};

public:

  WinAppEx(WinAppEx* app);
 ~WinAppEx();

  virtual BOOL   InitInstance();

          bool   isDfltPrtr() {return hdlDevMode != 0;}
          void   setDevMode(HANDLE& h);// {hdlDevMode = h;   h = 0;}
          HANDLE getDevMode() {HANDLE h = hdlDevMode;    hdlDevMode = 0;    return h;}
          void   setDevNames(HANDLE& h);// {hdlDevNames = h;   h = 0;}
          HANDLE getDevNames() {HANDLE h = hdlDevNames;   hdlDevNames = 0;   return h;}

          void   setDfltDevMode(PrintDialog& dlg);


private:

  WinAppEx() { }
  };


extern WinAppEx* winAppEx;
