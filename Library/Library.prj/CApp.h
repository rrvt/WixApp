// Subclass of CWinAppEx


#pragma once
#include "CMainFrm.h"


class CApp : public CWinAppEx {

CDocument* doc;
CView*     view;

public:

String appID;
String version;

  CApp(CApp* app);
 ~CApp() { }

  virtual int ExitInstance();

  // Title becomes:  <app name> -- <title> or just <title> (when setTitle alone is called)

  void       setAppName(  TCchar* appName)   {getMainFrame()->setAppName(appName);}
  void       setTitle(    TCchar* rightPart) {getMainFrame()->setTitle(rightPart);}

  CDocument* getDoc();
  CView*     getView();
  HANDLE     getDevMode() {return m_hDevMode ? m_hDevMode : defDevMode();}
  void       invalidate() {if (getView()) view->Invalidate();}

private:

  HANDLE    defDevMode();

  CMainFrm* getMainFrame() {return (CMainFrm*) m_pMainWnd;}

  CApp() : doc(0), view(0) { }
  };



extern CApp* theCApp;

class CDoc;
class CScrView;

inline CDoc*     cDoc()  {return (CDoc*)     theCApp->getDoc();}
inline CScrView* cView() {return (CScrView*) theCApp->getView();}

