// Product Section


#include "stdafx.h"
#include "Product.h"
#include "filename.h"
#include "Guid.h"
#include "Icons.h"
#include "Resources.h"


static TCchar* ProductSection = _T("Product");
static TCchar* WixNameKey     = _T("WixName");
static TCchar* WixVersionKey  = _T("WixVersion");
static TCchar* CompanyKey     = _T("Company");
static TCchar* ProductNameKey = _T("ProductName");
static TCchar* UpgradeGUIDKey = _T("UpgradeGUID");
static TCchar* DefaultCondKey = _T("DefaultCond");
static TCchar* SameVerAllowed = _T("SaveVerAllowed");
static TCchar* LicenseReqKey  = _T("LicenseRequired");
static TCchar* LicensePathKey = _T("LicensePath");
static TCchar* IconIDKey      = _T("IconID");

Product product;


void Product::readWixData() {

  wxd.readString(ProductSection, WixNameKey,     wixName);
  wxd.readString(ProductSection, WixVersionKey,  wixVersion);
  wxd.readString(ProductSection, CompanyKey,     company);
  wxd.readString(ProductSection, ProductNameKey, productName);
  wxd.readString(ProductSection, UpgradeGUIDKey, upgradeGUID);
  isSameVerAllowed = wxd.readInt(ProductSection, SameVerAllowed, 0) != 0;
  isLicenseReq     = wxd.readInt(ProductSection, LicenseReqKey,  0) != 0;
  licenseDsc.readWixData(ProductSection, LicensePathKey);
  wxd.readString(ProductSection, IconIDKey,      iconID);
  }


void Product::updateVersion(String& path)
                                {ResourceData res(path); wixVersion.clear(); res.getVersion(wixVersion);}


void Product::storeProduct(WixDataDlg& dialog) {productName  = getText(dialog.productNameEB);}



void Product::loadCB(WixDataDlg& dialog) {
  dialog.productNameEB.SetWindowText(productName);
      dialog.companyEB.SetWindowText(company);
      dialog.wixNameEB.SetWindowText(wixName);
      loadVerEB(dialog);
  dialog.progFtrIconEB.SetWindowText(iconID);
  }


void Product::loadVerEB(WixDataDlg& dialog) {
  dialog.wixVersionEB.SetWindowText(wixVersion);
  }



void Product::browseIcon(WixDataDlg& dialog) {
ComboBox& cb = dialog.iconCB;

  iconID = icons.browse();

  dialog.progFtrIconEB.SetWindowText(iconID);
  }


void Product::writeWixData() {

  wxd.writeString(ProductSection, ProductNameKey, productName);
  wxd.writeString(ProductSection, CompanyKey,     company);
  wxd.writeString(ProductSection, WixNameKey,     wixName);
  wxd.writeString(ProductSection, WixVersionKey,  wixVersion);
  wxd.writeString(ProductSection, UpgradeGUIDKey, upgradeGUID);
  wxd.writeInt(   ProductSection, SameVerAllowed, isSameVerAllowed);
  wxd.writeInt(   ProductSection, LicenseReqKey,  isLicenseReq);
  licenseDsc.writeWixData(ProductSection, LicensePathKey);
  wxd.writeString(ProductSection, IconIDKey,      iconID);
  }




void Product::output(Component* app, Prolog& prolog, Features& features) {
String line;
String ver;

  wix.crlf();

  if (upgradeGUID.isEmpty()) getGuid(upgradeGUID);

  line = _T("<Product Id=\"*\" UpgradeCode=\"") + upgradeGUID + _T("\" Language=\"1033\"\n");
  wix.stg(0, line);
  line = _T("Manufacturer=\"") + company + _T("\" Name=\"") + productName + _T("\"\n");
  wix.spaces(9); wix.stg(0, line);

  ver = app && app->isVersionAvail ? _T("!(bind.fileVersion.") + app->wixID + _T(")") : wixVersion;
  line = _T("Version=\"") + ver + _T("\"\n");
  wix.spaces(9); wix.stg(0, line);
  wix.spaces(9); wix.lit(0, _T(">\n"));

  package();

  majorUpgrade();

  if (isLicenseReq) addLicense();
  else              userInterface();

  installerIcons();  outputIcons();

  features.outputSetPath(1);

  features.outputFeatures(1);

  wix.lit(0, _T("</Product>\n"));
  }


void Product::package() {
String line;

  wix.crlf();
  wix.lit(1, _T("<Package InstallerVersion=\"200\" Compressed=\"yes\" InstallScope=\"perMachine\"\n"));
  line = _T("Manufacturer=\"") + company + _T("\" Description=\"Installs ") + productName + _T("\"\n");
  wix.spaces(11); wix.stg(0, line);
  line = _T("Comments=\"Copyright (c) ") + company +  _T("\"\n");
  wix.spaces(11); wix.stg(0, line);
  wix.spaces(11); wix.lit(0, _T("/>\n"));
  }


