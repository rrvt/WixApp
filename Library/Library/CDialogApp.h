// Subclass of CWinAppEx


#pragma once
#include "Printer.h"


class CDialogApp : public CWinAppEx {

DEVMODE    devMode;

public:

String appID;
String version;

  CDialogApp(CDialogApp* app);
 ~CDialogApp();

  virtual BOOL InitInstance() {return CWinAppEx::InitInstance();}
  virtual int  ExitInstance();

  // Title becomes:  <app name> -- <title> or just <title> (when setTitle alone is called)

  void       setAppName(  TCchar* appName);
  void       setTitle(    TCchar* rightPart);

  void       initPrinterAttr() {printer.initAttr(getDevMode());}
  void       savePrinterAttr() {printer.saveAttr(getDevMode());}
  String&    getPrinterName()  {return printer.getName(getDevMode());}

  void       setupPrinterDlg() {CWinApp::OnFilePrintSetup();}

private:

  HANDLE     getDevMode();

  CDialogApp() { }
  };


extern CDialogApp* theCDialogApp;


///-------------------

