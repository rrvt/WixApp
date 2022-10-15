// Product Section


#include "stdafx.h"
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

static BrowseDsc browseDsc      = {DefLicensePathKey, _T("License File"), _T(""), _T(""), _T("")};

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
  licenseDsc.readWixData(browseDsc, ProductSection, LicensePathKey);
  wxd.readString(ProductSection, IconIDKey,      iconID);
  }


void Product::updateVersion(String& path)
                                {ResourceData res(path); wixVersion.clear(); res.getVersion(wixVersion);}


void Product::storeProduct(WixAppDlg& dialog) {
  productName  = getText(dialog.productNameEB);
  }



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


void Product::loadVerEB(WixAppDlg& dialog) {
  dialog.wixVersionEB.SetWindowText(wixVersion);
  }



void Product::browseIcon(WixAppDlg& dialog) {
ComboBox& cb = dialog.prodIconCB;

  iconID = icons.browse(false);   updateIconCB(cb);  //icons.loadCB(cb);    icons.setCur(iconID, cb);
//  dialog.prodIconCB.SetWindowText(iconID);
  }


void Product::updateIconCB(ComboBox& cb) {icons.loadCB(cb);    icons.setCur(iconID, cb);}


void Product::updateIcon(WixAppDlg& dialog) {dialog.prodIconCB.getWindowText(iconID);}


void Product::writeWixData() {

  wxd.write(ProductSection, ProductNameKey, productName);
  wxd.write(ProductSection, CompanyKey,     company);
  wxd.write(ProductSection, WixNameKey,     wixName);
  wxd.write(ProductSection, WixVersionKey,  wixVersion);
  wxd.write(ProductSection, UpgradeCodeKey, upgradeCode);
  wxd.write(ProductSection, SameVerAllowed, isSameVerAllowed);
  wxd.write(ProductSection, LicenseReqKey,  isLicenseReq);
  licenseDsc.writeWixData(ProductSection, LicensePathKey);
  wxd.write(ProductSection, IconIDKey,      iconID);
  }




void Product::output(Component* app, Features& features) {      // Prolog& prolog,
String line;
String ver;

  wix.crlf();

  if (upgradeCode.isEmpty()) getGuid(upgradeCode);

  line = _T("<Product Id=\"*\" UpgradeCode=\"") + upgradeCode + _T("\" Language=\"1033\"");
  wix(0);   wix(line);   wix.crlf();
  line = _T("Manufacturer=\"") + company + _T("\" Name=\"") + productName + _T("\"");
  wix(9); wix(line);   wix.crlf();

  ver = app && app->isVersionAvail ? _T("!(bind.fileVersion.") + app->wixID + _T(")") : wixVersion;
  line = _T("Version=\"") + ver + _T("\"");
  wix(9); wix(line);   wix.crlf();
  wix(9); wix(_T(">"));   wix.crlf();

  package();

  majorUpgrade();

  if (isLicenseReq) addLicense();
  else              userInterface();

  installerIcons();  outputIcons();

  features.outputSetPath(1);

  features.outputFeatures(1);

  wix(0);   wix(_T("</Product>"));   wix.crlf();
  }


void Product::package() {
String line;

  wix.crlf();
  wix(1);   wix(_T("<Package InstallerVersion=\"200\" Compressed=\"yes\" InstallScope=\"perMachine\""));
  wix.crlf();
  line = _T("Manufacturer=\"") + company + _T("\" Description=\"Installs ") + productName + _T("\"");
  wix(11); wix(line);   wix.crlf();
  line = _T("Comments=\"Copyright (c) ") + company +  _T("\"");
  wix(11); wix(line);   wix.crlf();
  wix(11); wix(_T("/>"));   wix.crlf();
  }


//  <MajorUpgrade Schedule="afterInstallInitialize"
//                AllowDowngrades="no"   AllowSameVersionUpgrades="yes"   IgnoreRemoveFailure="no"
//                DowngradeErrorMessage="A newer version of [ProductName] is already installed." />

