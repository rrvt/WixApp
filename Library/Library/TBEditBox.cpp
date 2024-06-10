// ToolBar Edit Box


#include "pch.h"
#include "TBEditBox.h"
//#include "TBBtnCtx.h"
#include "ToolBarBase.h"
#include "ToolBarDim.h"


TBEditBox::TBEditBox(uint id) : CMFCToolBarEditBoxButton(id, -1), id(id), maxChars(0) { }

#if 0
void TBEditBox::install(TBBtnCtx& ctx)
      {SetFlatMode(false);   SetStyle(ES_AUTOHSCROLL);    m_bImage = false;   m_iWidth = ctx.getHoriz();}
#endif

void TBEditBox::install(int noChars) {

  SetFlatMode(false);   SetStyle(ES_AUTOHSCROLL);    m_bImage = false;

  m_iWidth = toolBarDim.getHoriz(noChars);
  }



TBEditBox* TBEditBox::get(int id) {try {return (TBEditBox*) GetByCmd((uint)id);}  catch (...) {return 0;}}

