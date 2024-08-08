// My ToolBar


#include "pch.h"
#include "MyToolBar.h"
#include "Resource.h"


static int NoOfButtonControls = 4;
static int NoOfButtons        = 7;


#ifdef MyButtonDefs

MyToolBar::MyToolBar() : button(ID_NewProject), button1(ID_OpenProject), button2(ID_SaveAllFiles),
                         button3(ID_SaveWxdFile)
                         {toolBarDim.initialize(NoOfButtonControls, NoOfButtons);}
#else

MyToolBar::MyToolBar() {toolBarDim.initialize(NoOfButtonControls, NoOfButtons);}

#endif


bool MyToolBar::addButton( uint id, TCchar* caption) {
int btnID  = button.getId();
int btn1ID = button1.getId();
int btn2ID = button2.getId();
int btn3ID = button3.getId();

  if (id == btnID)  return add(button,   id, caption);
  if (id == btn1ID) return add(button1,  id, caption);
  if (id == btn2ID) return add(button2,  id, caption);
  if (id == btn3ID) return add(button3,  id, caption);
  return false;
  }


bool MyToolBar::addEditBox(uint id, int noChars)              {
#ifdef ButtonDefs
  return add(editBox, id, noChars);
#endif
  return false;
  }


CString MyToolBar::getText(uint id) {
#ifdef ButtonDefs
  if (id == editBox.getId()) {return ToolBarBase::getText(editBox);}
#endif
  return 0;
  }

#ifdef DocViewTB

bool MyToolBar::addMenu(uint id, int idr, TCchar* caption) {
#ifdef ButtonDefs
int menuID  = menu.getId();
int menu1ID = menu1.getId();
int saveID  = saveMenu.getId();

  if (id == menuID)  return add(menu,     id, idr, caption);
  if (id == menu1ID) return add(menu1,    id, idr, caption);
  if (id == saveID)  return add(saveMenu, id, idr, caption);
#endif
  return false;
  }


bool MyToolBar::addMenu(uint id, int idr, int index) {
#ifdef ButtonDefs
int menuID  = menu.getId();
int menu1ID = menu1.getId();
int saveID  = saveMenu.getId();

  if (id == menuID)  return add(menu,     id, idr, index);
  if (id == menu1ID) return add(menu1,    id, idr, index);
  if (id == saveID)  return add(saveMenu, id, idr, index);
#endif
  return false;
  }


bool MyToolBar::addMenu(uint id, CbxItem cbxItem[], int n, TCchar* caption) {
#ifdef ButtonDefs
  return add(menu1, id, cbxItem, n, caption);
#endif
  return false;
  }

#else


bool MyToolBar::addMenu(uint id, int idr, TCchar* caption) {
#ifdef ButtonDefs
  return add(cbxMenu, id, idr, caption);
#endif
  }


bool MyToolBar::addMenu(uint id, CbxItem cbxItem[], int n, TCchar* caption) {
#ifdef ButtonDefs
  return add(cbxMenu1, id, cbxItem, n, caption);
#endif
  }


void MyToolBar::dispatch(uint id) {
#ifdef ButtonDefs
uint cbxId  = cbxMenu.getId();
uint cbxId1 = cbxMenu1.getId();

  if (id == cbxId)  ToolBarBase::dispatch(cbxMenu);
  if (id == cbxId1) ToolBarBase::dispatch(cbxMenu1);
#endif
  }

#endif




bool MyToolBar::addCBx(uint id) {
#ifdef ButtonDefs
  return add(cboBx,  id, 10);
#endif
  return false;
  }


bool MyToolBar::addCBx(uint id, int idr, TCchar* caption) {
#ifdef ButtonDefs
  return add(cboBx,  id, idr, caption);
#endif
  return false;
  }


bool MyToolBar::addCbxItem(uint id, TCchar* txt, int data) {
#ifdef ButtonDefs
int cbxID = cboBx.getId();

  if (id == cbxID) {return addItem(cboBx, txt, data);}
#endif
  return false;
  }


bool MyToolBar::addCBx(uint id, CbxItem cbxItem[], int n, TCchar* caption) {
#ifdef ButtonDefs
  return add(cboBx1,  id, cbxItem, n, caption);
#endif
  return false;
  }


bool MyToolBar::addCbxItemSorted(uint id, TCchar* txt, int data) {
#ifdef ButtonDefs
int cbxID = cboBx.getId();

  if (id == cbxID) {return addItemSorted(cboBx, txt, data);}
#endif
  return false;
  }


void MyToolBar::setCaption(uint id, TCchar* caption) {
#ifdef ButtonDefs
int cbxID = cboBx.getId();

  if (id == cbxID) ToolBarBase::setCaption(cboBx, caption);
#endif
  }


void MyToolBar::setWidth(uint id) {
#ifdef ButtonDefs
int cbxID = cboBx.getId();

  if (id == cbxID) ToolBarBase::setWidth(cboBx);
#endif
  }


#ifdef DocViewTB

bool MyToolBar::getCurSel( uint id, String& s, int& data) {
#ifdef ButtonDefs
int cbxID = cboBx.getId();

  if (id == cbxID) return ToolBarBase::getCurSel(cboBx, s, data);
#endif
  return false;
  }

#else

bool MyToolBar::getCurSel( uint id, String& s, int& data) {
#ifdef ButtonDefs
int cbxID  = cboBx.getId();
int cbxID1 = cboBx1.getId();

  if (id == cbxID)  return ToolBarBase::getCurSel(cboBx,  s, data);
  if (id == cbxID1) return ToolBarBase::getCurSel(cboBx1, s, data);
#endif
  return false;
  }

#endif

