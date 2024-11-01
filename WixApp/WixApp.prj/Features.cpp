// Manage Component Groups


#include "pch.h"
#include "Features.h"
#include "DefaultPath.h"
#include "Product.h"
#include "Solution.h"
#include "WixUtilities.h"


static TCchar* Section      = _T("FeatureList");
static TCchar* NoKey        = _T("NoFeatures");
static TCchar* FeatureKey   = _T("Feature%02i");
static TCchar* CurSelKey    = _T("CurSel");

       TCchar* FeatureExt   = _T("ftr");

static TCchar* FtrsSection  = _T("Features");
static TCchar* FtrsCountKey = _T("Count");
static TCchar* FtrPrefix    = _T("Ftr%02i");
static TCchar* FeatureID    = _T("FeatureID");

static TCchar* NilFtr       = _T("Feature");


Features features;


void Features::readWixData() {
double ver = solution.getVer();

  if (ver >= 2.0) readWixData2();
  else            readWixData1();
  }



void Features::readWixData2() {
String   curSel;
int      i;
String   section;
String   id;
String   key;
String   s;
Feature* ftr;
Feature* saveCur = 0;
int      n;

  n = wxd.readInt(FtrsSection, FtrsCountKey, 99);
  wxd.readString(FtrsSection, CurSelKey, curSel);

  for (i = 0; i < n; i++) {

    section.format(FtrPrefix, i);

    if (!wxd.readString(section, FeatureID, id) || id[0] == _T('<')) continue;

    ftr = store.add(id);

    ftr->readWixData2(section);
    }

  store.defaultCurID(curSel);
  }



void Features::readWixData1() {
String   curSel;
int      i;
String   key;
String   s;
  Feature* ftr;
Feature* saveCur = 0;
int      n;

  readOne(CurSelKey, curSel);     n = wxd.readInt(Section, NoKey, 99);

  for (i = 0; i < n; i++) {

    key.format(FeatureKey, i);   readOne(key, s);   if (s.isEmpty()) continue;

    ftr = store.add(s);    ftr->wixID = getWixID(ftr->id, FeatureExt);

    ftr->readWixData();
    }

  for (i = 0; i < nFeatures(); i++)
                    {ftr = &store.data[i];   if (ftr->id == curSel) {store.curID = curSel; break;}}
  }


void Features::readOne(TCchar* key, String& v) {
  v = _T("<>");   if (!wxd.readString(Section, key, v) || v == _T("<>")) v.clear();
  }


void Features::writeWixData() {
Iter iter(store);
int        i;
String     prefix;
Feature*   ftr;
int        nToWrite;

  for (nToWrite = 0, ftr = iter(); ftr; ftr = iter++) if (ftr->isValid()) nToWrite++;

  wxd.writeInt(FtrsSection, FtrsCountKey, nToWrite);
  if (!store.curID.isEmpty()) wxd.writeString(FtrsSection, CurSelKey,    store.curID);

  for (i = 0, ftr = iter(); ftr; ftr = iter++) {

    if (!ftr->isValid()) continue;

    prefix.format(FtrPrefix, i++);   ftr->writeWixData2(prefix);
    }
  }


void Features::writeCurSel() {
Feature*   ftr = store.curData();
Iter iter(store);

  if (ftr && ftr->save && !ftr->isUninstall && !ftr->id.isEmpty()) {writeCurID(ftr->id); return;}

  for (ftr = iter(); ftr; ftr = iter++) {
    if (ftr && ftr->save && !ftr->isUninstall && !ftr->id.isEmpty()) {writeCurID(ftr->id); return;}
    }
  }


void Features::writeCurID(String& id) {wxd.writeString(Section, CurSelKey, id);}


void Features::loadCB(WixAppDlg& dialog)
  {Feature* ftr = store.curData();   store.loadCB(dialog.featureCB);   if (ftr) ftr->load(dialog);}


void Features::reloadCB(WixAppDlg& dialog) {
Feature* ftr = store.curData();

  if (ftr) {
    defaultPath.del(ftr->wixID);  ftr->delAllComponents(dialog);  store.delItem(dialog.featureCB);
    }

  ftr = store.curData();

  store.loadCB(dialog.featureCB);   if (ftr) ftr->load(dialog);
  }


