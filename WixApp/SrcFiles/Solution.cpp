// The one object that describes the current Solution Path


#include "stdafx.h"
#include "Solution.h"
#include "GetPathDlg.h"
#include "filename.h"
#include "SolutionPath.h"
#include "WixData.h"
#include "WixUtilities.h"


static TCchar* SolutionSection = _T("SolutionInfo");
static TCchar* SolPathKey      = _T("Path");
static TCchar* SolNameKey      = _T("Name");
static TCchar* SolVersion      = _T("Version");

static TCchar* VarSolution     = _T("$(var.SolutionDir)");


TCchar* explanation =  _T("WixApp will produce a wix file \"Product.wxs\" when the Create Button is ")
                       _T("selected.  The original Product.wxs file is renamed with a date/time ")
                       _T("appendage.  The new Product.wxs may then be compiled.");
TCchar* explanation2 = _T("The first step is to find the solution file for the project as WixApp uses ")
                    _T("the path to xxx.sln as the starting point for paths to each of the components ")
                    _T("to be installed.  ")
                    _T("Alternately, select the Absolute Path button.  Here is the current path to the ")
                    _T("solution file:");

Solution solution;


void Solution::newFile() {
SolutionPath dlg;
String s;

  dlg.explanation  = explanation;
  dlg.Expanation2  = explanation2;
  s = pathUnits;     dlg.solutionPath = s.str();

  switch (dlg.DoModal()) {
    case IDOK     : if (getPathDlg(_T("Solution File"), 0, _T("sln"), _T("*.sln"), s))
                               {name = getMainName(s);   pathUnits = getPath(s);  wixData.updatePath(s);}
                    break;

    case IDCANCEL : clear(); break;
    }
  }


void Solution::readWixData() {
String path;

  wxd.readString(SolutionSection, SolPathKey, path);   pathUnits = path;
  wxd.readString(SolutionSection, SolNameKey, name);
  wxd.readString(SolutionSection, SolVersion, version);

  if (pathUnits.isEmpty() || name.isEmpty()) newFile();
  }


//IDR_WXDversion

void Solution::writeWixData() {
Tchar  buf[32];
String version;

  LoadString(0, IDR_WXDversion, buf, noElements(buf));    version = buf;

  wxd.writeString(SolutionSection, SolPathKey, (String) solution.pathUnits);
  wxd.writeString(SolutionSection, SolNameKey, name);
  wxd.writeString(SolutionSection, SolVersion, version);
  }


//void Solution::loadEB(WixDataDlg& dialog) {dialog.wixNameEB.SetWindowText(name);}


#if 0
void Solution::getRelSolution(String& path, String& varPath) {
String    main;
PathUnits var;
String    s;

  if (pathUnits.isEmpty()) {varPath = path.isEmpty() ? _T("") : path;   return;}

  main = removePath(path);

  var = getPath(path);

  if (!pathUnits.relativePath(var, s)) varPath = path;

  varPath = VarSolution + s + main;
  }
#endif



