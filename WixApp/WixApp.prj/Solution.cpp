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
String s;                                                                 //wixData.updatePath(s);

  dlg.explanation  = explanation;
  dlg.Expanation2  = explanation2;
  s = pathUnits;     dlg.solutionPath = s;  //.str()

  if (dlg.DoModal() == IDOK) getSolutionPath();
  }


void Solution::readWixData() {
String path;

  wxd.readString(SolutionSection, SolPathKey, path);
  wxd.readString(SolutionSection, SolNameKey, name);

  if (!PathFileExists(path)) getSolutionPath(path);
  else                       pathUnits = path;

  wxd.readString(SolutionSection, SolVersion, version);

  if (pathUnits.isEmpty() || name.isEmpty()) newFile();
  }


void Solution::getSolutionPath(TCchar* fullPath) {
String s;

  if (getPathDlg(_T("Solution File"), fullPath, _T("sln"), _T("*.sln"), s))
                                                       {name = getMainName(s);   pathUnits = getPath(s);}
  }


//IDR_WXDversion

void Solution::writeWixData() {
Tchar  buf[32];
String version;
String fullPath = solution.pathUnits + name + _T(".sln");

  LoadString(0, IDR_WXDversion, buf, noElements(buf));    version = buf;

  wxd.writeString(SolutionSection, SolPathKey, fullPath);
  wxd.writeString(SolutionSection, SolNameKey, name);
  wxd.writeString(SolutionSection, SolVersion, version);
  }