//  <MajorUpgrade Schedule="afterInstallInitialize"
//                AllowDowngrades="no"   AllowSameVersionUpgrades="yes"   IgnoreRemoveFailure="no"
//                DowngradeErrorMessage="A newer version of [ProductName] is already installed." />

void Product::majorUpgrade() {
  wix.crlf();
  wix.lit(1, _T("<MajorUpgrade Schedule=\"afterInstallInitialize\"\n"));
  wix.spaces(16);
  wix.lit(0, _T("AllowDowngrades=\"no\"   IgnoreRemoveFailure=\"no\"   AllowSameVersionUpgrades="));
  if (isSameVerAllowed) wix.lit(0, _T("\"yes\"\n"));
  else                  wix.lit(0, _T("\"no\"\n"));
  wix.spaces(16);
  wix.lit(0, _T("DowngradeErrorMessage=\"A newer version of [ProductName] is already installed.\" />\n"));


  wix.crlf(); wix.lit(1, _T("<MediaTemplate EmbedCab=\"yes\" />\n"));
  }



#if 0

void Product::majorUpgrade() {
  wix.crlf();
  wix.lit(1, "<MajorUpgrade Schedule=\"afterInstallFinalize\"\n");
  wix.spaces(16);
  wix.lit(0, "DowngradeErrorMessage=\"A newer version of [ProductName] is already installed.\" ");

  if (isSameVerAllowed) {wix.crlf(); wix.spaces(16); wix.lit(0, "AllowSameVersionUpgrades=\"yes\" ");}
  wix.lit(0, "/>\n");

  wix.crlf(); wix.lit(1, "<MediaTemplate EmbedCab=\"yes\" />\n");
  }
#endif


void Product::addLicense() {
String relPath = licenseDsc.relative();

//  solution.getRelSolution(licenseDsc.full(), relPath);

  wix.crlf();

  wix.lit(1, _T("<WixVariable Id=\"WixUILicenseRtf\" Value=\"") + relPath + _T("\" />\n"));

  wix.lit(1, _T("<UI Id=\"My_InstallDir.ui\"><UIRef Id=\"WixUI_InstallDir\" /></UI>\n"));
  }


void Product::userInterface() {
String line;

  wix.crlf();
  wix.lit(1, _T("<UI Id=\"My_InstallDir.ui\">\n"));
  wix.lit(2, _T("<UIRef Id=\"WixUI_InstallDir\" />\n"));
  line = _T("<Publish Dialog=\"WelcomeDlg\" Control=\"Next\" Event=\"NewDialog\" Value=\"InstallDirDlg\">\n");
  wix.lit(2, line);
  wix.lit(3, _T("NOT Installed\n"));
  wix.lit(2, _T("</Publish>\n"));

  line = _T("<Publish Dialog=\"InstallDirDlg\" Control=\"Back\" ")
         _T("Event=\"NewDialog\" Value=\"WelcomeDlg\" Order=\"2\">\n");
  wix.lit(2, line);
  wix.lit(3, _T("1\n"));
  wix.lit(2, _T("</Publish>\n"));

  wix.lit(1, _T("</UI>\n"));
  }


void Product::skipLicense() {

  wix.crlf();
  wix.lit(1, _T("<UI Id=\"SkipLicenseAgreement\">\n"));
  wix.lit(2, _T("<UIRef Id=\"WixUI_InstallDir\" />\n"));
  wix.lit(2, _T("<Publish Dialog=\"WelcomeDlg\"    Control=\"Next\" "));
  wix.lit(0, _T("Event=\"NewDialog\" Value=\"InstallDirDlg\" Order=\"2\">\n"));
  wix.lit(3, _T("1\n"));
  wix.lit(2, _T("</Publish>\n"));
  wix.lit(2, _T("<Publish Dialog=\"InstallDirDlg\" Control=\"Back\" "));
  wix.lit(0, _T("Event=\"NewDialog\" Value=\"WelcomeDlg\"    Order=\"2\">\n"));
  wix.lit(3, _T("1\n"));
  wix.lit(2, _T("</Publish>\n"));
  wix.lit(1, _T("</UI>\n"));
  }



void Product::installerIcons() {
String line;

  wix.crlf();

  oneInstallerIcon(_T("WixUIDialogBmp"), _T("dialog.bmp"));
  oneInstallerIcon(_T("WixUIBannerBmp"), _T("banner.bmp"));
  }


void Product::oneInstallerIcon(TCchar* id, TCchar* bmp) {
String line;

  line  = _T("<WixVariable Id=\""); line += id; line += _T("\" ");
  line += _T("Value=\"") + installerIconPath + bmp; line += _T("\"/>\n");

  wix.stg(1, line);
  }



void Product::markIcon() {
IconDesc* p;

  p = icons.find(iconID);  if (p) p->inUse = true;

  icons.markDfltDir();
  }


void Product::outputIcons() {
String    line;
IconDesc* dsc = icons.find(iconID);

  wix.crlf();

  icons.output(1);

  if (dsc) wix.out(1, _T("<Property Id=\"ARPPRODUCTICON\"  Value=\""), dsc->wixID, _T("\" />"));
  }

