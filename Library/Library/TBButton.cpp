// Toolbar Button (i.e. MFC Tool Bar Button)


#include "pch.h"
#include "TBButton.h"


TBButton::TBButton(uint id) : CMFCToolBarButton(id, -1), id(id) { }


void TBButton::install(TCchar* caption) {
  m_nStyle  = BTNS_BUTTON | BS_VCENTER;
  m_strText = caption;
  m_bText   = true;
  m_bImage  = false;
  }

