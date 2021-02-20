// Product Section


#pragma once
#include "Features.h"
#include "WixOut.h"
#include "Prolog.h"
#include "WixUtilities.h"


class Product {
public:

String   productName;
String   company;
String   wixName;
String   wixVersion;
String   wixPath;
String   upgradeGUID;
String   iconID;
String   installerIconPath;
bool     isSameVerAllowed;
bool     isLicenseReq;
PathDesc licenseDsc;

  Product() : isSameVerAllowed(false), isLicenseReq(false) { }
 ~Product() { }

  void readWixData( );
  void writeWixData();
  void updateVersion(String& path);

  void loadCB(WixDataDlg& dialog);
  void loadVerEB(WixDataDlg& dialog);

  void storeProduct(  WixDataDlg& dialog);
  void storeCompany(  WixDataDlg& dialog) {company      = getText(dialog.companyEB);}
  void storeWixName(  WixDataDlg& dialog) {wixName      = getText(dialog.wixNameEB);}
  void storeWixVer(   WixDataDlg& dialog) {wixVersion   = getText(dialog.wixVersionEB);}

  void browseIcon(WixDataDlg& dialog);

  void setInstallerIconPath(String& path) {installerIconPath = path;}

  void mark();

  void output(Component* app, Prolog& prolog, Features& features);

private:

  void package();
  void majorUpgrade();
  void skipLicense();
  void addLicense();
  void userInterface();
  void installerIcons();
  void oneInstallerIcon(TCchar* id, TCchar* bmp);
  void outputIcons();
  };


extern Product product;
