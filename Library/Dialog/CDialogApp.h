// Subclass of CWinAppEx


#pragma once
#include "PrinterAttr.h"
#include "WinAppEx.h"

class DialogEx;
class PrintDialog;


class CDialogApp : public WinAppEx {

public:

String    appName;
String    appID;
String    version;

DialogEx* dialog{0};

  CDialogApp(CDialogApp* app);
 ~CDialogApp();

  virtual BOOL InitInstance();
          void initPrinter();
  virtual int  ExitInstance();

//  void       saveDevMode( HGLOBAL hndl);
//  void       saveDevNames(HGLOBAL hndl);

private:

  void       getAppName(String& name);

  void       dbg(TCchar* fmt, void* v);

  CDialogApp() : WinAppEx((CDialogApp*)0) { }
  };


extern CDialogApp* theDlgApp;

inline DialogEx*   doc() {return theDlgApp->dialog;}


///-------------------
#if 0
  bool       isDfltPrtr() {return hdlDevMode != 0;}
  void       setDevMode(HANDLE& h) {hdlDevMode = h;   h = 0;}
  HANDLE     getDevMode() {HANDLE h = hdlDevMode;    hdlDevMode = 0;    return h;}
  void       setDevNames(HANDLE& h) {hdlDevNames = h;   h = 0;}
  HANDLE     getDevNames() {HANDLE h = hdlDevNames;   hdlDevNames = 0;   return h;}

//  String&    getPrinterAttr() {prtrDevAttr.get(getDefaultPrinter());   return prtrDevAttr.name;}
//  void       updateDevMode()  {prtrDevAttr.set(getDefaultPrinter());}
//  void       clearDevMode()   {m_hDevMode = m_hDevNames = 0;}

//  bool       getDefaultPrinter(PRINTDLG& pd);

  //  HANDLE     getDefaultPrinter();
#endif

