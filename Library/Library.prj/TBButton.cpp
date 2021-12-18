// Toolbar Button (i.e. MFC Tool Bar Button)


#include "stdafx.h"
#include "TBButton.h"


TBButton::TBButton(uint id) : CMFCToolBarButton(id, -1) { }


void TBButton::install(TCchar* caption)
  {m_nStyle = TBBS_BUTTON | TBBS_AUTOSIZE;   m_strText = caption;   m_bText = true;   m_bImage = false;}

