// My ToolBar


#include "pch.h"
#include "MyToolBar.h"
#include "Resource.h"


static int NoOfButtonControls = 4;
static int NoOfButtons        = 7;


MyToolBar::MyToolBar() : button(ID_NewProject), button1(ID_OpenProject), //button2(ID_SaveAllFiles),
                         button3(ID_SaveWxdFile), saveMenu(createCboBox(ID_SaveMenu))
                                          {toolBarDim.initialize(NoOfButtonControls, NoOfButtons);}


bool MyToolBar::addButton( uint id, TCchar* caption) {

  if (id == button.getId())  return add(button,   id, caption);
  if (id == button1.getId()) return add(button1,  id, caption);
  if (id == button3.getId()) return add(button3,  id, caption);
  return false;
  }


bool MyToolBar::addEditBox(uint id, int noChars)              {
//  if (id == editBox.getId()) return add(editBox, id, noChars);

  return false;
  }


CString MyToolBar::getText(uint id) {

//  if (id == editBox.getId()) {return ToolBarBase::getText(editBox);}

  return _T("");
  }


#ifdef DocView

bool MyToolBar::addMenu(uint id, int idr, TCchar* caption) {

  if      (id == menu.getId())      return add(menu,     id, idr, caption);
  else if (id == menu1.getId())     return add(menu1,    id, idr, caption);
  else if (id == saveMenu.getId())  return add(saveMenu, id, idr, caption);

  return false;
  }


bool MyToolBar::addMenu(uint id, CCbxItem cbxItem[], int n, TCchar* caption) {

  if      (id == menu.getId())      return add(menu,     id, cbxItem, n, caption);
  else if (id == menu1.getId())     return add(menu1,    id, cbxItem, n, caption);
  else if (id == saveMenu.getId())  return add(saveMenu, id, cbxItem, n, caption);

  return false;
  }


bool MyToolBar::addMenu(uint id, int idr, int index) {
int menuID  = menu.getId();
int menu1ID = menu1.getId();
int saveID  = saveMenu.getId();

  if (id == menuID)  return add(menu,     id, idr, index);
  if (id == menu1ID) return add(menu1,    id, idr, index);
  if (id == saveID)  return add(saveMenu, id, idr, index);

  return false;
  }

#endif


void MyToolBar::setCboCaption(uint id, TCchar* txt) {
  if      (id == saveMenu.getId())  ToolBarBase::setCboCaption(saveMenu,  txt);
#if 0
  else if (id == cboBox1.getId()) ToolBarBase::setCboCaption(cboBox1, txt);
#ifndef DocView
  else if (id == cboMenu.getId())  ToolBarBase::setCboCaption(cboMenu,  txt);
  else if (id == cboMenu1.getId()) ToolBarBase::setCboCaption(cboMenu1, txt);
#endif
#endif
  }


void MyToolBar::setImage(uint id, int toolBarIndex) {
  if (id == saveMenu.getId())  ToolBarBase::setImage(saveMenu, toolBarIndex);
  }



void MyToolBar::setCboWthPrct(uint id, int percent) {
  if      (id == saveMenu.getId())  ToolBarBase::setCboWthPrct(saveMenu,  percent);
#if 0
  else if (id == cboBox1.getId()) ToolBarBase::setCboWthPrct(cboBox1, percent);
#ifndef DocView
  else if (id == cboMenu.getId())  ToolBarBase::setCboWthPrct(cboMenu,  percent);
  else if (id == cboMenu1.getId()) ToolBarBase::setCboWthPrct(cboMenu1, percent);
#endif
#endif
  }


void MyToolBar::setCboHeight(uint id) {
#if 0
  if      (id == cboBox.getId())  ToolBarBase::setCboHeight(cboBox);
  else if (id == cboBox1.getId()) ToolBarBase::setCboHeight(cboBox1);
#ifndef DocView
  else if (id == cboMenu.getId())  ToolBarBase::setCboHeight(cboMenu);
  else if (id == cboMenu1.getId()) ToolBarBase::setCboHeight(cboMenu1);
#endif
#endif
  }


void MyToolBar::dispatch(uint id) {
  if      (id == saveMenu.getId())    ToolBarBase::dispatch(saveMenu);
#if 0
  else if (id == cboBox1.getId())   ToolBarBase::dispatch(cboBox1);
#ifndef DocView
  else if (id == cboMenu.getId())  ToolBarBase::dispatch(cboMenu);
  else if (id == cboMenu1.getId()) ToolBarBase::dispatch(cboMenu1);
#endif
#endif
  }


// Manipulate tuple in the ComboBox

bool MyToolBar::addCboItem(uint id, TCchar* txt, ulongP data) {
#if 0
  if      (id == cboBox.getId())  {return ToolBarBase::addCboItem(cboBox,  txt, data);}
  else if (id == cboBox1.getId()) {return ToolBarBase::addCboItem(cboBox1, txt, data);}
#endif
  return false;
  }


bool MyToolBar::addCboItemSrtd(uint id, TCchar* txt, ulongP data) {
#if 0
  if      (id == cboBox.getId())  {return ToolBarBase::addCboItemSrtd(cboBox,  txt, data);}
  else if (id == cboBox1.getId()) {return ToolBarBase::addCboItemSrtd(cboBox1, txt, data);}
#endif
  return false;
  }


bool MyToolBar::setCurSel(uint id, int index) {

//  if (id == cboBox1.getId()) return ToolBarBase::setCurSel(cboBox1, index);

  return false;
  }


bool MyToolBar::setCurSel(uint id, TCchar* tc) {
//  if (id == cboBox1.getId()) return ToolBarBase::setCurSel(cboBox1, tc);

  return false;
  }


int  MyToolBar::getCurSel(uint id) {
//  if (id == cboBox1.getId()) return ToolBarBase::getCurSel(cboBox1);

  return -1;
  }


bool MyToolBar::getCurSel(uint id, String& s, ulongP& data) {
#if 0
  if (id == cboBox1.getId()) return ToolBarBase::getCurSel(cboBox1, s, data);
  if (id == cboBox.getId())  return ToolBarBase::getCurSel(cboBox,  s, data);
#endif
  return false;
  }


ulongP MyToolBar::getData(uint id, int index) {
//  if (id == cboBox1.getId()) return ToolBarBase::getData(cboBox1, index);

  return 0;
  }


int MyToolBar::find(uint id, TCchar* tc) {
//  if (id == cboBox1.getId()) return ToolBarBase::find(cboBox1, tc);

  return -1;
  }



