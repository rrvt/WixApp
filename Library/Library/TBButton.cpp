// Toolbar Button (i.e. MFC Tool Bar Button)


#include "pch.h"
#include "TBButton.h"


void TBButton::install(TCchar* caption) {
  m_nStyle  = BTNS_BUTTON | BS_TOP | BS_CENTER;
  m_strText = caption;
  m_bText   = true;
  m_bImage  = false;
  }

/*
BS_LEFT
BS_RIGHT
BS_CENTER
BS_TOP
BS_BOTTOM
BS_VCENTER
*/
