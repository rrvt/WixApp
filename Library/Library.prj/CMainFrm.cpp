// Subclass of CFrameWndEx


#include "stdafx.h"
#include "CMainFrm.h"



void CMainFrm::dspTitle() {
String s = appName;

  if (!appName.isEmpty())  s += _T(" -- ");
  if (!viewName.isEmpty()) s += viewName;

  if (!s.isEmpty()) SetWindowText(s);
  }

