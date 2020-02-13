// Button that integrates with a Tool Bar (see ToolBar.h)
// To process the selection from combo box use the following declaration in xxxDoc.h:
//
//    protected:
//      DECLARE_MESSAGE_MAP()
//      afx_msg void button();
//
// The function is located in xxxDoc.cpp and is defined by:
//
//    void xxxDoc::OnCloseCombo() {}
//
// The Message is caught by the following in xxxDoc.cpp file:
//
//    BEGIN_MESSAGE_MAP(xxxDoc, CDocument)
//      ON_BN_CLICKED(ID_Button,  &xxxDoc::button)
//    END_MESSAGE_MAP()
//
// Note, the message processing may be done whereever message maps are found.


#pragma once
#include "ToolBar.h"


class Button : public CButton {

public:

ToolBar& toolBar;

  Button(ToolBar& tb) : toolBar(tb) { }

  bool create(int toolBarItemID, int itemID, int width);
  };

