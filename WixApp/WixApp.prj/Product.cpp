// Product Section


#include "pch.h"
#include "Product.h"
#include "DefaultPath.h"
#include "filename.h"
#include "Guid.h"
#include "Icons.h"
#include "Solution.h"
#include "WixOptsDlg.h"
#include "Resources.h"


static TCchar*   ProductSection = _T("Product");
static TCchar*   WixNameKey     = _T("WixName");
static TCchar*   WixVersionKey  = _T("WixVersion");
static TCchar*   CompanyKey     = _T("Company");
static TCchar*   ProductNameKey = _T("ProductName");
static TCchar*   UpgradeGUIDKey = _T("UpgradeGUID");
static TCchar*   UpgradeCodeKey = _T("UpgradeCode");
static TCchar*   DefaultCondKey = _T("DefaultCond");
static TCchar*   SameVerAllowed = _T("SaveVerAllowed");
static TCchar*   LicenseReqKey  = _T("LicenseRequired");
static TCchar*   LicensePathKey = _T("LicensePath");
static TCchar*   IconIDKey      = _T("IconID");

static KeyedPathDsc browseDsc = {DefLicensePathKey, _T("License File"), _T(""), _T(""), _T("")};

Product product;


void Product::readWixData() {
double ver = solution.getVer();

  wxd.readString(ProductSection, WixNameKey,     wixName);
  wxd.readString(ProductSection, WixVersionKey,  wixVersion);
  wxd.readString(ProductSection, CompanyKey,     company);
  wxd.readString(ProductSection, ProductNameKey, productName);

  if (ver > 2.0) wxd.read(ProductSection, UpgradeCodeKey, upgradeCode);
  else           wxd.read(ProductSection, UpgradeGUIDKey, upgradeCode);

  isSameVerAllowed = wxd.readInt(ProductSection, SameVerAllowed, 0) != 0;
  isLicenseReq     = wxd.readInt(ProductSection, LicenseReqKey,  0) != 0;
  licPath.readWixData(ProductSection, LicensePathKey, browseDsc);
  wxd.readString(ProductSection, IconIDKey, iconID);
  }


void Product::updateVersion(String& path)
                          {ResourceData res(path); wixVersion.clear(); res.getVersion(wixVersion);}


void Product::storeProduct(WixAppDlg& dialog) {productName  = getText(dialog.productNameEB);}



void Product::loadCB(WixAppDlg& dialog) {
ComboBox& cb = dialog.prodIconCB;

  dialog.productNameEB.SetWindowText(productName);
      dialog.companyEB.SetWindowText(company);
      dialog.wixNameEB.SetWindowText(wixName);
      loadVerEB(dialog);

      icons.loadCB(cb);
      icons.setCur(iconID, cb);

  dialog.prodIconCB.SetWindowText(iconID);
  }


void Product::loadVerEB(WixAppDlg& dialog) {dialog.wixVersionEB.SetWindowText(wixVersion);}


void Product::getIconPath(WixAppDlg& dialog)
           {ComboBox& cb = dialog.prodIconCB;   iconID = icons.getPath(false);   updateIconCB(cb);}


void Product::updateIconCB(ComboBox& cb) {icons.loadCB(cb);   icons.setCur(iconID, cb);}


void Product::updateIcon(WixAppDlg& dialog) {dialog.prodIconCB.getWindowText(iconID);}


void Product::writeWixData() {

  wxd.write(ProductSection, ProductNameKey, productName);
  wxd.write(ProductSection, CompanyKey,     company);
  wxd.write(ProductSection, WixNameKey,     wixName);
  wxd.write(ProductSection, WixVersionKey,  wixVersion);
  wxd.write(ProductSection, UpgradeCodeKey, upgradeCode);
  wxd.write(ProductSection, SameVerAllowed, isSameVerAllowed);
  wxd.write(ProductSection, LicenseReqKey,  isLicenseReq);
  licPath.writeWixData(ProductSection, LicensePathKey);
  wxd.write(ProductSection, IconIDKey,      iconID);
  }


void Product::output(Component* app, Features& features) {
String line;
String ver;

  wix.crlf();

  if (upgradeCode.isEmpty()) getGuid(upgradeCode);

  productID(0, app);        wix.crlf();

  package(TabVal);          wix.crlf();

  majorUpgrade(TabVal);     wix.crlf();

  if (isLicenseReq) addLicense(TabVal);
  else              userInterface(TabVal);
  wix.crlf();

  installerIcons(TabVal);   wix.crlf();

  outputIcons(TabVal);      wix.crlf();

  if (features.outputSetPaths(TabVal)) wix.crlf();

  features.outputFeatures(TabVal);

  wix(0, _T("</Product>"));
  }


void Product::productID(int tab, Component* app) {
String line;
String ver;

  line = _T("<Product Id=\"*\" UpgradeCode=\"") + upgradeCode + _T("\" Language=\"1033\"");
  wix(tab, line);
  line = _T("Manufacturer=\"") + company + _T("\" Name=\"") + productName + _T("\"");
  wix(16, line);

  ver = app && app->isVersionAvail ? _T("!(bind.fileVersion.") + app->wixID + _T(")") : wixVersion;
  line = _T("Version=\"") + ver + _T("\"");
  wix(16, line);
  wix(16, _T(">"));
  }


