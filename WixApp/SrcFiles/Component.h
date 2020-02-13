// A Single Component's data


#pragma once
#include "DirDesc.h"
#include "Icons.h"
#include "iniFile.h"
#include "PathDesc.h"


class Component : public Data {
String section;
String guid;

public:

PathDesc pathDsc;
String    description;
String    target;
String    arguments;
DirDesc   progFile;
DirDesc   startMenu;
IconDesc  icon;
bool      iconUsed;
bool      isStartMenu;
bool      isDesktop;
bool      isVersionAvail;
bool      isOnPath;
bool      isWin7;
bool      isWinXP;
bool      isWin2K;

bool      isUninstall;
bool      isUninstallDir;
bool      regOut;
bool      isApp;

  Component();

  Component(Component& c) {copyObj(c);}
 ~Component() { }

  Component& operator= (Component& c) {copyObj(c); return *this;}

  void readWixData();
  void writeWixData();
  void delWixData() {}

  void loadNew(WixDataDlg& dialog);
  void load(   WixDataDlg& dialog);
  void store(  WixDataDlg& dialog);

  void updateComponent(    WixDataDlg& dialog);
  void browsePath(         WixDataDlg& dialog);
  void storeDescription(   WixDataDlg& dialog);
  void storeProgFileName(  WixDataDlg& dialog);
  void storeMenuName(      WixDataDlg& dialog);
  void storeIsStartMenu(   WixDataDlg& dialog);
  void storeIsDeskTop(     WixDataDlg& dialog);
  void storeIsVersionAvail(WixDataDlg& dialog);
  void storeIsOnPath(      WixDataDlg& dialog);
  void storeIsWin7(        WixDataDlg& dialog);
  void storeIsWinXP(       WixDataDlg& dialog);
  void storeIsWin2K(       WixDataDlg& dialog);
  void delData();


  void browseIcon(WixDataDlg& dialog);
  void updateIcon(WixDataDlg& dialog);

  void identifyIconUsed();
  void identifyDirectoriesUsed();

  void outputSetPath(int tab, bool& crlfOut);

  bool validate();
  void output(int tab);

private:

  void   setApp(String& path);

  void   outputStartMenu(int tab);
  void   outputDesktop(  int tab);
  void   outputRegistry( int tab);
  void   outputCondition(int tab, int firstCond, int lastCond = 0);
  String getSetEnvPath();

  void   readOne(TCchar* key, String& v);
  void   readDir(TCchar* key, DirDesc& dir, DirStor& stor);
  void   copyObj(Component& c);
  };
