// ToolBar Edit Box


#include "stdafx.h"
#include "TBEditBox.h"



TBEditBox* TBEditBox::get(int id) {
  try {return (TBEditBox*) GetByCmd((uint)id);}
  catch (...) {return 0;}
  }