void Product::majorUpgrade() {
  wix.crlf();
  wix(1);   wix(_T("<MajorUpgrade Schedule=\"afterInstallInitialize\""));   wix.crlf();
  wix(16);
  wix(_T("AllowDowngrades=\"no\"   IgnoreRemoveFailure=\"no\"   AllowSameVersionUpgrades="));
  if (isSameVerAllowed) wix(_T("\"yes\""));
  else                  wix(_T("\"no\""));
  wix.crlf();
  wix(16);
  wix(_T("DowngradeErrorMessage=\"A newer version of [ProductName] is already installed.\" />"));
  wix.crlf(2);

  wix(1);   wix(_T("<MediaTemplate EmbedCab=\"yes\" />"));   wix.crlf();
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

  wix.crlf();   wix(1);

  wix(_T("<WixVariable Id=\"WixUILicenseRtf\" Value=\"") + relPath + _T("\" />"));   wix.crlf();

  wix(_T("<UI Id=\"My_InstallDir.ui\"><UIRef Id=\"WixUI_InstallDir\" /></UI>"));   wix.crlf();
  }


void Product::userInterface() {
String line;

  wix.crlf();
  wix(1);   wix(_T("<UI Id=\"My_InstallDir.ui\">"));   wix.crlf();
  wix(2);   wix(_T("<UIRef Id=\"WixUI_InstallDir\" />"));   wix.crlf();
  line =
      _T("<Publish Dialog=\"WelcomeDlg\" Control=\"Next\" Event=\"NewDialog\" Value=\"InstallDirDlg\">");

  wix(line);   wix.crlf();
  wix(3);   wix(_T("NOT Installed"));   wix.crlf();
  wix(2);   wix(_T("</Publish>"));   wix.crlf();

  line = _T("<Publish Dialog=\"InstallDirDlg\" Control=\"Back\" ")
         _T("Event=\"NewDialog\" Value=\"WelcomeDlg\" Order=\"2\">");
  wix(line);   wix.crlf();
  wix(3);   wix(_T("1"));   wix.crlf();
  wix(2);   wix(_T("</Publish>"));   wix.crlf();

  wix(1);   wix(_T("</UI>"));   wix.crlf();
  }


void Product::skipLicense() {

  wix.crlf();
  wix(1);   wix(_T("<UI Id=\"SkipLicenseAgreement\">"));   wix.crlf();
  wix(2);   wix(_T("<UIRef Id=\"WixUI_InstallDir\" />"));   wix.crlf();
            wix(_T("<Publish Dialog=\"WelcomeDlg\"    Control=\"Next\" "));
  wix(0);   wix(_T("Event=\"NewDialog\" Value=\"InstallDirDlg\" Order=\"2\">"));   wix.crlf();
  wix(3);   wix(_T("1"));   wix.crlf();
  wix(2);   wix(_T("</Publish>"));   wix.crlf();
            wix(_T("<Publish Dialog=\"InstallDirDlg\" Control=\"Back\" "));
  wix(0);   wix(_T("Event=\"NewDialog\" Value=\"WelcomeDlg\"    Order=\"2\">"));   wix.crlf();
  wix(3);   wix(_T("1"));   wix.crlf();
  wix(2);   wix(_T("</Publish>"));   wix.crlf();
  wix(1);   wix(_T("</UI>"));   wix.crlf();
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
  line += _T("Value=\"") + installerIconPath + bmp; line += _T("\"/>");

  wix(1);   wix(line);   wix.crlf();
  }



void Product::mark() {
IconDesc* p;

  p = icons.find(iconID);  if (p) p->inUse = true;

  icons.markDfltDir();

  if (isLicenseReq) defaultPath.mark(DefLicensePathKey);
  }


void Product::outputIcons() {
String    line;
IconDesc* dsc = icons.find(iconID);

  wix.crlf();

  wix(1);
  icons.output();

  if (dsc) wix(_T("<Property Id=\"ARPPRODUCTICON\"  Value=\""), dsc->wixID, _T("\" />"));
  }

