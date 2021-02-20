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
static TCchar* WxdFilePathKey = _T("WxdFullPath");


WixData wixData;


bool WixData::readWixData() {
String path;

  getWxdPath(path);

  if (!getPathDlg(_T("Wix Data File"), path, _T("wxd"), _T("*.wxd"), path)) return false;

  saveWxdPath(path);    wxd.setPath(path);

        solution.readWixData();
     defaultPath.readWixData();
           icons.readWixData();

         product.readWixData();

  pffDirectories.readWixData();
  pmfDirectories.readWixData();

        features.readWixData();
  return true;
  }


void WixData::writeWixData(TCchar* filePath) {
String wixDataPath = filePath;

  iniFile.writeString(IniSection, _T("WixName"),  product.wixName);

  wxd.setPath(wixDataPath);     backupFile(wixDataPath, 10);

  clearAllSections();

        solution.writeWixData();
     defaultPath.writeWixData();
           icons.writeWixData();

         product.writeWixData();

  pffDirectories.writeWixData();
  pmfDirectories.writeWixData();

        features.writeWixData();
  }




void WixData::getWxdPath(String& path) {iniFile.readString(IniSection, WxdFilePathKey, path, _T(""));}


void WixData::saveWxdPath(TCchar* path)
                            {String pth = path;   iniFile.writeString(IniSection, WxdFilePathKey, pth);}


void WixData::clearAllSections() {
Tchar*  p;

  for (p = wxd.startReadSection(); p; p = wxd.nextSection()) wxd.deleteSection(p);
  }


void WixData::newFile(WixDataDlg* dialog) {
String path;

  solution.newFile();

  path = solution;   removeOneDir(path);

  saveWxdPath(path);   newFileNow = true;
  }


void WixData::setDefaults(WixDataDlg* dialog) {
String productName;

  if (!loadingFileNow) {product.storeProduct(*dialog);   productName = product.productName;}

  if (newFileNow && !productName.isEmpty()) {
    product.wixName = productName;   dialog->wixNameEB.SetWindowText(productName);

    features.setDefaults(*dialog);

    newFileNow = false;
    }
  }


void WixData::openFile(WixDataDlg* dialog) {

  loadingFileNow = true;

  if (!readWixData()) return;

   product.loadCB(*dialog);
  features.loadCB(*dialog);

  loadingFileNow = false;
  }



bool WixData::validate(bool rptErrors) {
bool        rslt      = true;
Component*  app       = features.findAnApp();
int         wixVerLng = product.wixVersion.length();

  if (product.wixName.isEmpty() && rptErrors)
                                {messageBox(_T("WixName is empty, please provide one.")); return false;}
  features.markDirs();

  product.mark();   features.markIconsUsed();

  if (!app && !wixVerLng && rptErrors) {

    String msg = _T("There is no App designated and the no version was input.");

    MessageBox(0, msg, _T("WixApp"), MB_OK);   rslt &= false;
    }

  if (!icons.validate(rptErrors)) rslt &= false;

  if (!solution.isEmpty() && !PathFileExists(String(solution)) && rptErrors) {

    String msg = _T("Solution path not found:  ") + solution;

    MessageBox(0, msg, _T("WixApp"), MB_OK);   rslt &= false;
    }

  if (!features.validate(rptErrors)) rslt &= false;

  return rslt;
  }


void WixData::output() {
String      wxsPath;
String      pathOnly;
String      msg = _T("Output a new Wix Product File: ");
Component*  app;
String      currentGroup;
String      defPath;

  getWxdPath(defPath);   defPath = getPath(defPath);   defPath += _T("Product");

  if (getSaveAsPathDlg(_T("Product"), defPath, _T("wxs"), _T("*.wxs"), wxsPath)) {

    app = features.findAnApp();   if (app) pffDirectories.appDir = app->getProgFile();

    backupFile(wxsPath, 10);

    wix.open(wxsPath);

    currentGroup = features.getCurFeature()->id;

    prepareUninstalls();

    features.find(currentGroup);

    prolog.output();

    product.output(features.findAnApp(), prolog, features);

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
    ftr = features.newItem();   cmp = ftr->newItem();       // potential changing of app pointer

    app                 = features.findAnApp();
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


