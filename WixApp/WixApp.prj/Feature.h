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
  Feature(Feature& c) {copy(c);}
 ~Feature() { }

  Feature& operator= (Feature& c) {copy(c); return *this;}

  void       clear();

  bool       isValid() {return save && !isUninstall && !id.isEmpty();}

  void       readWixData( );
  void       writeWixData();
  void       readWixData2( String& section);
  void       writeWixData2(String& section);

  void       loadNew(WixAppDlg& dialog);
  void       load(   WixAppDlg& dialog);

  void       storeProgFileName(  WixAppDlg& dialog);
  void       storeMenuName(      WixAppDlg& dialog);

  void       loadComponent(WixAppDlg& dialog);
  void       storeComponent(WixAppDlg& dialog);

  void       setDirectories(String& progID, String& menuID)
                                                       {progFileID = progID; startMenuID = menuID;}

  DirDesc*   getProgFile();
  DirDesc*   getStartMenu();

  void       store(WixAppDlg& dialog);

  int        nComponents() {return components.data.end();}

  void       storeComponentData(WixAppDlg& dialog);
  void       changeComponent(   WixAppDlg& dialog);
  void       newComponent(      WixAppDlg& dialog);

  Component* newItem();
  Component* newItem(TCchar* id);

  void       delAllComponents(WixAppDlg& dialog);
  void       delComponent(    WixAppDlg& dialog);

  void       delWixData(  );

  Component* getCur() {oneComponentAvail(); return components.curData();}

  Component* findAnApp();

  void       markIconsUsed();

  void       markDirs();

  Component* findCmp(TCchar* id) {return components.findItem(id);}

  bool       validate(bool rptErrors = true);

  bool       outputSetPath(int tab, bool& crlfOut);    // Add app to Environment variable Path
  void       outputRefs(int tab);
  void       output(int tab);

  void       saveData(Archive& ar);

private:

  void       oneComponentAvail() {if (!nComponents()) components.add(String(""));}
  void       readOne(TCchar* key, String& v);
  void       copy(Feature& g);

  friend class Component;
  };

