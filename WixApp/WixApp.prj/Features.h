// Manage Component Groups


#pragma once
#include "Feature.h"


extern TCchar* FeatureExt;


class Features {
typedef EntityStore <Feature, 1> Store;
typedef EStoreIter  <Feature, 1> Iter;

Store store;

public:

  Features() { }
  Features(Features& ftr) {copy(ftr);}
 ~Features() { }

  Features&  operator= (Features& ftr) {copy(ftr); return *this;}

  void       readWixData();
  void       readWixData1();
  void       readWixData2();

  void       writeWixData();

  void       loadCB(  WixAppDlg& dialog);
  void       reloadCB(WixAppDlg& dialog);
  void       storeCB( WixAppDlg& dialog);

  Component* getCurComponent() {Feature* ftr = getCurFeature();   return ftr->getCur();}

  void        browseCurComponent(WixAppDlg& dlg)
                   {Feature* ftr = getCurFeature();   ftr->getCur()->browsePath(ftr->wixID, dlg);}

  int        nFeatures() {return store.data.end();}

  Feature*   getCurFeature() {if (!nFeatures()) oneFeatureAvail(); return store.curData();}
  Feature*   find(String& id) {return store.findItem(id);}

  Feature*   newItem();
  Feature*   newItem(TCchar* id);

  void       setDefaults(WixAppDlg& dialog);

  void       storeFeatureData(WixAppDlg& dialog);
  void       changeFeature(WixAppDlg& dialog);
  void       newFeature(   WixAppDlg& dialog);

  Component* findAnApp();

  void       markDirs();
//  void       markIconsUsed();

//  bool       outputSetPaths(int tab);

  void       outputFeatures(int tab);

  void       outputFeatureTables(int tab);

  bool       validate(bool rptErrors = true);
  void       outputComponents(int tab);

  void       saveData(Archive& ar);

private:

  void       oneFeatureAvail() {if (!nFeatures()) store.add(String(""));}

  void       readOne(TCchar* key, String& v);
  void       writeCurSel();
  void       writeCurID(String& id);

//  void       delFeature(   WixAppDlg& dialog);

  void       copy(Features& ftr) {store = ftr.store;}
  };


extern Features features;