void Features::storeCB(WixAppDlg& dialog) {
Feature* ftr    = store.storeCB(dialog.featureCB);   if (!ftr) return;
Feature* curFtr = store.curData();

  ftr->wixID = getWixID(ftr->id, FeatureExt);

  if (curFtr) curFtr->storeComponent(dialog);
  }


void Features::setDefaults(WixAppDlg& dialog) {
String   productName = product.productName;

  dialog.featureCB.SetWindowText(productName);
  dialog.componentCB.SetWindowText(productName);
  storeFeatureData(dialog);
  }


void Features::storeFeatureData(WixAppDlg& dialog) {
Feature* ftr = store.curData();
String   id;

  dialog.featureCB.getWindowText(id);

  if (id.isEmpty()) {store.delNil(NilFtr);  return;}

  if (ftr && (ftr->id.isEmpty() || ftr->id != id)) store.curID = ftr->id = id;

  else if (!ftr) ftr = store.add(id);

  store.add(id, dialog.featureCB);    ftr->store(dialog);
  }


void Features::changeFeature(WixAppDlg& dialog) {
String   s;
Feature* ftr;

  dialog.featureCB.getCurSel(s);   ftr = store.findItem(s);

  if (ftr) ftr->load(dialog);                   //defaultPath.setKey(ftr->wixID);}
  }


void Features::newFeature(WixAppDlg& dialog) {
Feature* ftr = newItem();

  dialog.featureCB.setCurSel(ftr->id);

  ftr->loadNew(dialog);
  }


Feature* Features::newItem()           {Feature* ftr = store.addNil(NilFtr); ftr->newItem(); return ftr;}
Feature* Features::newItem(TCchar* id) {Feature* ftr = store.add(id);        ftr->newItem(); return ftr;}


Component* Features::findAnApp() {
Iter iter(store);
Feature*   ftr;
Component* c;

  for (ftr = iter(); ftr; ftr = iter++) {c = ftr->findAnApp(); if (c) return c;}

  return 0;
  }


bool Features::outputSetPaths(int tab) {
Iter     iter(store);
Feature* ftr;
bool     crlfOut = false;
bool     rslt = false;

  for (ftr = iter(); ftr; ftr = iter++) rslt |= ftr->outputSetPath(tab, crlfOut);

  return rslt;
  }



void Features::outputFeatures(int tab) {
Iter     iter(store);
Feature* ftr;
String   line;

  wix(tab, _T("<Feature Id=\"ProductFeatures\" Title=\"Main Product\" Level=\"1\" >"));

  tab += TabVal;

  for (ftr = iter(); ftr; ftr = iter++)
                                {wix(tab, _T("<ComponentGroupRef Id=\""), ftr->wixID, _T("\"/>"));}
  tab -= TabVal;
  wix(tab, _T("</Feature>"));
  }


void Features::outputFeatureTables(int tab) {
Iter     iter(store);
Feature* ftr;
String   line;

  wix(tab, _T("<Fragment>"));

  tab += TabVal;

  for (ftr = iter(); ftr; ftr = iter++) {

    wix(tab, _T("<ComponentGroup Id=\""), ftr->wixID, _T("\">"));

    tab += TabVal;

    ftr->outputRefs(tab);

    tab -= TabVal;

    wix(tab, _T("</ComponentGroup>"));
    }

  tab -= TabVal;

  wix(tab, _T("</Fragment>"));
  }


void Features::markIconsUsed() {
Iter iter(store);
Feature*   ftr;

  for (ftr = iter(); ftr; ftr = iter++) {
    ftr->markIconsUsed();
    }
  }


void Features::markDirs() {
Iter     iter(store);
Feature* ftr;

  for (ftr = iter(); ftr; ftr = iter++) ftr->markDirs();
  }


bool Features::validate(bool rptErrors) {
Iter     iter(store);
Feature* ftr;
bool     rslt = true;

  for (ftr = iter(); ftr; ftr = iter++) if (!ftr->validate(rptErrors)) rslt &= false;

  return rslt;
  }


void Features::outputComponents(int tab) {
Iter     iter(store);
Feature* ftr;

  for (ftr = iter(); ftr; ftr = iter++) {wix.crlf(); ftr->output(tab);}
  }


void Features::saveData(Archive& ar) {
Iter     iter(store);
Feature* ftr;

  ar << _T("Features") << aCrlf;

  for (ftr = iter(); ftr; ftr = iter++) ftr->saveData(ar);
  }


