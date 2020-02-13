// Manage Component Groups


#include "stdafx.h"
#include "Feature.h"
#include "WixUtilities.h"


static TCchar* Section    = _T("FeatureList");
static TCchar* NoKey      = _T("NoFeatures");
static TCchar* GroupKey   = _T("Feature%02i");
static TCchar* CurSelKey  = _T("CurSel");

       TCchar* FeatureExt = _T("ftr");


Feature feature;


void Feature::readWixData() {
String curSel;
int    i;
String key;
String s;
Group* grp;
Group* saveCur = 0;
int    n;

  readOne(CurSelKey, curSel);     n = wxd.readInt(Section, NoKey, 99);

  for (i = 0; i < n; i++) {

    key.format(GroupKey, i);   readOne(key, s);   if (s.empty()) continue;

    grp = groups.add(s);    grp->wixID = getWixID(grp->id, FeatureExt);

    grp->readWixData();
    }

  for (i = 0; i < groups.nData; i++)
                         {grp = &groups.data[i];   if (grp->id == curSel) {groups.curData = grp; break;}}
  }



void Feature::storeCB(WixDataDlg& dialog) {
Group* grp = groups.storeCB(dialog.groupCB);

  grp->wixID = getWixID(grp->id, FeatureExt);

  if (groups.curData) groups.curData->storeCB(dialog);
  }



void Feature::readOne(TCchar* key, String& v) {
  v = _T("<>");   if (!wxd.readString(Section, key, v) || v == _T("<>")) v.clear();
  }


void Feature::writeWixData() {
GroupsIter iter(groups);
int        i;
String     key;
Group*     grp;
int        nToWrite;

  writeCurSel();

  for (nToWrite = 0, grp = iter.startLoop(); grp; grp = iter.next())
                            if (grp->save && !grp->isUninstall && !grp->id.empty()) nToWrite++;

  wxd.writeInt(Section, NoKey, nToWrite);

  for (i = 0, grp = iter.startLoop(); grp; grp = iter.next()) {

    if (!grp->save || grp->isUninstall || grp->id.empty()) continue;

    key.format(GroupKey, i++);   wxd.writeString(Section, key, grp->id);   grp->writeWixData();
    }
  }


void Feature::writeCurSel() {
Group*     grp = groups.curData;
GroupsIter iter(groups);

  if (grp && grp->save && !grp->isUninstall && !grp->id.empty()) writeCurID(grp->id);

  for (grp = iter.startLoop(); grp; grp = iter.next()) {
    if (grp && grp->save && !grp->isUninstall && !grp->id.empty()) {writeCurID(grp->id); return;}
    }
  }


void Feature::writeCurID(String& id) {wxd.writeString(Section, CurSelKey, id);}


void Feature::updateID(WixDataDlg& dialog) {
Group* grp = groups.curData;    if (!grp) grp = groups.newItem();

  grp->updateID(dialog); groups.loadCB(dialog.groupCB);
  }


void Feature::changeGroup(WixDataDlg& dialog) {
String     s;
Group* grp;

  dialog.groupCB.getCurSel(s);   grp = groups.find(s);   if (grp) grp->loadCB(dialog);
  }


Group* Feature::newGroup() {
Group* grp = groups.curData;

  if (!grp || !grp->id.empty()) grp = groups.newItem();   grp->newItem(); return grp;
  }


void Feature::delGroup(WixDataDlg& dialog) {
Group* grp = groups.curData;

  if (!grp) return;

  grp->delAllComponents(dialog);

  groups.delItem(dialog.groupCB);
  }


Component* Feature::findAnApp() {
GroupsIter iter(groups);
Group*  grp;
Component*   c;

  for (grp = iter.startLoop(); grp; grp = iter.next()) {c = grp->findAnApp(); if (c) return c;}

  return 0;
  }


void Feature::outputSetPath(int tab) {
GroupsIter iter(groups);
Group*     grp;
bool       crlfOut = false;

  for (grp = iter.startLoop(); grp; grp = iter.next()) grp->outputSetPath(tab, crlfOut);
  }



void Feature::outputFeatures(int tab) {
GroupsIter iter(groups);
Group*     grp;
String     line;

  wix.crlf();

  wix.lit(tab, _T("<Feature Id=\"ProductFeatures\" Title=\"Main Product\" Level=\"1\" >\n"));

  for (grp = iter.startLoop(); grp; grp = iter.next())
                            wix.out(tab+1, _T("<ComponentGroupRef Id=\""), grp->wixID, _T("\"/>"));

  wix.lit(tab, _T("</Feature>\n"));
  }


void Feature::outputFeatureTables(int tab) {
GroupsIter iter(groups);
Group*  grp;
String       line;

  wix.crlf();
  wix.lit(0, _T("<Fragment>\n"));

  for (grp = iter.startLoop(); grp; grp = iter.next()) {

    wix.out(tab, _T("<ComponentGroup Id=\""), grp->wixID, _T("\">"));

    grp->outputRefs(tab+1);

    wix.lit(tab, _T("</ComponentGroup>\n"));
    }

  wix.lit(0, _T("</Fragment>\n"));
  }


void Feature::identifyIconsUsed() {
GroupsIter iter(groups);
Group*  grp;

  for (grp = iter.startLoop(); grp; grp = iter.next()) {
    grp->identifyIconsUsed();
    }
  }


void Feature::identifyDirectoriesUsed() {
GroupsIter iter(groups);
Group*  grp;

  for (grp = iter.startLoop(); grp; grp = iter.next()) {
    grp->identifyDirectoriesUsed();
    }
  }



bool Feature::validate() {
GroupsIter iter(groups);
Group*     grp;
bool       rslt = true;

  for (grp = iter.startLoop(); grp; grp = iter.next()) if (!grp->validate()) rslt &= false;

  return rslt;
  }


void Feature::outputComponents() {
GroupsIter iter(groups);
Group*  grp;

  for (grp = iter.startLoop(); grp; grp = iter.next()) grp->output();
  }