void Product::package(int tab) {
String line;

  wix(tab, _T("<Package InstallerVersion=\"200\" Compressed=\"yes\" InstallScope=\"perMachine\""));

  line = _T("Manufacturer=\"") + company + _T("\" Description=\"Installs ") + productName;
  line += _T("\"");
  wix(16, line);

  line = _T("Comments=\"Copyright (c) ") + company +  _T("\"");
  wix(16, line);
  wix(16, _T("/>"));
  }


//  <MajorUpgrade Schedule="afterInstallInitialize"
//                AllowDowngrades="no"   AllowSameVersionUpgrades="yes"   IgnoreRemoveFailure="no"
//                DowngradeErrorMessage="A newer version of [ProductName] is already installed." />

void Product::majorUpgrade(int tab) {
String line;

  wix(tab, _T("<MajorUpgrade Schedule=\"afterInstallInitialize\""));

  line = _T("AllowDowngrades=\"no\"   IgnoreRemoveFailure=\"no\"   AllowSameVersionUpgrades=");
  line += isSameVerAllowed ? _T("\"yes\"") : _T("\"no\"");
  wix(16, line);

  wix(16, _T("DowngradeErrorMessage=\"A newer version of [ProductName] is already installed.\""));
  wix(16, _T("/>"));
  wix.crlf();

  wix(tab, _T("<MediaTemplate EmbedCab=\"yes\" />"));
  }


void Product::addLicense(int tab) {
String relPath = licPath.prodPath();

  wix(tab, _T("<WixVariable Id=\"WixUILicenseRtf\" Value=\"") + relPath + _T("\" />"));

  wix(tab, _T("<UI Id=\"My_InstallDir.ui\"><UIRef Id=\"WixUI_InstallDir\" /></UI>"));
  }


void Product::userInterface(int tab) {
String line;

  wix(TabVal,   _T("<UI Id=\"My_InstallDir.ui\">"));
  tab += TabVal;
  wix(tab, _T("<UIRef Id=\"WixUI_InstallDir\" />"));

  line  = _T("<Publish Dialog=\"WelcomeDlg\" Control=\"Next\" Event=\"NewDialog\" ");
  line += _T("Value=\"InstallDirDlg\">");
  wix(tab, line);

  tab += TabVal;   wix(tab, _T("NOT Installed"));   tab -= TabVal;
  wix(tab, _T("</Publish>"));

  line = _T("<Publish Dialog=\"InstallDirDlg\" Control=\"Back\" ")
         _T("Event=\"NewDialog\" Value=\"WelcomeDlg\" Order=\"2\">");
  wix(tab, line);

  tab += TabVal;   wix(tab, _T("1"));   tab -= TabVal;

  wix(tab, _T("</Publish>"));

  tab -= TabVal;   wix(tab, _T("</UI>"));
  }


void Product::skipLicense(int tab) {
String s;

  wix(tab, _T("<UI Id=\"SkipLicenseAgreement\">"));
  tab += TabVal;   wix(tab, _T("<UIRef Id=\"WixUI_InstallDir\" />"));
  s  = _T("<Publish Dialog=\"WelcomeDlg\"    Control=\"Next\" ");
  s += _T("Event=\"NewDialog\" Value=\"InstallDirDlg\" Order=\"2\">");
  wix(tab, s);
  tab += TabVal;   wix(tab, _T("1"));
  tab -= TabVal;   wix(tab, _T("</Publish>"));
  s  = _T("<Publish Dialog=\"InstallDirDlg\" Control=\"Back\" ");
  s += _T("<Publish Dialog=\"InstallDirDlg\" Control=\"Back\" ");
  s += _T("Event=\"NewDialog\" Value=\"WelcomeDlg\"    Order=\"2\">");
  wix(tab, s);

  tab += TabVal;    wix(tab, _T("1"));
  tab -= TabVal;    wix(_T("</Publish>"));
  tab -= TabVal;    wix(tab, _T("</UI>"));
  }



void Product::installerIcons(int tab) {
String line;

  oneInstallerIcon(tab, _T("WixUIDialogBmp"), _T("dialog.bmp"));
  oneInstallerIcon(tab, _T("WixUIBannerBmp"), _T("banner.bmp"));
  }


void Product::oneInstallerIcon(int tab, TCchar* id, TCchar* bmp) {
String line;

  line  = _T("<WixVariable Id=\""); line += id; line += _T("\" ");
  line += _T("Value=\"") + installerIconPath + bmp; line += _T("\"/>");

  wix(tab, line);
  }



void Product::mark() {
IconDesc* p;

  p = icons.find(iconID);  if (p) p->inUse = true;

  icons.markDfltDir();

  if (isLicenseReq) defaultPath.mark(DefLicensePathKey);
  }


void Product::outputIcons(int tab) {
String    line;
IconDesc* dsc = icons.find(iconID);

  icons.output(tab);

  if (dsc) wix(tab, _T("<Property Id=\"ARPPRODUCTICON\"  Value=\""), dsc->wixID, _T("\" />"));
  }

