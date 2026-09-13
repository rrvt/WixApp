// Subclass of CWinAppEx


#pragma once
#include "CMainFrm.h"
//#include "Printer.h"
#include "WinAppEx.h"


class CApp : public WinAppEx {

CDocument* doc;
CView*     view;

//DEVMODE    devMode;

public:

String name;
String appID;
String version;

  CApp(CApp* app);
 ~CApp();
  virtual BOOL InitInstance();
//          void initPrinter() {/*if (printer.updateAttr(getPrinterAttr())) updateDevMode();*/}

  virtual int  ExitInstance();

  // Title becomes:  <app name> -- <title> or just <title> (when setTitle alone is called)

  void       setAppName(  TCchar* appName);   //{getMainFrame()->setAppName(appName);}
  void       setTitle(    TCchar* rightPart); //{getMainFrame()->setTitle(rightPart);}

  CDocument* getDoc();
  CView*     getView();

//  String&    getPrinterAttr() {printer.getDevMode(getDefaultPrinter());   return printer.name;}
//  void       setDevMode()     {printer.setDevMode(getDefaultPrinter());}
//  void       clearDevMode()   {m_hDevMode = m_hDevNames = 0;}

  void       onFilePrintSetup() {CWinApp::OnFilePrintSetup();}

  bool       sendCommand(uint command)
               {SendMessage(getMainFrame()->GetSafeHwnd(), WM_COMMAND, MAKEWPARAM(command, 0), 0);}
private:

  HANDLE     getDefaultPrinter();

  CMainFrm* getMainFrame() {return (CMainFrm*) m_pMainWnd;}

  CApp() : WinAppEx(this), doc(0), view(0) { }
  };



extern CApp* theCApp;

class CDoc;
class ScrollView;

inline CDoc*       cDoc()  {return (CDoc*)     theCApp->getDoc();}
inline ScrollView* cView() {return (ScrollView*) theCApp->getView();}



/////////-----------------
//  void       savePrinterAttr() {printer.saveAttr(getDevMode());}
//  String&    getPrinterName()  {return printer.getName(getDevMode());}


