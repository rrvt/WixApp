// Feature -- One entity of install


#include "pch.h"
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

static TCchar* NilCmp        = _T("NilComponent");


#if 0
void Feature::clear() {
  Data::clear();
  components.clear();           section.clear();
  progFileID.clear();           startMenuID.clear();
  save = isUninstall = false;
  }
#endif

void Feature::copy(Feature& g) {
    Data::copy(g);
    components  = g.components;  section     = g.section;
    progFileID  = g.progFileID;  startMenuID = g.startMenuID;
    save        = g.save;        isUninstall = g.isUninstall;
    }



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

    cmp = components.add(s);    cmp->readWixData(wixID);
    }

  for (i = 0; i < nComponents(); i++)
         {cmp = &components.data[i];   if (cmp->id == curSel) {components.curID = cmp->id; break;}}
  }


void Feature::readOne(TCchar* key, String& v) {
  v = _T("<>");   if (!wxd.readString(section, key, v) || v == _T("<>")) v.clear();
  }


void Feature::storeProgFileName(WixAppDlg& dialog) {
String   s   = getText(dialog.progFileEB);
DirDesc* dsc = pffDirectories.add(s);

  if (!dsc) {progFileID  = _T(""); return;}

  progFileID = dsc->id;  dsc->inUse = true;
  }


void Feature::storeMenuName(WixAppDlg& dialog) {
String   s   = getText(dialog.startMenuEB);
DirDesc* dsc = pmfDirectories.add(s);

  if (!dsc) {startMenuID = _T(""); return;}

  startMenuID = dsc->id;  dsc->inUse = true;
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

    cmp = components.add(cmpID);    cmp->readWixData2(wixID, cmpSection);
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

  for (cmp = iter(), i = 0; cmp; cmp = iter++, i++) {

    cmpSection.format(CmpSection, prefix.str(), i);

    cmp->writeWixData2(cmpSection);
    }
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


void Feature::loadNew(WixAppDlg& dialog) {load(dialog);  dialog.featureCB.setFocus();}


void Feature::load(WixAppDlg& dialog) {

  dialog.progFileEB.SetWindowText(pffDirectories.fullPath(progFileID));
  dialog.startMenuEB.SetWindowText(pmfDirectories.fullPath(startMenuID));

  loadComponent(dialog);
  }


void Feature::loadComponent(WixAppDlg& dialog) {
Component* curCmp = components.curData();

  components.loadCB(dialog.componentCB);

  if (curCmp)  curCmp->load(dialog);
  else {Component c; c.load(dialog);}
  }


void Feature::storeComponent(WixAppDlg& dialog) {
Component* curCmp = components.curData();

  components.storeCB(dialog.componentCB);

  if (curCmp) curCmp->store(dialog);
  }


DirDesc* Feature::getProgFile()  {return pffDirectories.find(progFileID);}
DirDesc* Feature::getStartMenu() {return pmfDirectories.find(startMenuID);}


void Feature::store(WixAppDlg& dialog) {

  wixID = getWixID(id, FeatureExt);

  storeProgFileName(dialog);   storeMenuName(dialog);

  storeComponentData(dialog);
  }


void Feature::delWixData() {
String section;

  section.format(Section, id.str());

  wxd.deleteSection(section);
  }


void Feature::storeComponentData(WixAppDlg& dialog) {
Component* cmp = components.curData();
String     id;

  dialog.componentCB.getWindowText(id);

  if (id.isEmpty()) {

    components.delNil(NilCmp);

    return;
    }

  if (cmp && (cmp->id.isEmpty() || cmp->id != id)) components.curID = cmp->id = id;

  else if (!cmp) cmp = newItem(id);

  cmp->store(dialog);   components.loadCB(dialog.componentCB);
  }


void Feature::changeComponent(WixAppDlg& dialog) {
String     s;
Component* cmp;

  dialog.componentCB.getCurSel(s);   cmp = components.findItem(s);   if (cmp) cmp->load(dialog);
  }


void Feature::newComponent(WixAppDlg& dialog) {newItem()->loadNew(dialog);}


Component* Feature::newItem()           {return components.addNil(NilCmp);}
Component* Feature::newItem(TCchar* id) {return components.add(id);}


void Feature::delAllComponents(WixAppDlg& dialog) {
ComponentsIter iter(components);
Component*     cmp;

  for (cmp = iter(); cmp; cmp = iter++) delComponent(dialog);
  }



void Feature::delComponent(WixAppDlg& dialog) {components.delItem(dialog.componentCB);}


Component* Feature::findAnApp() {
ComponentsIter iter(components);
Component*     c;

  for (c = iter(); c; c = iter++) if (c->isApp && c->isVersionAvail) return c;
  for (c = iter(); c; c = iter++) if (c->isOnPath)                   return c;
  return 0;
  }


void Feature::markIconsUsed() {
ComponentsIter iter(components);
Component*     c;

  for (c = iter(); c; c = iter++) c->identifyIconUsed();
  }


void Feature::markDirs() {
ComponentsIter iter(components);
Component*     c;

  for (c = iter(); c; c = iter++) c->markDirs(*this);

  defaultPath.mark(wixID);
  }


bool Feature::outputSetPath(int tab, bool& crlfOut) {
ComponentsIter iter(components);
Component*     cmp;
bool           rslt = false;

  for (cmp = iter(); cmp; cmp = iter++) rslt |= cmp->outputSetPath(tab, crlfOut);

  return rslt;
  }


bool Feature::validate(bool rptErrors) {
int  i;
bool rslt = true;

  for (i = 0; i < nComponents(); i++) if (!components.data[i].validate(rptErrors)) rslt &= false;

  return rslt;
  }


void Feature::outputRefs(int tab) {
ComponentsIter iter(components);
Component*     cmp;

  for (cmp = iter(); cmp; cmp = iter++)
                                       wix(tab, _T("<ComponentRef Id=\""), cmp->wixID, _T("\"/>"));
  }


void Feature::output(int tab) {
int    i;
String line;


  wix(tab, _T("<Fragment>"));

  tab += TabVal;

  for (i = 0; i < nComponents(); i++) {if (i) wix.crlf(); components.data[i].output(tab);}

  tab -= TabVal;

  wix(tab, _T("</Fragment>"));
  }


void Feature::saveData(Archive& ar) {
String k;
ComponentsIter iter(components);
Component*     cmp;
int            i;

  ar << aClrTabs << aSetTab(3) << aSetTab(30);

  ar << aTab << _T("Feature: ") << id << aCrlf;

  ar << aClrTabs << aSetTab(6) << aSetTab(30);
  ar << aTab << _T("Wix ID:")          << aTab << wixID << aCrlf;
  ar << aTab << _T("Section:")         << aTab << section << aCrlf;
  ar << aTab << _T("Program File ID:") << aTab << progFileID << aCrlf;
  ar << aTab << _T("StartMenuID:")     << aTab << startMenuID << aCrlf;
  ar << aTab << _T("save:")            << aTab << save << aCrlf;
  ar << aTab << _T("Is Uninstall:")    << aTab << isUninstall << aCrlf;

  ar << aTab << _T("Components") << aCrlf;
  for (i = 0, cmp = iter(); cmp; i++, cmp = iter++) {if (i) ar << aCrlf;   cmp->saveData(ar);}
  }

