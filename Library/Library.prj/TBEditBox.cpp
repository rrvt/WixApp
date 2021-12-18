// ToolBar Edit Box


#include "stdafx.h"
#include "TBEditBox.h"
#include "TBBtnCtx.h"


TBEditBox::TBEditBox(uint id) : CMFCToolBarEditBoxButton(id, -1) { }


void TBEditBox::install(TBBtnCtx& ctx)
      {SetFlatMode(false);   SetStyle(ES_AUTOHSCROLL);    m_bImage = false;   m_iWidth = ctx.getHoriz();}



TBEditBox* TBEditBox::get(int id) {try {return (TBEditBox*) GetByCmd((uint)id);}  catch (...) {return 0;}}

