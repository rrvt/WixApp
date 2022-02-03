// Subclass of CFrameWndEx


#pragma once


class CMainFrm : public CFrameWndEx {

String appName;
String viewName;

protected:

uint   sysAboutID;

public:

       CMainFrm() : sysAboutID(0) { }
      ~CMainFrm() { }

  void setAppName(TCchar* name)      {appName  = name;      dspTitle();}
  void setTitle(  TCchar* rightPart) {viewName = rightPart; dspTitle();}

protected:

  bool addAboutToSysMenu(uint idm, uint ids);

private:

  void dspTitle();
  };

