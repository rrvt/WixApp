// Tool Bar to be used with Button and ComboBox


#include "stdafx.h"
#include "ToolBar.h"
#include "TBComboBox.h"
#include "TBEditBox.h"



void ToolBar::setBtnCtrl(int id, TCchar* cptn, int width) {
CtrlInfo*  ci     = find(id);
WINDOWINFO info;
RECT&      client = info.rcClient;

  GetWindowInfo(&info);

  if (!ci) {CtrlInfo ctrlInfo;  ctrlInfo.id = id;  data = ctrlInfo;   ci = find(id);}

  ci->init(BtnCtrl, id, width, client.bottom - client.top, TBBS_BUTTON | TBBS_AUTOSIZE);
  ci->setCaption(cptn);
  }


void ToolBar::setCbxCtrl(int id, int width, int depth, ulong style) {
CtrlInfo* ci = find(id);

  if (!ci) {CtrlInfo ctrlInfo;  ctrlInfo.id = id;  data = ctrlInfo;   ci = find(id);}

  style |= CBS_DROPDOWNLIST;

  ci->init(ComboBoxCtrl, id, width, depth, style);
  }


void ToolBar::setEbxCtrl(int id, int width) {
CtrlInfo* ci = find(id);

  if (!ci) {CtrlInfo ctrlInfo;  ctrlInfo.id = id;  data = ctrlInfo;   ci = find(id);}

  ci->init(EditBoxCtrl, id, width, 0, 0);
  }


void ToolBar::setMnuCtrl(int id, HMENU hMenu, TCchar* cptn) {
CtrlInfo* ci    = find(id);

  if (!ci) {CtrlInfo ctrlInfo;  ctrlInfo.id = id;  data = ctrlInfo;   ci = find(id);}

  ci->init(MenuCtrl, id, 0, 0, 0);
  ci->setMenu(hMenu);
  ci->setCaption(cptn);
  }



CtrlInfo* ToolBar::find(int id) {
TBIter    iter(*this);
CtrlInfo* ci;

  for (ci = iter(); ci; ci = iter++) if (ci->id == id) return ci;

  return 0;
  }


void CtrlInfo::init(CtrlType typ, int cmdID, int wdth, int dpth, ulong styl) {  // int indx,
  ctrlType  = typ;
  id        = cmdID;
  width     = wdth;
  depth     = dpth;
  style     = styl;
  }


bool ToolBar::install() {
TBIter    iter(*this);
CtrlInfo* ci;
int       noDeleted = 0;

  for (ci = iter(); ci; ci = iter++) {
    if (!ci->install(*this)) return false;
    }

  return true;
  }


bool CtrlInfo::install(ToolBar& tb) {

  switch (ctrlType) {
    case BtnCtrl      : return installBtn(tb);
    case ComboBoxCtrl : return installCbx(tb);
    case EditBoxCtrl  : return installEbx(tb);
    case MenuCtrl     : return installMnu(tb);
    case ListCtrl     :
    default           : return false;
    }

  return true;
  }


bool CtrlInfo::installBtn(ToolBar& tb) {
int      x = GetCmdMgr()->GetCmdImage(id);
TBButton tbBtn(id, x, caption);

  tbBtn.m_bImage = tbBtn.m_bText = true;

  tbBtn.SetStyle(style);

  return tb.ReplaceButton(id, tbBtn) > 0;
  }


bool CtrlInfo::installCbx(ToolBar& tb) {
TBComboBox cbx(id, 0, style, width);

  cbx.SetDropDownHeight(depth);
  cbx.SetFlatMode(false);

  return tb.ReplaceButton(id, cbx) > 0;
  }


bool CtrlInfo::installEbx(ToolBar& tb) {
TBEditBox ebx(id, -1, ES_AUTOHSCROLL, width);

  ebx.SetFlatMode(false);

  return tb.ReplaceButton(id, ebx) > 0;
  }


bool CtrlInfo::installMnu(ToolBar& tb) {
int                   x = GetCmdMgr()->GetCmdImage(id);
CMFCToolBarMenuButton mnu(id, hMenu, x, caption, true);

  return tb.ReplaceButton(id, mnu) > 0;
  }



void CtrlInfo::copy(CtrlInfo& ci) {
  ctrlType  = ci.ctrlType;
  id        = ci.id;
  caption   = ci.caption;
  width     = ci.width;
  depth     = ci.depth;
  style     = ci.style;
  hMenu     = ci.hMenu;
  rect      = ci.rect;
  deleted   = ci.deleted;
  }



void ToolBar::getFont() {
LOGFONT lf;

  font.CreatePointFont(90, _T("Arial"));   font.GetLogFont(&lf);   font.DeleteObject();

  lf.lfWeight = 650;   font.CreateFontIndirect(&lf);
  }



