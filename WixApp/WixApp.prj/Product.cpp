// Product Section


#include "pch.h"
#include "Product.h"
#include "DefaultPath.h"
#include "filename.h"
#include "Guid.h"
#include "Icons.h"
#include "PFFdirectories.h"
#include "Solution.h"
#include "WixOptsDlg.h"
#include "ResourceData.h"


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
static TCchar*   IsWin10Key     = _T("IsWin10");
static TCchar*   IsWin7Key      = _T("IsWin7");
static TCchar*   IsWinXPKey     = _T("IsWinXP");


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

  isWin10 = wxd.readInt(ProductSection, IsWin10Key, 0) != 0;
  isWin7  = wxd.readInt(ProductSection, IsWin7Key,  0) != 0;
  isWinXP = wxd.readInt(ProductSection, IsWinXPKey, 0) != 0;
  }


void Product::updateVersion(String& path)
                          {ResourceData res(path); wixVersion.clear(); res.getVersion(wixVersion);}


void Product::storeProduct(WixAppDlg& dialog) {productName  = getText(dialog.productNameEB);}

void Product::storeIsWin10(WixAppDlg& dialog){isWin10     = dialog.isWin10ch.GetCheck()    != 0;}
void Product::storeIsWin7(WixAppDlg& dialog) {isWin7      = dialog.isWin7ch.GetCheck()     != 0;}
void Product::storeIsWinXP(WixAppDlg& dialog){isWinXP     = dialog.isWinXPch.GetCheck()    != 0;}



void Product::loadCB(WixAppDlg& dialog) {
ComboBox& cb = dialog.prodIconCB;

  dialog.productNameEB.SetWindowText(productName);
      dialog.companyEB.SetWindowText(company);
      dialog.wixNameEB.SetWindowText(wixName);
      loadVerEB(dialog);

      icons.loadCB(cb);
     dialog.isWin10ch.SetCheck(isWin10);
     dialog.isWin7ch.SetCheck(isWin7);
     dialog.isWinXPch.SetCheck(isWinXP);

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
  wxd.writeInt(ProductSection, IsWin10Key, isWin10);
  wxd.writeInt(ProductSection, IsWin7Key,  isWin7);
  wxd.writeInt(ProductSection, IsWinXPKey, isWinXP);
  }


void Product::output(Component* app, Features& features) {
String line;
String ver;

  wix.crlf();

  if (upgradeCode.isEmpty()) getGuid(upgradeCode);

  package(0, app);        wix.crlf();

  summary(TabVal);          wix.crlf();

  majorUpgrade(TabVal);     wix.crlf();

#if 1
  addLicense(TabVal);
#else
  if (isLicenseReq) addLicense(TabVal);
  else              userInterface(TabVal);
#endif
  wix.crlf();

  installDir(TabVal);       wix.crlf();

  installerIcons(TabVal);   wix.crlf();

  outputIcons(TabVal);      wix.crlf();

//  if (features.outputSetPaths(TabVal)) wix.crlf();

  features.outputFeatures(TabVal);

  wix(0, _T("</Package>"));
  }


// There are three cases
//  -- One OS
//  -- OS1 to OS2
//  -- OS to infinity
//  <Condition>
//    <![CDATA[Installed OR VersionNT >= 501 AND VersionNT < 601]]>         <!-- Win XP, Vista -->
//  </Condition>

void Product::outputOScondition(int tab) {
int    lower;
int    upper;
String line;

  if (!isWin10 && !isWin7 && !isWinXP) return;

  lower = isWin10 ? _WIN32_WINNT_WIN10 : isWin7  ? _WIN32_WINNT_WIN7 :
                                                               isWinXP ? _WIN32_WINNT_WINXP  : 0;
  upper = isWinXP ? _WIN32_WINNT_WINXP : 0;

  wix(tab, _T("<Condition>"));

  if (lower == upper) line.format(_T("<![CDATA[Installed OR VersionNT = %x]]>"), lower);

  else if (lower && upper)
    line.format(_T("<![CDATA[Installed OR VersionNT >= %x AND VersionNT <= %x]]>"),
                                                                              lower, upper);
  else line.format(_T("<![CDATA[Installed OR VersionNT >= %x]]>"), lower);

  wix(tab + TabVal, line);

  wix(tab, _T("</Condition>"));
  }


void Product::package(int tab, Component* app) {
String line;
String ver;
int    indent = tab+2;

  line = _T("<Package UpgradeCode=\"") + upgradeCode + _T("\" Language=\"1033\"");
  wix(tab, line);
  line = _T("Manufacturer=\"") + company + _T("\" Name=\"") + productName + _T("\"");
  wix(indent, line);

  ver = app && app->isVersionAvail ? _T("!(bind.fileVersion.") + app->wixID + _T(")") : wixVersion;
  line = _T("Version=\"") + ver + _T("\"");
  line += _T(" InstallerVersion=\"500\"");
  wix(indent, line);
  wix(indent, _T(">"));
  }


