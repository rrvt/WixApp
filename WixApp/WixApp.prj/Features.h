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
 ~Features() { }

  Features(Features& ftr) {copyOp(ftr);}

  Features& operator= (Features& ftr) {copyOp(ftr); return *this;}

  void       readWixData();
  void       readWixData1();
  void       readWixData2();

  void       writeWixData();
  void       writeWixData1();
  void       writeWixData2();

  void loadCB(WixDataDlg& dialog) {
    Feature* curFtr = store.curData();
    store.loadCB(dialog.featureCB);   if (curFtr) curFtr->load(dialog);
    }

  void       storeCB(WixDataDlg& dialog);

  Component* getCurComponent() {Feature* ftr = getCurFeature();   return ftr->getCur();}

  int        nFeatures() {return store.data.end();}

  Feature*   getCurFeature() {if (!nFeatures()) oneFeatureAvail(); return store.curData();}
  Feature*   find(String& id) {return store.find(id);}

  Feature*   newItem();
  Feature*   newItem(TCchar* id);

  void       setDefaults(WixDataDlg& dialog);

  void       storeFeatureData(WixDataDlg& dialog);
  void       changeFeature(WixDataDlg& dialog);
  void       newFeature(   WixDataDlg& dialog);
  void       delFeature(   WixDataDlg& dialog);

  Component* findAnApp();

  void       markDirs();
  void       markIconsUsed();

  void       outputSetPath(int tab);

  void       outputFeatures(int tab);

  void       outputFeatureTables(int tab);

  bool       validate(bool rptErrors = true);
  void       outputComponents();

private:

  void oneFeatureAvail() {if (!nFeatures()) store.add(String(""));}

  void readOne(TCchar* key, String& v);
  void writeCurSel();
  void writeCurID(String& id);

  void copyOp(Features& ftr) {store = ftr.store;}
  };


extern Features features;
