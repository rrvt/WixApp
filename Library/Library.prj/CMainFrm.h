// Subclass of CFrameWndEx


#pragma once


class CMainFrm : public CFrameWndEx {

String appName;
String viewName;

public:

  CMainFrm() { }
 ~CMainFrm() { }


  void setAppName(TCchar* name)      {appName  = name;      dspTitle();}
  void setTitle(  TCchar* rightPart) {viewName = rightPart; dspTitle();}

private:

  void dspTitle();
  };

