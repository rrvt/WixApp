// Feature -- One entity of install


#include "stdafx.h"
#include "Feature.h"
#include "DefaultPath.h"
#include "PFFdirectories.h"
#include "PMFdirectories.h"
#include "WixUtilities.h"


static TCchar* Section       = _T("%s.ftr");
static TCchar* NoKey         = _T("NoComponents");
static TCchar* CmpsKey       = _T("Component%02i");
static TCchar* CurSelKey     = _T("CurSel");
static TCchar* FeatureExt    = _T("ftr");
static TCchar* ProgKey       = _T("ProgramID");
static TCchar* StartKey      = _T("StartMenuID");

static TCchar* FeatureID     = _T("FeatureID");
static TCchar* CmpSection    = _T("%sCmp%02i");
static TCchar* IDkey         = _T("ID");


void Feature::readWixData() {
String     curSel;
int        n;
int        i;
String     key;
String     s;
Component* cmp;

  section.format(Section, id.str());

  readOne(CurSelKey, curSel);

  wxd.readString(section, ProgKey,  progFileID);
  wxd.readString(section, StartKey, startMenuID);

  n = wxd.readInt(section, NoKey, 99);

  for (i = 0; i < n; i++) {

    key.format(CmpsKey, i);   readOne(key, s);   if (s.isEmpty()) continue;

    cmp = components.add(s);    cmp->readWixData();
    }

  for (i = 0; i < nComponents(); i++)
              {cmp = &components.data[i];   if (cmp->id == curSel) {components.curID = cmp->id; break;}}
  }


void Feature::readOne(TCchar* key, String& v) {
  v = _T("<>");   if (!wxd.readString(section, key, v) || v == _T("<>")) v.clear();
  }


void Feature::storeProgFileName(WixDataDlg& dialog) {
String   s   = getText(dialog.progFileEB);
DirDesc* dsc = pffDirectories.add(s);

  progFileID  = dsc ? dsc->id : _T("");
  }


void Feature::storeMenuName(WixDataDlg& dialog) {
String   s   = getText(dialog.startMenuEB);
DirDesc* dsc = pmfDirectories.add(s);

  startMenuID = dsc ? dsc->id : _T("");
  }


void Feature::readWixData2( String& section) {
String     curSelID;
int        n;
int        i;
String     cmpSection;
String     cmpID;
Component* cmp;

  wixID = getWixID(id, FeatureExt);
  wxd.readString(section, ProgKey,   progFileID);
  wxd.readString(section, StartKey,  startMenuID);
  wxd.readString(section, CurSelKey, curSelID);

  n = wxd.readInt(section, NoKey, 99);

  for (i = 0; i < n; i++) {

    cmpSection.format(CmpSection, section.str(), i);

    if (!wxd.readString(cmpSection, IDkey, cmpID) || cmpID[0] == _T('<')) continue;

    cmp = components.add(cmpID);    cmp->readWixData2(cmpSection);
    }

  components.defaultCurID(curSelID);
  }



void Feature::writeWixData2(String& prefix) {
ComponentsIter iter(components);
Component*     cmp;
int            i;
String         cmpSection;

  if (id.isEmpty()) return;

  section = prefix;

  wxd.writeString(section, FeatureID, id);
  wxd.writeString(section, ProgKey,   progFileID);
  wxd.writeString(section, StartKey,  startMenuID);

  if (!components.curID.isEmpty()) wxd.writeString(section, CurSelKey, components.curID);

  wxd.writeInt(section, NoKey, nComponents());

  for (cmp = iter.startLoop(), i = 0; cmp; cmp = iter.nextItem(), i++) {

    cmpSection.format(CmpSection, prefix.str(), i);   //wxd.writeString(section, key, cmp->id);

    cmp->writeWixData2(cmpSection);
    }

//  for (; i < n; i++) {key.format(CmpsKey, i);  wxd.deleteString(section, key);}
  }


void Feature::writeWixData() {
int        n;
int        i;
String     key;
Component* cmp;

  if (id.isEmpty()) return;

  section.format(Section, id.str());

  if (!components.curID.isEmpty()) wxd.writeString(section, CurSelKey, components.curID);

  wxd.writeString(section,  ProgKey, progFileID);
  wxd.writeString(section, StartKey, startMenuID);

  n = wxd.readInt(section, NoKey, nComponents());
  wxd.writeInt(section, NoKey, nComponents());

  for (i = 0; i < nComponents(); i++) {

    cmp = &components.data[i];

    key.format(CmpsKey, i);   wxd.writeString(section, key, cmp->id);

    cmp->writeWixData();
    }

  for (; i < n; i++) {key.format(CmpsKey, i);  wxd.deleteString(section, key);}
  }


void Feature::loadNew(WixDataDlg& dialog) {load(dialog);  dialog.featureCB.setFocus();}


