// Tool Bar to be used with Button and ComboBox


#pragma once
#include "ComboBox.h"
#include "ExpandableP.h"
#include "IterT.h"
#include "TBButton.h"


enum CtrlType {NilCtrl, RCBtn, BtnCtrl, ComboBoxCtrl, EditBoxCtrl, MenuCtrl, ListCtrl};
class ToolBar;


class CtrlInfo {
public:
CtrlType  ctrlType;
int       id;
String    caption;
int       width;
int       depth;                        // or height down
ulong     style;
HMENU     hMenu;

CRect     rect;
bool      deleted;

  CtrlInfo() : ctrlType(NilCtrl), id(-1), width(0), depth(0), style(0), hMenu(0), deleted(false) { }
  CtrlInfo(CtrlInfo& ci) {copy(ci);}
 ~CtrlInfo() {ctrlType = NilCtrl; id = -1; hMenu = 0;}

  void init(CtrlType typ, int cmdID, int wdth, int dpth, ulong styl);
  void setCaption(TCchar* cptn) {caption = cptn;}
  void setMenu(HMENU menu)      {hMenu   = menu;}
  bool install(ToolBar& tb);

  CtrlInfo& operator= (CtrlInfo& ci) {copy(ci); return *this;}

  bool      operator== (CtrlInfo& ci) {return id == ci.id;}
  bool      operator>= (CtrlInfo& ci) {return id >=  ci.id;}

private:

  bool installBtn(ToolBar& tb);
  bool installCbx(ToolBar& tb);
  bool installEbx(ToolBar& tb);
  bool installMnu(ToolBar& tb);

  void copy(CtrlInfo& ci);
  };


typedef RcdPtrT<CtrlInfo> CtrlInfoP;
class ToolBar;
typedef IterT<ToolBar, CtrlInfo> TBIter;


class ToolBar : public CMFCToolBar {

ExpandableP<CtrlInfo, CtrlInfoP, 2> data;

public:

int   endPos;
int   index;
CFont font;

  ToolBar() : endPos(0), index(0) {getFont();}
 ~ToolBar() { }

  void getFont();

  void setBtnCtrl(int id, TCchar* cptn, int width);
  void setCbxCtrl(int id, int width, int depth, ulong style = 0);
  void setEbxCtrl(int id, int width);
  void setMnuCtrl(int id, HMENU hMenu, TCchar* cptn);

  bool install();


//  bool createCB(ComboBox& cb, int toolBarItemID, int itemID,
//              int width = 150, int dropHt = 100, ulong style = CBS_DROPDOWN | CBS_SORT | WS_VSCROLL);


//  bool createBtn(Button& bt, int toolBarItemID, int itemID, int width);

private:

  CtrlInfo* find(int index);

  // returns either a pointer to data (or datum) at index i in array or zero

  CtrlInfo* datum(int i) {return 0 <= i && i < nData() ? data[i].p : 0;}       // or data[i].p

  int   nData()      {return data.end();}                       // returns number of data items in array

  void  removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

  friend typename TBIter;
  };

