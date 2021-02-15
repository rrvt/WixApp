// Toolbar Button (i.e. MFC Tool Bar Button)


/*
This article describes how to add a toolbar button that contains a Windows control to a toolbar. In MFC, a toolbar
button must be a CMFCToolBarButton Class-derived class, for example CMFCToolBarComboBoxButton Class,
CMFCToolBarEditBoxButton Class, CMFCDropDownToolbarButton Class, or CMFCToolBarMenuButton Class.
*/


#pragma once


class TBButton : public CMFCToolBarButton {

public:

  TBButton(uint id, int imageID, TCchar* cptn) : CMFCToolBarButton(id, imageID, cptn) { }
 ~TBButton() { }

  TBButton& operator= (TBButton& tbBtn);
  };
