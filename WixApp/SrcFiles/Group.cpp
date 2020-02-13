// Group -- One entity of install


#include "stdafx.h"
#include "Group.h"
#include "Directory.h"
#include "WixUtilities.h"


static TCchar* Section       = _T("%s.grp");
static TCchar* NoKey         = _T("NoComponents");
static TCchar* CmpsKey       = _T("Component%02i");
static TCchar* CurSelKey     = _T("CurSel");
static TCchar* GroupExt      = _T("grp");



void Group::readWixData() {
String     curSel;
int        n;
int        i;
String     key;
String     s;
Component* cmp;

  section.format(Section, id.str());     readOne(CurSelKey, curSel);

  n = wxd.readInt(section, NoKey, 99);

  for (i = 0; i < n; i++) {

    key.format(CmpsKey, i);   readOne(key, s);   if (s.empty()) continue;

    cmp = components.add(s);
 //   cmp->icon = icons.getNew();
    cmp->readWixData();
    }

  for (i = 0; i < components.nData; i++)
                 {cmp = &components.data[i];   if (cmp->id == curSel) {components.curData = cmp; break;}}
  }


void Group::readOne(TCchar* key, String& v) {
  v = _T("<>");   if (!wxd.readString(section, key, v) || v == _T("<>")) v.clear();
  }


void Group::writeWixData() {
int        n;
int        i;
String     key;
Component* cmp;

  if (id.empty()) return;

  section.format(Section, id.str());

  if (components.curData) wxd.writeString(section, CurSelKey, components.curData->id);

  n = wxd.readInt(section, NoKey, components.nData);
  wxd.writeInt(section, NoKey, components.nData);

  for (i = 0; i < components.nData; i++) {

    cmp = &components.data[i];

    key.format(CmpsKey, i);   wxd.writeString(section, key, cmp->id);

    cmp->writeWixData();
    }

  for (; i < n; i++) {key.format(CmpsKey, i);  wxd.deleteString(section, key);}
  }


void Group::loadCB(WixDataDlg& dialog) {
  components.loadCB(dialog.componentCB);
  if (components.curData) components.curData->load(dialog);
  }


void Group::storeCB(WixDataDlg& dialog) {
  components.storeCB(dialog.componentCB);
  if (components.curData) components.curData->store(dialog);
  }


void Group::updateID(WixDataDlg& dialog)
                                    {dialog.groupCB.getWindowText(id);  wixID = getWixID(id, GroupExt);}


void Group::delWixData() {
String section;

  section.format(Section, id.str());

  wxd.deleteSection(section);
  }


void Group::updateComponent(WixDataDlg& dialog) {
Component* cmp = components.curData;

  if (!cmp) cmp = components.newItem();

  cmp->updateComponent(dialog);   components.loadCB(dialog.componentCB);
  }



void Group::changeComponent(WixDataDlg& dialog) {
String     s;
Component* cmp;

  dialog.componentCB.getCurSel(s);   cmp = components.find(s);   if (cmp) cmp->load(dialog);
  }


void Group::newComponent(WixDataDlg& dialog) {

  newItem()->loadNew(dialog);
  }


Component* Group::newItem() {
Component* cmp = components.curData;

  if (cmp && cmp->id.empty()) return cmp;

  return components.newItem();
  }


void Group::leavingCB(WixDataDlg& dialog) {
  components.leavingCB(dialog.componentCB);
  if (components.curData) components.curData->load(dialog);
  }


void Group::delAllComponents(WixDataDlg& dialog) {
ComponentIter iter(components);
Component*    cmp;

  for (cmp = iter.startLoop(); cmp; cmp = iter.next()) delComponent(dialog);
  }



void Group::delComponent(WixDataDlg& dialog) {components.delItem(dialog.componentCB);}


Component* Group::findAnApp() {
ComponentIter iter(components);
Component*    c;

  for (c = iter.startLoop(); c; c = iter.next()) if (c->isApp && c->isVersionAvail) return c;
  for (c = iter.startLoop(); c; c = iter.next()) if (c->isOnPath) return c;
  return 0;
  }


void Group::identifyIconsUsed() {
ComponentIter iter(components);
Component*    c;

  for (c = iter.startLoop(); c; c = iter.next()) c->identifyIconUsed();
  }


void Group::identifyDirectoriesUsed() {
ComponentIter iter(components);
Component*    c;

  for (c = iter.startLoop(); c; c = iter.next()) c->identifyDirectoriesUsed();
  }


void Group::outputSetPath(int tab, bool& crlfOut) {
ComponentIter iter(components);
Component*    cmp;

  for (cmp = iter.startLoop(); cmp; cmp = iter.next()) cmp->outputSetPath(tab, crlfOut);
  }


bool Group::validate() {
int  i;
bool rslt = true;

  for (i = 0; i < components.nData; i++) if (!components.data[i].validate()) rslt &= false;

  return rslt;
  }


void Group::outputRefs(int tab) {
ComponentIter iter(components);
Component*    cmp;

  for (cmp = iter.startLoop(); cmp; cmp = iter.next())
                                      {wix.out(tab, _T("<ComponentRef Id=\""), cmp->wixID, _T("\"/>"));}
  }


void Group::output() {
int    i;
String line;

  wix.crlf();

  wix.lit(0, _T("<Fragment>\n"));

  for (i = 0; i < components.nData; i++) {
    components.data[i].output(1);
    }

  wix.crlf();  wix.lit(0, _T("</Fragment>\n"));

  }

