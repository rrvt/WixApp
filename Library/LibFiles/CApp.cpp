// Subclass of CWinAppEx


#include "stdafx.h"
#include "CApp.h"
#include "MessageBox.h"
#include "Resources.h"


CApp::CApp() : doc(0), view(0) {
ResourceData res;

  res.getVersion(version);

  if (res.getAppID(appID)) SetAppID(appID);   else messageBox(_T("Unable to create an appID"));

  EnableHtmlHelp();
  }


CDocument* CApp::getDoc() {

  if (doc) return doc;

  POSITION      pos = GetFirstDocTemplatePosition(); if (!pos) return 0;
  CDocTemplate* t   = GetNextDocTemplate(pos);       if (!t)   return 0;

  pos = t->GetFirstDocPosition();                    if (!pos) return 0;
  doc = t->GetNextDoc(pos);   return doc;
  }


CView* CApp::getView() {
POSITION   pos;

  if (view) return view;

  if (!getDoc()) return 0;

  pos  = doc->GetFirstViewPosition();        if (!pos)  return 0;
  view = doc->GetNextView(pos);   return view;
  }


HANDLE CApp::defDevMode() {
PRINTDLG pd;

  pd.lStructSize = (DWORD) sizeof(PRINTDLG);

  return GetPrinterDeviceDefaults(&pd) ? pd.hDevMode : 0;
  }

