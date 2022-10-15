// Product Section


#pragma once
#include "Features.h"
#include "WixOut.h"
//#include "Prolog.h"
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
PathDesc licenseDsc;

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

  void browseIcon(WixAppDlg& dialog);
  void updateIconCB(ComboBox& cb);
//  void updateCB(ComboBox& cb);
  void updateIcon(WixAppDlg& dialog);

  void setInstallerIconPath(String& path) {installerIconPath = path;}

  void mark();

  void output(Component* app, Features& features);        //Prolog& prolog,

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