void Product::summary(int tab) {
String line;

  line =  _T("<SummaryInformation Manufacturer=\"") + company;
  line += _T("\" Description=\"Installs ") + productName + _T("\"");
  wix(tab, line);

  line = _T("Comments=\"Copyright (c) ") + company +  _T("\"");
  wix(tab+2, line);
  wix(tab+2, _T("/>"));
  }


//  <MajorUpgrade Schedule="afterInstallInitialize"
//    AllowDowngrades="no" AllowSameVersionUpgrades="yes" IgnoreRemoveFailure="no"
//    DowngradeErrorMessage="A newer version of [ProductName] is already installed." />

void Product::majorUpgrade(int tab) {
String line;

  wix(tab, _T("<MajorUpgrade Schedule=\"afterInstallInitialize\""));

  tab += TabVal;

  line = _T("AllowDowngrades=\"no\" IgnoreRemoveFailure=\"no\" AllowSameVersionUpgrades=");
  line += isSameVerAllowed ? _T("\"yes\"") : _T("\"no\"");
  wix(tab, line);

  wix(tab, _T("DowngradeErrorMessage=\"A newer version of [ProductName] is already installed.\""));
  wix(tab, _T("/>"));   wix.crlf();
  tab -= TabVal;

  wix(tab, _T("<MediaTemplate EmbedCab=\"yes\" />"));
  }


void Product::addLicense(int tab) {
String relPath = licPath.prodPath();

  wix(tab, _T("<WixVariable Id=\"WixUILicenseRtf\" Value=\"") + relPath + _T("\" />"));

  if (!isLicenseReq) wix(tab, _T("<Property Id=\"LicenseAccepted\" Value=\"1\" />"));


//  wix(tab, _T("<UI Id=\"My_InstallDir.ui\"><UIRef Id=\"WixUI_InstallDir\" /></UI>"));
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


/*
  <UI Id="My_InstallDir.ui">
    <ui:WixUI Id="WixUI_InstallDir" />
    <UIRef Id="WixUI_ErrorProgressText" />
  </UI>
*/

void Product::installDir(int tab) {
  wix(tab, _T("<Property Id=\"WIXUI_INSTALLDIR\" Value=\""),
                                                        pffDirectories.appDir->wixID, _T("\" />"));
  wix.crlf();
  wix(tab, _T("<UI Id=\"My_InstallDir.ui\">"));
  wix(tab+2, _T("<ui:WixUI Id=\"WixUI_InstallDir\" />"));
  wix(tab+2, _T("<UIRef Id=\"WixUI_ErrorProgressText\" />"));
  wix(tab, _T("</UI>"));
  }



void Product::installerIcons(int tab) {
String line;

  oneInstallerIcon(tab, _T("WixUIDialogBmp"), _T("dialog.bmp"));
  oneInstallerIcon(tab, _T("WixUIBannerBmp"), _T("banner.bmp"));
  }


void Product::oneInstallerIcon(int tab, TCchar* id, TCchar* bmp) {
String line;

  line  = _T("<WixVariable Id=\""); line += id; line += _T("\" ");
  line += _T("Value=\"") + installerIconPath + bmp; line += _T("\" />");

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

  if (dsc) wix(tab, _T("<Property Id=\"ARPPRODUCTICON\" Value=\""), dsc->wixID, _T("\" />"));
  }


void Product::saveData(Archive& ar) {
String k;

  ar << aClrTabs << aSetTab(3) << aSetTab(30);

  ar << _T("Product: ") << productName << aCrlf;
  ar << aTab << _T("Company:")           << aTab   << company           << aCrlf;
  ar << aTab << _T("Wix Name:")          << aTab   << wixName           << aCrlf;
  ar << aTab << _T("Wix Version:")       << aTab   << wixVersion        << aCrlf;
  ar << aTab << _T("Wix Path:")          << aTab   << wixPath           << aCrlf;
  ar << aTab << _T("upgrade Code:")      << aTab   << upgradeCode       << aCrlf;
  ar << aTab << _T("Icon ID:")           << aTab   << iconID            << aCrlf;
  ar << aTab << _T("Installer Icon ID:") << aTab   << installerIconPath << aCrlf;
  ar << aTab << _T("isWin10:")           << aTab   << isWin10           << aCrlf;
  ar << aTab << _T("isWin7:")            << aTab   << isWin7            << aCrlf;
  ar << aTab << _T("isWinXP:")           << aTab   << isWinXP           << aCrlf;
//  k = isSameVerAllowed;
  ar << aTab << _T("isSameVerAllowed:") << aTab << isSameVerAllowed << aCrlf;
  k = isLicenseReq;
  ar << aTab << _T("isLicenseReq:") << aTab << isLicenseReq << aCrlf;
  ar << aTab << _T("License Path:") << aTab; licPath.saveData(ar);   ar << aCrlf;
  }

