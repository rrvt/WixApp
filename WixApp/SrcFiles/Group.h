// ComponentGrpList -- One entity of install


#pragma once
#include "Component.h"


class Group : public Data {
typedef EntityStore <Component, 1> Components;
typedef EStoreIter  <Component, 1> ComponentIter;

Components components;
String     section;

public:
bool   save;
bool   isUninstall;

  Group() : save(true), isUninstall(false) {}
  Group(Group& c) {copyObj(c);}
 ~Group() { }

  Group& operator= (Group& c) {copyObj(c); return *this;}

  void delData()
    {id.clear(); components.clear(); section.clear(); wixID.clear(); save = false; isUninstall = false;}

  void       readWixData( );
  void       writeWixData();

  void       loadCB(WixDataDlg& dialog);
  void       storeCB(WixDataDlg& dialog);

  void       updateID(WixDataDlg& dialog);

  void       updateComponent( WixDataDlg& dialog);
  void       changeComponent( WixDataDlg& dialog);
  void       newComponent(    WixDataDlg& dialog);
  Component* newItem();
  void       leavingCB(       WixDataDlg& dialog);
  void       delAllComponents(WixDataDlg& dialog);
  void       delComponent(    WixDataDlg& dialog);

  void       delWixData(  );

  Component* getCur() {oneComponentAvail(); return components.curData;}

  Component* findAnApp();

  void identifyIconsUsed();
  void identifyDirectoriesUsed();

  void outputSetPath(int tab, bool& crlfOut);

  void outputRefs(int tab);
  bool validate();
  void output();

private:

  void oneComponentAvail() {if (!components.nData) components.add(String(""));}
  void readOne(TCchar* key, String& v);
  void copyObj(Group& g) {
    id = g.id; wixID = g.wixID; components = g.components; section = g.section;
    save = g.save; isUninstall = g.isUninstall;
    }
  };

