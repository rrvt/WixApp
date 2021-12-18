// ToolBar Drop Down Menu


#include "stdafx.h"
#include "TBMenuButton.h"


TBMenuButton::TBMenuButton(uint id) : CMFCToolBarMenuButton(id, 0, 0) { }


bool TBMenuButton::install(uint idr, TCchar* caption) {
CMenu menu;  if (!menu.LoadMenu(idr)) return false;

  Initialize(m_nID, menu.GetSafeHmenu(), -1, caption);   m_bText = true;   m_bImage = false;

  return true;
  }


