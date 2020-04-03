// Wix Data Class


#include "stdafx.h"
#include "WixData.h"
#include "CopyFile.h"
#include "DefaultPath.h"
#include "Directory.h"
#include "filename.h"
#include "Finish.h"
#include "GetPathDlg.h"
#include "Product.h"
#include "Prolog.h"
#include "Solution.h"
#include "WixApp.h"
#include "WixOut.h"
#include "WixUtilities.h"


static TCchar* IniSection     = _T("Current");
static TCchar* IniPathKey     = _T("WixFullPath");


WixData wixData;


bool WixData::readWixData() {
String path;
TCchar* p;

  p = iniFile.readString(IniSection, IniPathKey, path) ? path.str() : 0;

  if (!getPathDlg(_T("Wix Data File"), p, _T("wxd"), _T("*.wxd"), path)) return false;

  wxd.setPath(path);   iniFile.writeString(IniSection, IniPathKey, getPath(path));

  solution.readWixData();
  product.readWixData();
  feature.readWixData();
  return true;
  }


void WixData::writeWixData(String& wxsPath) {
String wixDataPath = getPath(wxsPath) + product.wixName + _T(".wxd");

  wxd.setPath(wixDataPath);

  clearAllSections();

  solution.writeWixData();

  defaultPath.writeWixData();

  product.writeWixData();

  feature.writeWixData();

  iniFile.writeString(IniSection, IniPathKey, wixDataPath);
  iniFile.writeString(IniSection, _T("WixName"),  product.wixName);
  }


void WixData::clearAllSections() {
Tchar*  p;

  for (p = wxd.startReadSection(); p; p = wxd.nextSection()) wxd.deleteSection(p);
  }


bool WixData::validate() {
bool        rslt      = true;
Component*  app       = feature.findAnApp();
int         wixVerLng = product.wixVersion.length();

  if (!app && !wixVerLng) {

    String msg = _T("There is no App designated and the no version was input.");

    MessageBox(0, msg, _T("WixApp"), MB_OK);   rslt &= false;
    }

  product.identifyIcons();   feature.identifyIconsUsed();

  if (!icons.validate()) rslt &= false;

  if (!solution.empty() && !PathFileExists(String(solution))) {

    String msg = _T("Solution path not found:  ") + solution;

    MessageBox(0, msg, _T("WixApp"), MB_OK);   rslt &= false;
    }

  if (!feature.validate()) rslt &= false;

  return rslt;
  }






void WixData::output() {
String      wxsPath;
String      pathOnly;
String      msg = _T("Output a new Wix Product File: ");
Component*  app = feature.findAnApp();
String      currentGroup;
String      defPath;

  if (iniFile.readString(IniSection, IniPathKey, defPath)) defPath = getPath(defPath);

  defPath += _T("Product");

  if (app) directory.appDir = app->progFile;

  if (getSaveAsPathDlg(_T("Product"), defPath, _T("wxs"), _T("*.wxs"), wxsPath)) {

    backupFile(wxsPath, 10);

    wix.open(wxsPath);

    currentGroup = feature.getCurGroup()->id;

    feature.identifyDirectoriesUsed();

    prepareUninstalls();

    feature.find(currentGroup);

    prolog.output();

    product.output(app, prolog, feature);

    feature.outputFeatureTables(0);

    directory.output();

    feature.outputComponents();

    wix.crlf(); wix.lit(0, _T("</Wix>\n"));

    wix.close();

    wxsPath = getPath(wxsPath);

    copyHelperFile(wxsPath, _T("My_en-us.wxl"));     //    copyHelperFile(wxsPath, "My_InstallDir.wxs");

    writeWixData(wxsPath);

    finalMsg();
    }
  }


void WixData::prepareUninstalls() {
Component* c   = feature.findAnApp();
Group*     grp = 0;
Component* cmp = 0;

  if (c && c->isStartMenu) {
    grp = feature.newGroup();   cmp = grp->newItem();

    grp->id             = _T("Uninstall ") + c->id;
    grp->wixID          = getWixID(grp->id, FeatureExt);
    grp->isUninstall    = true;

    cmp->id             = _T("Uninstall ") + c->id;
    cmp->wixID          = getWixID(cmp->id, _T("cmp"));
    cmp->description    = _T("Uninstalls ") + c->id;
    cmp->target         = _T("[System64Folder]msiexec.exe");
    cmp->arguments      = _T("/x [ProductCode]");
    cmp->progFile       = c->startMenu;
    cmp->startMenu      = c->progFile;
    cmp->icon           = product.ctrlPanel;
    cmp->isStartMenu    = true;
    cmp->isUninstall    = true;
    }
  }


void WixData::copyHelperFile(String& wxsPath, TCchar* fileName) {
String fromFile = theApp.myPath + fileName;
String toFile   = wxsPath       + fileName;

  if (isFilePresent(toFile)) return;

  copyFile(fromFile, toFile);
  }



void WixData::finalMsg() {Finish finish; finish.DoModal();}

