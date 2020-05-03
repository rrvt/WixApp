// Wix Data Class


#include "stdafx.h"
#include "WixData.h"
#include "CopyFile.h"
#include "DefaultPath.h"
#include "filename.h"
#include "GetPathDlg.h"
#include "Options.h"
#include "PFFdirectories.h"
#include "PMFdirectories.h"
#include "Product.h"
#include "Prolog.h"
#include "Solution.h"
#include "WixApp.h"
#include "WixOut.h"
#include "WixUtilities.h"


static TCchar* IniSection     = _T("Current");
static TCchar* IniPathKey     = _T("WixFullPath");


WixData wixData;


// A new Solution has been found, so update the WixPath on the off chance it is for a new application

void WixData::updatePath(String& s) {String path = getPath(s);   saveWixPath(path);}



bool WixData::readWixData() {
String path;
TCchar* p;

  p = getWixPath(path);

  if (!getPathDlg(_T("Wix Data File"), p, _T("wxd"), _T("*.wxd"), path)) return false;

  saveWixPath(path);

  wxd.setPath(path);

     solution.readWixData();
        icons.readWixData();
  defaultPath.readWixData();

  product.readWixData();

  pffDirectories.readWixData();
  pmfDirectories.readWixData();

  features.readWixData();
  return true;
  }


void WixData::writeWixData() {
String path;
String wixDataPath;

  getWixPath(path);

  wixDataPath = path + product.wixName + _T(".wxd");
  iniFile.writeString(IniSection, _T("WixName"),  product.wixName);

  wxd.setPath(wixDataPath);

  clearAllSections();

     solution.writeWixData();
        icons.writeWixData();
  defaultPath.writeWixData();

  product.writeWixData();

  pffDirectories.writeWixData();
  pmfDirectories.writeWixData();

  features.writeWixData();
  }


TCchar* WixData::getWixPath(String& path) {
  return iniFile.readString(IniSection, IniPathKey, path) ? path.str() : 0;
  }


void WixData::saveWixPath(TCchar* path) {
  iniFile.writeString(IniSection, IniPathKey, getPath(path));
  }


void WixData::clearAllSections() {
Tchar*  p;

  for (p = wxd.startReadSection(); p; p = wxd.nextSection()) wxd.deleteSection(p);
  }


                                                                         //solution.loadEB(*dialog);

void WixData::newFile(WixDataDlg* dialog)
                                    {solution.newFile(); newFileNow = true;}


void WixData::setDefaults(WixDataDlg* dialog) {
String productName;
  product.storeProduct(*dialog);   productName = product.productName;

  if (newFileNow && !productName.isEmpty()) {
    product.wixName = productName;   dialog->wixNameEB.SetWindowText(productName);

    features.setDefaults(*dialog);

    newFileNow = false;
    }
  }


void WixData::openFile(WixDataDlg* dialog) {

  if (!readWixData()) return;

//  solution.loadEB(*dialog);
   product.loadCB( *dialog);
  features.loadCB(*dialog);
  }



bool WixData::validate() {
bool        rslt      = true;
Component*  app       = features.findAnApp();
int         wixVerLng = product.wixVersion.length();

  if (product.wixName.isEmpty()) {messageBox(_T("WixName is empty, please provide one.")); return false;}

  features.markDirs();

  product.markIcon();   features.markIconsUsed();    defaultPath.mark(DefLicensePathKey);

  if (!app && !wixVerLng) {

    String msg = _T("There is no App designated and the no version was input.");

    MessageBox(0, msg, _T("WixApp"), MB_OK);   rslt &= false;
    }

  if (!icons.validate()) rslt &= false;

  if (!solution.isEmpty() && !PathFileExists(String(solution))) {

    String msg = _T("Solution path not found:  ") + solution;

    MessageBox(0, msg, _T("WixApp"), MB_OK);   rslt &= false;
    }

  if (!features.validate()) rslt &= false;

  return rslt;
  }


void WixData::output() {
String      wxsPath;
String      pathOnly;
String      msg = _T("Output a new Wix Product File: ");
Component*  app = features.findAnApp();
String      currentGroup;
String      defPath;

  getWixPath(defPath);   defPath += _T("Product");

  if (getSaveAsPathDlg(_T("Product"), defPath, _T("wxs"), _T("*.wxs"), wxsPath)) {

    saveWixPath(wxsPath);

    if (app) pffDirectories.appDir = app->getProgFile();

    backupFile(wxsPath, 10);

    wix.open(wxsPath);

    currentGroup = features.getCurFeature()->id;

    prepareUninstalls();

    features.find(currentGroup);

    prolog.output();

    product.output(app, prolog, features);

    features.outputFeatureTables(0);

    pffDirectories.begOutput();
    pmfDirectories.output();
    pffDirectories.finOutput();

    features.outputComponents();

    wix.crlf(); wix.lit(0, _T("</Wix>\n"));

    wix.close();

    copyHelperFile(getPath(wxsPath), _T("My_en-us.wxl"));
    }
  }





void WixData::prepareUninstalls() {
Component* app = features.findAnApp();
Feature*   ftr = 0;
Component* cmp = 0;

  if (app && app->isStartMenu) {
    ftr = features.newItem();   cmp = ftr->newItem();

    ftr->id             = _T("Uninstall ") + app->id;
    ftr->wixID          = getWixID(ftr->id, FeatureExt);
    ftr->isUninstall    = true;

    ftr->setDirectories(app->getProgFileID(), app->getStartMenuID());

    cmp->id             = _T("Uninstall ") + app->id;
    cmp->wixID          = getWixID(cmp->id, _T("cmp"));
    cmp->description    = _T("Uninstalls ") + app->id;
    cmp->target         = _T("[System64Folder]msiexec.exe");
    cmp->arguments      = _T("/x [ProductCode]");
    cmp->markDirs(*ftr);
    cmp->setIcon(product.iconID);
    cmp->isStartMenu    = true;
    cmp->isUninstall    = true;
    cmp->isApp          = true;
    }
  }


void WixData::copyHelperFile(String& wxsPath, TCchar* fileName) {
String fromFile = theApp.myPath + fileName;
String toFile   = wxsPath       + fileName;

  if (isFilePresent(toFile)) return;

  copyFile(fromFile, toFile);
  }



//void WixData::finalMsg() {Finish finish; finish.DoModal();}



#if 0

  <Component Id="UninstallWixApp.cmp"  Guid="CE0AF1E2-A4EB-4DFA-B49A-C1DCBECDE3CD"
                                                                              Directory="WixApp.pmf">

    <Shortcut Id               = "UninstallWixApp.shc"
              Name             = "Uninstall WixApp"
              Description      = "Uninstalls WixApp"
              Target           = "[System64Folder]msiexec.exe"
              Arguments        = "/x [ProductCode]"
              Icon             = "WixAppIcon.ico"
              />
    <RemoveFolder Id="WixApp.uni" Directory="WixApp.pmf" On="uninstall"/>
    <RegistryValue Root="HKCU" Key="Software\UninstallWixApp.cmp" Name="installed"
                   Type="integer" Value="1" KeyPath="yes"/>
  </Component>
#endif

