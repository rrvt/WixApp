// Subclass of CWinAppEx


#pragma once
#include "CMainFrm.h"
#include "Printer.h"


class CApp : public CWinAppEx {

CDocument* doc;
CView*     view;

DEVMODE    devMode;

public:

String appID;
String version;

  CApp(CApp* app);
 ~CApp();
  virtual BOOL InitInstance() {return CWinAppEx::InitInstance();}
  virtual int  ExitInstance();

  // Title becomes:  <app name> -- <title> or just <title> (when setTitle alone is called)

  void       setAppName(  TCchar* appName);   //{getMainFrame()->setAppName(appName);}
  void       setTitle(    TCchar* rightPart); //{getMainFrame()->setTitle(rightPart);}

  CDocument* getDoc();
  CView*     getView();
  void       initPrinterAttr() {printer.initAttr(getDevMode());}
  void       savePrinterAttr() {printer.saveAttr(getDevMode());}
  String&    getPrinterName()  {return printer.getName(getDevMode());}

//  HANDLE     swapDevMode(HANDLE newDevMode);
  void       invalidate() {if (getView()) view->Invalidate();}

  void       setupPrinterDlg() {CWinApp::OnFilePrintSetup();}

  bool       sendCommand(uint command)
               {SendMessage(getMainFrame()->GetSafeHwnd(), WM_COMMAND, MAKEWPARAM(command, 0), 0);}

private:

  HANDLE     getDevMode();

  CMainFrm* getMainFrame() {return (CMainFrm*) m_pMainWnd;}

  CApp() : doc(0), view(0) { }
  };



extern CApp* theCApp;

class CDoc;
class CScrView;

inline CDoc*     cDoc()  {return (CDoc*)     theCApp->getDoc();}
inline CScrView* cView() {return (CScrView*) theCApp->getView();}