void Feature::load(WixDataDlg& dialog) {

  dialog.progFileEB.SetWindowText(pffDirectories.fullPath(progFileID));
  dialog.startMenuEB.SetWindowText(pmfDirectories.fullPath(startMenuID));

  loadComponent(dialog);
  }


void Feature::loadComponent(WixDataDlg& dialog) {
Component* curCmp = components.curData();

  defaultPath.setCurPath(wixID);

  components.loadCB(dialog.componentCB);

  if (curCmp)  curCmp->load(dialog);
  else {Component c; c.load(dialog);}
  }


void Feature::storeComponent(WixDataDlg& dialog) {
Component* curCmp = components.curData();

  components.storeCB(dialog.componentCB);

  if (curCmp) curCmp->store(dialog);
  }


DirDesc* Feature::getProgFile()  {return pffDirectories.find(progFileID);}
DirDesc* Feature::getStartMenu() {return pmfDirectories.find(startMenuID);}


void Feature::store(WixDataDlg& dialog) {

  wixID = getWixID(id, FeatureExt);

  storeProgFileName(dialog);   storeMenuName(dialog);   defaultPath.setCurPath(wixID);

  storeComponentData(dialog);
  }


void Feature::delWixData() {
String section;

  section.format(Section, id.str());

  wxd.deleteSection(section);
  }


void Feature::storeComponentData(WixDataDlg& dialog) {
Component* cmp = components.curData();
String     id;

  dialog.componentCB.getWindowText(id);

  if (id.isEmpty()) {

    ComponentsIter iter(components);

    for (cmp = iter.startLoop(); cmp; cmp = iter.nextItem())
                         if (cmp->id.find(_T("< Component ")) >= 0) {components.delItem(cmp->id); break;}
    return;
    }

  if (cmp && (cmp->id.isEmpty() || cmp->id != id)) components.curID = cmp->id = id;

  else if (!cmp) cmp = newItem(id);

  cmp->store(dialog);   components.loadCB(dialog.componentCB);
  }


void Feature::changeComponent(WixDataDlg& dialog) {
String     s;
Component* cmp;

  dialog.componentCB.getCurSel(s);   cmp = components.find(s);   if (cmp) cmp->load(dialog);
  }


void Feature::newComponent(WixDataDlg& dialog) {newItem()->loadNew(dialog);}


Component* Feature::newItem(TCchar* id) {
Component* cmp = components.curData();
String     s = id && *id ? id : _T("< Component ");

  if (!cmp || cmp->id.find(s) < 0) cmp = components.newItem(s);

  return cmp;
  }


void Feature::delAllComponents(WixDataDlg& dialog) {
ComponentsIter iter(components);
Component*     cmp;

  for (cmp = iter.startLoop(); cmp; cmp = iter.nextItem()) delComponent(dialog);
  }



void Feature::delComponent(WixDataDlg& dialog) {components.delItem(dialog.componentCB);}


Component* Feature::findAnApp() {
ComponentsIter iter(components);
Component*     c;

  for (c = iter.startLoop(); c; c = iter.nextItem()) if (c->isApp && c->isVersionAvail) return c;
  for (c = iter.startLoop(); c; c = iter.nextItem()) if (c->isOnPath)                   return c;
  return 0;
  }


void Feature::markIconsUsed() {
ComponentsIter iter(components);
Component*     c;

  for (c = iter.startLoop(); c; c = iter.nextItem()) c->identifyIconUsed();
  }


void Feature::markDirs() {
ComponentsIter iter(components);
Component*     c;

  for (c = iter.startLoop(); c; c = iter.nextItem()) c->markDirs(*this);

  defaultPath.mark(wixID);
  }


void Feature::markDfltDirs() {
  }


void Feature::outputSetPath(int tab, bool& crlfOut) {
ComponentsIter iter(components);
Component*     cmp;

  for (cmp = iter.startLoop(); cmp; cmp = iter.nextItem()) cmp->outputSetPath(tab, crlfOut);
  }


bool Feature::validate() {
int  i;
bool rslt = true;

  for (i = 0; i < nComponents(); i++) if (!components.data[i].validate()) rslt &= false;

  return rslt;
  }


void Feature::outputRefs(int tab) {
ComponentsIter iter(components);
Component*     cmp;

  for (cmp = iter.startLoop(); cmp; cmp = iter.nextItem())
                                      {wix.out(tab, _T("<ComponentRef Id=\""), cmp->wixID, _T("\"/>"));}
  }


void Feature::output() {
int    i;
String line;

  wix.crlf();

  wix.lit(0, _T("<Fragment>\n"));

  for (i = 0; i < nComponents(); i++) {
    components.data[i].output(1);
    }

  wix.crlf();  wix.lit(0, _T("</Fragment>\n"));

  }

