// ToolBar Drop Down Menu


#include "pch.h"
#include "TBMenu.h"


TBMenu* TBMenu::install(uint idr, TCchar* caption) {
CMenu menu;  if (!menu.LoadMenu(idr)) return 0;
int   n;
int   i;
int   id;

  for (i = 0, n = menu.GetMenuItemCount(); i < n; i++) {
    id = menu.GetMenuItemID(i);   menu.EnableMenuItem(id, MF_ENABLED);
    }

  Initialize(m_nID, menu.GetSafeHmenu(), -1, caption);   m_bText = true;   m_bImage = false;

  return this;
  }


TBMenu* TBMenu::install(uint idr, uint imageIndex) {
CMenu menu;  if (!menu.LoadMenu(idr)) return 0;

  Initialize(m_nID, menu.GetSafeHmenu(), -1);   SetImage(imageIndex);   return this;
  }



TBMenu* TBMenu::install(const CbxItem cbxItem[], int n, TCchar* caption) {
  return 0;
  }


TBMenu* TBMenu::install(const CbxItem cbxItem[], int n, uint imageIndex) {
  return 0;
  }


