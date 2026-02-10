// A Single Component's data


#pragma once
#include "DirDesc.h"
#include "Icons.h"
#include "RelPath.h"


class Feature;


class Component : public Data {
String   section;
String   guid;
String   progFileID;
String   startMenuID;
String   iconID;

public:

RelPath  relPath;
String   description;
String   target;
String   arguments;
bool     iconUsed;
bool     isStartMenu;
bool     isDesktop;
bool     isVersionAvail;
bool     isOnPath;
bool     isStartupApp;

bool     isUninstall;
bool     isUninstallDir;
bool     isApp;

  Component();

  Component(Component& c) {copyObj(c);}
 ~Component() { }

  Component& operator= (Component& c) {copyObj(c); return *this;}

  void       readWixData(String& ftrID);
  void       writeWixData();
  void       readWixData2(String& ftrID, String& cmpSection);
  void       writeWixData2(String& cmpSection);
  void       delWixData() {}

  void       loadNew(WixAppDlg& dialog);
  void       load(   WixAppDlg& dialog);
  void       store(  WixAppDlg& dialog);

  String&    getProgFileID() {return progFileID;}
  DirDesc*   getProgFile();

  String&    getStartMenuID() {return startMenuID;}
  DirDesc*   getStartMenu();

  void       browsePath(String& ftrID, WixAppDlg& dialog);
  void       storeDescription(   WixAppDlg& dialog);
  void       storeIsStartMenu(   WixAppDlg& dialog);
  void       storeIsDeskTop(     WixAppDlg& dialog);
  void       storeIsVersionAvail(WixAppDlg& dialog);
  void       storeIsOnPath(      WixAppDlg& dialog);
  void       storeStartup(       WixAppDlg& dialog);
  void       delData();


  void       getIconPath(WixAppDlg& dialog);
  void       updateIcon(WixAppDlg& dialog);
  void       updateIconCB(ComboBox& cb);

  void       setIcon(String& id) {iconID = id;}
  void       identifyIconUsed()  {icons.markIcon(iconID);}

  void       markDirs(Feature& ftr);
  void       markDirectories(String& id, DirStor& stor);

  void       prepareUninstalls(String& newID, TCchar* wixid, String& progFileId);

  bool       outputSetEnvPath(int tab);

  bool       validate(bool rptErrors = true);
  void       output(int tab);

  void       saveData(Archive& ar);

private:

  void       setApp(String& path);

  void       outputSourcePath(int tab);
  void       outputLink(int tab, TCchar* suffix, TCchar* dir);
  void       outputRegistry( int tab);
  String     getSetEnvPath();
  void       readOne(TCchar* key, String& v);
  void       copyObj(Component& c);
  };




////////---------------------

#if 0
  void       storeIsWin10(       WixAppDlg& dialog);
  void       storeIsWin7(        WixAppDlg& dialog);
  void       storeIsWinXP(       WixAppDlg& dialog);
#endif
#if 0
bool     isWin10;
bool     isWin7;
bool     isWinXP;
#endif
//  bool       outputSetPath(int tab, bool& crlfOut);


