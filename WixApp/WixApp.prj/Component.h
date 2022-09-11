// A Single Component's data


#pragma once
#include "DirDesc.h"
#include "Icons.h"
#include "iniFile.h"
#include "PathDesc.h"


class Feature;


class Component : public Data {
String   section;
String   guid;
String   progFileID;
String   startMenuID;
String   iconID;

public:

PathDesc pathDsc;
String   description;
String   target;
String   arguments;
bool     iconUsed;
bool     isStartMenu;
bool     isDesktop;
bool     isVersionAvail;
bool     isOnPath;
bool     isStartupApp;
bool     isWin7;
bool     isWinXP;
bool     isWin2K;

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

  void       loadNew(WixDataDlg& dialog);
  void       load(   WixDataDlg& dialog);
  void       store(  WixDataDlg& dialog);

  String&    getProgFileID() {return progFileID;}
  DirDesc*   getProgFile();

  String&    getStartMenuID() {return startMenuID;}
  DirDesc*   getStartMenu();

  void       browsePath(String& ftrID, WixDataDlg& dialog);
  void       storeDescription(   WixDataDlg& dialog);
  void       storeIsStartMenu(   WixDataDlg& dialog);
  void       storeIsDeskTop(     WixDataDlg& dialog);
  void       storeIsVersionAvail(WixDataDlg& dialog);
  void       storeIsOnPath(      WixDataDlg& dialog);
  void       storeStartup(       WixDataDlg& dialog);
  void       storeIsWin7(        WixDataDlg& dialog);
  void       storeIsWinXP(       WixDataDlg& dialog);
  void       storeIsWin2K(       WixDataDlg& dialog);
  void       delData();


  void       browseIcon(WixDataDlg& dialog);
  void       updateIcon(WixDataDlg& dialog);

  void       setIcon(String& id) {iconID = id;}
  void       identifyIconUsed()  {icons.markIcon(iconID);}

  void       markDirs(Feature& ftr);
  void       markDirectories(String& id, DirStor& stor);

  void       prepareUninstalls(String& newID, String& wixid, String& progFileId);

  void       outputSetPath(int tab, bool& crlfOut);

  bool       validate(bool rptErrors = true);
  void       output(int tab);

private:

  void       setApp(String& path);

  void       outputLink(int tab, TCchar* suffix, TCchar* dir);
  void       outputRegistry( int tab);
  void       outputCondition(int tab, int firstCond, int lastCond = 0);
  String     getSetEnvPath();

  void       readOne(TCchar* key, String& v);
  void       copyObj(Component& c);
  };





//  void       outputStartMenu(int tab);
//  void       outputDesktop(  int tab);

