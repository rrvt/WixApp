// ComponentGrpList -- One entity of install


#pragma once
#include "Component.h"


class Feature : public Data {
typedef EntityStore <Component, 1> Components;
typedef EStoreIter  <Component, 1> ComponentsIter;

Components components;
String     section;
String     progFileID;
String     startMenuID;

public:

bool    save;
bool    isUninstall;

  Feature() : save(true), isUninstall(false) {}
  Feature(Feature& c) {copyObj(c);}
 ~Feature() { }

  Feature& operator= (Feature& c) {copyObj(c); return *this;}

  void delData()
    {id.clear(); components.clear(); section.clear(); wixID.clear(); save = false; isUninstall = false;}

  void       readWixData( );
  void       writeWixData();
  void       readWixData2( String& section);
  void       writeWixData2(String& section);

  void       loadNew(WixDataDlg& dialog);
  void       load(   WixDataDlg& dialog);

  void       storeProgFileName(  WixDataDlg& dialog);
  void       storeMenuName(      WixDataDlg& dialog);

  void       loadComponent(WixDataDlg& dialog);
  void       storeComponent(WixDataDlg& dialog);

  void       setDirectories(String& progID, String& menuID) {progFileID = progID; startMenuID = menuID;}

  DirDesc*   getProgFile();
  DirDesc*   getStartMenu();

  void       store(WixDataDlg& dialog);

  int        nComponents() {return components.data.end();}

  void       storeComponentData(WixDataDlg& dialog);
  void       changeComponent(   WixDataDlg& dialog);
  void       newComponent(      WixDataDlg& dialog);

  Component* newItem(TCchar* id = 0);
  void       delAllComponents(WixDataDlg& dialog);
  void       delComponent(    WixDataDlg& dialog);

  void       delWixData(  );

  Component* getCur() {oneComponentAvail(); return components.curData();}

  Component* findAnApp();

  void       markIconsUsed();

  void       markDirs();
  void       markDfltDirs();

  Component* findCmp(TCchar* id) {return components.find(id);}

  void       outputSetPath(int tab, bool& crlfOut);

  void       outputRefs(int tab);
  bool       validate(bool rptErrors = true);
  void       output();

private:

  void   oneComponentAvail() {if (!nComponents()) components.add(String(""));}
  void   readOne(TCchar* key, String& v);
  void   copyObj(Feature& g) {
    id = g.id; wixID = g.wixID; components  = g.components; section = g.section;
    progFileID = g.progFileID;  startMenuID = g.startMenuID; save = g.save; isUninstall = g.isUninstall;
    }

  friend class Component;
  };

