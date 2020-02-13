// Tool Bar to be used with Button and ComboBox


#include "stdafx.h"
#include "ToolBar.h"


void ToolBar::getFont() {
LOGFONT lf;

  font.CreatePointFont(90, _T("Arial"));   font.GetLogFont(&lf);   font.DeleteObject();

  lf.lfWeight = 650;   font.CreateFontIndirect(&lf);
  }



