// Subclass of CWinAppEx


#pragma once
#include "Printer.h"


class CDialogApp : public CWinAppEx {

DEVMODE    devMode;

public:

String appID;
String version;

  CDialogApp(CDialogApp* app);
 ~CDialogApp() {
    #ifdef DebugMemoryLeaks
      _CrtDumpMemoryLeaks();
    #endif
    }

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

//#include "CMainFrm.h"
#if 0
class CDoc;
class CScrView;

inline CDoc*     cDoc()  {return (CDoc*)     theCDialogApp->getDoc();}
inline CScrView* cView() {return (CScrView*) theCDialogApp->getView();}
#endif

//  CMainFrm* getMainFrame() {return (CMainFrm*) m_pMainWnd;}
#if 0
CDocument* doc;
CView*     view;
#endif
//  void       invalidate() {if (getView()) view->Invalidate();}
//{getMainFrame()->setAppName(appName);}
//{getMainFrame()->setTitle(rightPart);}

//  HANDLE     swapDevMode(HANDLE newDevMode);

