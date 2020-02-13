// The one object that describes the current Solution Path


#include "stdafx.h"
#include "Solution.h"
#include "GetPathDlg.h"
#include "filename.h"
#include "PathUnits.h"
#include "SolutionPath.h"
#include "WixUtilities.h"


static TCchar* SolutionSection = _T("SolutionInfo");
static TCchar* SolPathKey      = _T("Path");
static TCchar* SolNameKey      = _T("Name");

static TCchar* VarSolution     = _T("$(var.SolutionDir)");


TCchar* explanation =  _T("WixApp will produce a wix file \"Product.wxT\" which may later be ")
                       _T("changed to \"Product.wxs\" and compiled normally to produce an installer ")
                       _T("for one or more files.  It will only produce a simple installer with just ")
                       _T("a few attributes.");
TCchar* explanation2 = _T("The first step is to find the solution file for the project as WixApp uses ")
                    _T("the path to xxx.sln as the starting point for paths to each of the components ")
                    _T("to be installed.  The next step is find and select the solution file.")
                    _T("Alternately, select the Absolute Path button.  Here is the current path to the ")
                    _T("solution file:");

Solution solution;


void Solution::newFile() {
SolutionPath dlg;
String s;

  dlg.explanation  = explanation;
  dlg.Expanation2  = explanation2;
  s = pathUnits;     dlg.solutionPath = s.str();

  if (dlg.DoModal() == IDOK) {

    if (getPathDlg(_T("Solution File"), 0, _T("sln"), _T("*.sln"), s))
                                                      {name = getMainName(s);   pathUnits = getPath(s);}
    }
  }


void Solution::readWixData() {
String path;

  wxd.readString(SolutionSection, SolPathKey, path);   pathUnits = path;
  wxd.readString(SolutionSection, SolNameKey, name);

  if (pathUnits.empty() || name.empty()) newFile();
  }


void Solution::writeWixData() {
  wxd.writeString(SolutionSection, SolPathKey, (String) solution.pathUnits);
  wxd.writeString(SolutionSection, SolNameKey, solution.name);
  }


void Solution::loadEB(WixDataDlg& dialog) {dialog.wixNameEB.SetWindowText(name);}


#if 0
void Solution::getRelSolution(String& path, String& varPath) {
String    main;
PathUnits var;
String    s;

  if (pathUnits.empty()) {varPath = path.empty() ? _T("") : path;   return;}

  main = removePath(path);

  var = getPath(path);

  if (!pathUnits.relativePath(var, s)) varPath = path;

  varPath = VarSolution + s + main;
  }
#endif



