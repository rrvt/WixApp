// Subclass of CFrameWndEx


#include "pch.h"
#include "CMainFrm.h"



void CMainFrm::dspTitle() {
String s = appName;

  if (!appName.isEmpty())  s += _T(" -- ");
  if (!viewName.isEmpty()) s += viewName;

  if (!s.isEmpty()) SetWindowText(s);
  }




bool CMainFrm::addAboutToSysMenu(uint idm, uint ids) {
CMenu*  sysMenu;
Cstring cs;

  if ((idm & 0xFFF0) != idm) return false;
  if (idm >= 0xF000)         return false;

  sysMenu = GetSystemMenu(FALSE);

  if (!sysMenu || !cs.loadRes(ids) || cs.isEmpty()) return false;

  sysAboutID = idm;

  sysMenu->AppendMenu(MF_SEPARATOR);
  sysMenu->AppendMenu(MF_STRING, sysAboutID, cs);   return true;
  }

