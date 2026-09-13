// The one object that describes the current Solution Path


#include "pch.h"
#include "Solution.h"
#include "Archive.h"
#include "DefaultPath.h"
//#include "GetPathDlg.h"
#include "filename.h"
#include "IniData.h"
//#include "IniFileEx.h"
#include "PathDlg.h"
#include "SolutionPathDlg.h"
#include "WixApp.h"
#include "WixAppDlg.h"
#include "WixData.h"
#include "WixUtilities.h"

static TCchar* SolutionSection = _T("SolutionInfo");
static TCchar* SolPathKey      = _T("Path");
static TCchar* SolNameKey      = _T("Name");
static TCchar* SolVersion      = _T("Version");

static TCchar* VarSolution     = _T("$(var.SolutionDir)");

       TCchar* RootPathKey     = _T("RootPath");


bool Solution::newProject(bool isNew) {
String          s;
String          r;
String          t;
SolutionPathDlg dlg;
int             last;

  if (!isNew && rootUnits.isEmpty()) readRootPath(s);
  else                               s = rootUnits;

  last = s.length() - 1;

  dlg.rootPath = last >= 0 && s[last] == _T('\\') ? s.substr(0, last) : s;

  if (!isNew) dlg.solutionPath = t = solUnits;

  loop {
    switch (dlg.DoModal()) {
      case IDOK             : s = dlg.solutionPath;   r = dlg.rootPath;  s.trim();  r.trim();
                              if (s.isEmpty() || r.isEmpty()) continue;
                              break;
      case IDC_AbsolutePaths:
      case IDCANCEL         : return false;
      default               : return false;
      }
    break;
    }

  name = getMainName(s);   solUnits = t = getPath(s);

  last = r.length() - 1;   if (last >= 0 && r[last] != _T('\\')) r +=  _T('\\');

  rootUnits = r;   baseUnits = rootUnits.path(rootUnits.nData()-1);   saveRootPath(r);

  doc()->setTitle(r);   return true;
  }


bool Solution::readRootPath(String& path) {return iniData.read(RootPathKey, path, _T(""));}


void Solution::saveRootPath(TCchar* path) {iniData.write(RootPathKey, path);}


void Solution::readWixData() {
String rootPath;
String path;
String version;
uint   pos;

  wxd.read(SolutionSection, RootPathKey, rootPath);

  if (!PathFileExists(rootPath)) {
    getRootPath(rootPath);
    if (!rootPath.isEmpty()) wxd.write(SolutionSection, RootPathKey, rootPath);
    }

  if (!rootPath.isEmpty()) {rootUnits = rootPath; baseUnits = rootUnits.path(rootUnits.nData()-1);}

  wxd.read(SolutionSection, SolPathKey,  path);

  if (!PathFileExists(path)) {
    getSolutionPath(path);

    if (!path.isEmpty()) wxd.write(SolutionSection, SolPathKey, path);
    }

  if (!path.isEmpty()) solUnits = path;

  doc()->setTitle(path);

  wxd.read(SolutionSection, SolNameKey,  name);
  wxd.read(SolutionSection, SolVersion, version);    ver = version.stod(pos);

  if (solUnits.isEmpty() || name.isEmpty()) newProject(false);
  }


bool Solution::getRootPath(String& path)
                        {PathDlg dlg(_T("Project Root Directory"), path); return dlg.folder(path);}


void Solution::getSolutionPath(TCchar* fullPath) {
String s = fullPath;
String t;

  if (getSolutionPath(s)) {name = getMainName(s);   solUnits = t = getPath(s);}
  }


bool Solution::getSolutionPath(String& path) {
PathDlg dlg(_T("Solution File"), path, _T("sln"), _T("Solution"), _T("*.sln"));

  return dlg.openFile(path);
  }


void Solution::writeWixData() {
Tchar  buf[32];
String version;
uint   pos;
String rootPath = rootUnits;
String fullPath = solution.solUnits + name + _T(".sln");

  LoadString(0, IDR_WxdVersion, buf, noElements(buf));    version = buf;   ver = version.stod(pos);

  wxd.write(SolutionSection, RootPathKey, rootPath);
  wxd.write(SolutionSection, SolPathKey,  fullPath);
  wxd.write(SolutionSection, SolNameKey,  name);
  wxd.write(SolutionSection, SolVersion,  ver);
  }


void Solution::saveData(Archive& ar) {
String  rootPath = rootUnits;
PUIter  iter(solUnits);
String* s;
int     i;
int     t;

  ar << aClrTabs << aSetRTab(3) << aSetTab(6);

  ar << _T("Root: ") << rootPath << aCrlf;
  ar << _T("Name: ") << name << aCrlf;

  ar << _T("Version: ") << ver << aCrlf;

  ar << _T("Path Units") << aCrlf;

  for (i = 0, s = iter(); s; i++, s = iter++) {
    t = i * 10;   ar << aTab << t << aTab << *s << aCrlf;
    }
  }


