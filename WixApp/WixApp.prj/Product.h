// Product Section


#pragma once
#include "Features.h"
#include "WixOut.h"
#include "WixUtilities.h"


class Product {
public:

String   productName;
String   company;
String   wixName;
String   wixVersion;
String   wixPath;
String   upgradeCode;
String   iconID;
String   installerIconPath;
bool     isSameVerAllowed;
bool     isLicenseReq;
RelPath  licPath;
bool     isWin10{false};
bool     isWin7{false};
bool     isWinXP{false};

  Product() : isSameVerAllowed(false), isLicenseReq(false) { }
 ~Product() { }

  void readWixData( );
  void writeWixData();
  void updateVersion(String& path);

  void loadCB(WixAppDlg& dialog);
  void loadVerEB(WixAppDlg& dialog);

  void storeProduct(  WixAppDlg& dialog);
  void storeCompany(  WixAppDlg& dialog) {company      = getText(dialog.companyEB);}
  void storeWixName(  WixAppDlg& dialog) {wixName      = getText(dialog.wixNameEB);}
  void storeWixVer(   WixAppDlg& dialog) {wixVersion   = getText(dialog.wixVersionEB);}

  void storeIsWin10(  WixAppDlg& dialog);
  void storeIsWin7(   WixAppDlg& dialog);
  void storeIsWinXP(  WixAppDlg& dialog);

  void getIconPath(   WixAppDlg& dialog);
  void updateIconCB(ComboBox& cb);
  void updateIcon(    WixAppDlg& dialog);

  void setInstallerIconPath(String& path) {installerIconPath = path;}

  void mark();

  void output(Component* app, Features& features);
  void outputOScondition(int tab);

  void saveData(Archive& ar);

private:

  void package(int tab, Component* app);
  void summary(int tab);
  void majorUpgrade(int tab);
  void skipLicense(int tab);
  void addLicense(int tab);
  void userInterface(int tab);
  void installDir(int tab);
  void installerIcons(int tab);
  void oneInstallerIcon(int tab, TCchar* id, TCchar* bmp);
  void outputIcons(int tab);
  };


extern Product product;
