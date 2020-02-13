// Manage Component Groups


#pragma once
#include "Group.h"


extern TCchar* FeatureExt;


class Feature {
typedef EntityStore <Group, 1> Groups;
typedef EStoreIter  <Group, 1> GroupsIter;

Groups groups;

public:

  Feature() { }
 ~Feature() { }

  Feature(Feature& ftr) {copyOp(ftr);}

  Feature& operator= (Feature& ftr) {copyOp(ftr); return *this;}

  void       readWixData();
  void       writeWixData();

  void loadCB(WixDataDlg& dialog)
    {groups.loadCB(dialog.groupCB);  if (groups.curData) groups.curData->loadCB(dialog);}

  void storeCB(WixDataDlg& dialog);

  Component* getCurComponent() {Group* grp = getCurGroup();   return grp->getCur();}
  Group*     getCurGroup() {if (!groups.nData) oneGroupAvail(); return groups.curData;}
  Group*     find(String& id) {return groups.find(id);}
  Group*     newGroup();

  void updateID(   WixDataDlg& dialog);
  void changeGroup(WixDataDlg& dialog);
  void delGroup(WixDataDlg& dialog);

  Component* findAnApp();

  void identifyIconsUsed();
  void identifyDirectoriesUsed();

  void outputSetPath(int tab);

  void outputFeatures(int tab);

  void outputFeatureTables(int tab);

  bool validate();
  void outputComponents();

private:

  void oneGroupAvail() {if (!groups.nData) groups.add(String(""));}

  void readOne(TCchar* key, String& v);
  void writeCurSel();
  void writeCurID(String& id);

  void copyOp(Feature& ftr) {groups = ftr.groups;}
  };


extern Feature feature;
