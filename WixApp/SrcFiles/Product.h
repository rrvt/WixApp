// Product Section


#pragma once
#include "Feature.h"
#include "WixOut.h"
#include "Prolog.h"
#include "Expandable.h"
#include "WixUtilities.h"


class Product {
public:

String    wixPath;
String    wixName;
String    wixVersion;
String    company;
String    productName;
String    upgradeGUID;
IconDesc  ctrlPanel;
String    installerIconPath;
bool      isSameVerAllowed;
bool      isLicenseReq;
PathDesc  licenseDsc;

  Product() : isSameVerAllowed(false), isLicenseReq(false) { }
 ~Product() { }

  void readWixData( );
  void writeWixData();
  void updateVersion(String& path);

  void loadCB(WixDataDlg& dialog);
  void loadVerEB(WixDataDlg& dialog);

  void storeWixName(  WixDataDlg& dialog) {wixName      = getText(dialog.wixNameEB);}
  void storeWixVer(   WixDataDlg& dialog) {wixVersion   = getText(dialog.wixVersionEB);}
  void storeCompany(  WixDataDlg& dialog) {company      = getText(dialog.companyEB);}
  void storeProduct(  WixDataDlg& dialog) {productName  = getText(dialog.productNameEB);}

  void browseIcon(WixDataDlg& dialog);

  void setInstallerIconPath(String& path) {installerIconPath = path;}

  void identifyIcons();

  void output(Component* app, Prolog& prolog, Feature& feature);

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
