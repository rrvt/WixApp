// ToolBar Drop Down Menu


#include "pch.h"
#include "TBMenuButton.h"


TBMenuButton::TBMenuButton(uint id) : CMFCToolBarMenuButton(id, 0, 0) { }


bool TBMenuButton::install(uint idr, TCchar* caption) {
CMenu menu;  if (!menu.LoadMenu(idr)) return false;

  Initialize(m_nID, menu.GetSafeHmenu(), -1, caption);   m_bText = true;   m_bImage = false;

  return true;
  }


bool TBMenuButton::install(uint idr, uint imageIndex) {
CMenu menu;  if (!menu.LoadMenu(idr)) return false;

  Initialize(m_nID, menu.GetSafeHmenu(), -1);   SetImage(imageIndex);   return true;
  }

