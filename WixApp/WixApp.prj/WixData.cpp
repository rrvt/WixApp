// Wix Data Class


#include "pch.h"
#include "WixData.h"
#include "CopyFile.h"
#include "DefaultPath.h"
#include "filename.h"
#include "GetPathDlg.h"
#include "WixOptsDlg.h"
#include "PFFdirectories.h"
#include "PMFdirectories.h"
#include "Product.h"
#include "Solution.h"
#include "WixApp.h"
#include "WixOut.h"
#include "WixUtilities.h"


static TCchar* IniPathKey     = _T("WixFullPath");
static TCchar* WxdFilePathKey = _T("WxdFullPath");

static TCchar* WxdDataFile    = _T("Wix Data File");
static TCchar* WxdDefExt      = _T("wxd");
static TCchar* WxdExtPat      = _T("*.wxd");


WixData wixData;


bool WixData::newProject(WixAppDlg* dialog) {

  if (!solution.newProject(true)) return false;

  delWxdPath();  isNewProject = true;   return true;
  }


void WixData::openProject(WixAppDlg* dialog) {

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

  defaultPath.clearMarks();
  icons.clearMarks();
  pffDirectories.clearMarks();
  pmfDirectories.clearMarks();

  features.markDirs();

  product.mark();

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


void WixData::outputProduct() {
String     wxsPath;
String     pathOnly;
String     msg = _T("Output a new Wix Product File: ");
Component* app;
DirDesc*   appDir;
String     currentGroup;
String     defPath;
PathDlgDsc dsc;
int        tab;

  defPath = readWxdPath(defPath) ? getPath(defPath) : solution.getRootPath();

  defPath += _T("Product");

  dsc(_T("Product"), defPath, _T("wxs"), _T("*.wxs"));

  if (!getSaveAsPathDlg(dsc, wxsPath)) return;

  app = features.findAnApp();

  if (app) {pffDirectories.appDir = appDir = app->getProgFile();   appDir->wixID.upperCase();}

  backupFile(wxsPath, 10);

  wix.open(wxsPath);

  currentGroup = features.getCurFeature()->id;

  prepareUninstalls();

  features.find(currentGroup);

  wix(0, _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?>"));
  wix(0, _T("<Wix xmlns=\"http://schemas.microsoft.com/wix/2006/wi\">"));

  product.output(features.findAnApp(), features);   wix.crlf();

  features.outputFeatureTables(0);                  wix.crlf();

  tab = 0;
  pffDirectories.begOutput(tab);
  pmfDirectories.output(tab);     wix.crlf();
  pffDirectories.finOutput(tab);

  tab = 0;
  features.outputComponents(tab);

  wix(tab, _T("</Wix>"));   wix.close();

  copyHelperFile(getPath(wxsPath), _T("My_en-us.wxl"));
  }


void WixData::outputWxd() {
PathDlgDsc dsc;
String     path;

  if (!readWxdPath(path)) path = solution.getRootPath() + solution.name;

  dsc(WxdDataFile, path, WxdDefExt, WxdExtPat);    if (!getSaveAsPathDlg(dsc, path)) return;

  saveWxdPath(path);     validate(false);

  wxd.setPath(path);     backupFile(path, 10);

  clearAllSections();

        solution.writeWixData();
     defaultPath.writeWixData();
           icons.writeWixData();

         product.writeWixData();

  pffDirectories.writeWixData();
  pmfDirectories.writeWixData();

        features.writeWixData();
  }


bool WixData::readWixData() {
PathDlgDsc dsc;
String     path;

  if (!readWxdPath(path)) path = solution.getRootPath();

  dsc(WxdDataFile, path, WxdDefExt, WxdExtPat);    if (!getOpenDlg(dsc, path)) return false;

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



bool WixData::readWxdPath(String& path)
                                  {return iniFile.read(IniSection, WxdFilePathKey, path, _T(""));}


void WixData::saveWxdPath(TCchar* path) {iniFile.write(IniSection, WxdFilePathKey, path);}


void WixData::delWxdPath() {iniFile.deleteString(IniSection, WxdFilePathKey);}


void WixData::clearAllSections() {
IniSectIter iter(wxd);
TCchar*     section;

  for (section = iter(); section; section = iter++) wxd.deleteSection(section);
  }


void WixData::setDefaults(WixAppDlg* dialog) {
String productName;

  if (!loadingFileNow) {product.storeProduct(*dialog);   productName = product.productName;}

  if (isNewProject && !productName.isEmpty()) {
    product.wixName = productName;   dialog->wixNameEB.SetWindowText(productName);

    features.setDefaults(*dialog);

    isNewProject = false;
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


void WixData::copyHelperFile(TCchar* wxsPath, TCchar* fileName) {
String fromFile = theApp.myPath + fileName;
String toFile   = wxsPath;      toFile += fileName;

  if (isFilePresent(toFile)) return;

  copyFile(fromFile, toFile);
  }




