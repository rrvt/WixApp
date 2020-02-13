// Button that integrates with a Tool Bar


#include "stdafx.h"
#include "Button.h"


bool Button::create(int toolBarItemID, int itemID, int width) {
ulong style = BS_PUSHBUTTON | BS_CENTER | BS_VCENTER | BS_TEXT | WS_VISIBLE | WS_TABSTOP | WS_CHILD;
CRect rect;
int   index;

  if (::IsWindow(m_hWnd)) return false;

  index = toolBar.CommandToIndex(toolBarItemID);    ASSERT(index >= 0);

  toolBar.SetButtonInfo(index, itemID, TBBS_SEPARATOR, width);   toolBar.GetItemRect(index, &rect);

  if (!Create(_T("Button"), style, rect, &toolBar, itemID))
                                        {TRACE0("Failed to create combobox in ToolBar\n"); return false;}
  SetFont(&toolBar.font); return true;                // update the combo with a font.
  }

