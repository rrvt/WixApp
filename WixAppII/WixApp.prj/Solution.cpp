// The one object that describes the current Solution Path


#include "pch.h"
#include "Solution.h"
#include "DefaultPath.h"
#include "GetPathDlg.h"
#include "filename.h"
#include "IniFile.h"
#include "SolutionPathDlg.h"
#include "WixData.h"
#include "WixUtilities.h"

static TCchar* SolutionSection = _T("SolutionInfo");
static TCchar* SolPathKey      = _T("Path");
static TCchar* SolNameKey      = _T("Name");
static TCchar* SolVersion      = _T("Version");

static TCchar* VarSolution     = _T("$(var.SolutionDir)");

       TCchar* IniSection      = _T("Current");
       TCchar* RootPathKey     = _T("RootPath");


Solution solution;


bool Solution::newProject(bool isNew) {
String          s;
String          r;
String          t;
SolutionPathDlg dlg;
int             last;

  if (!isNew && rootPath.isEmpty()) readRootPath(s);   else s = rootPath;

  last = s.length() - 1;

  dlg.rootPath = last >= 0 && s[last] == _T('\\') ? s.substr(0, last) : s;

  if (!isNew) dlg.solutionPath = t = pathUnits;

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

  name = getMainName(s);   pathUnits = t = getPath(s);

  rootPath = r;   last = rootPath.length() - 1;

  if (rootPath[last] != _T('\\')) rootPath +=  _T('\\');

  saveRootPath(rootPath);   return true;
  }


bool Solution::readRootPath(String& path)
                                      {return iniFile.read(IniSection, RootPathKey, path, _T(""));}


void Solution::saveRootPath(TCchar* path) {iniFile.write(IniSection, RootPathKey, path);}


void Solution::readWixData() {
String path;
String version;
uint   pos;

  wxd.read(SolutionSection, RootPathKey, rootPath);
  wxd.read(SolutionSection, SolPathKey,  path);
  wxd.read(SolutionSection, SolNameKey,  name);

  if (!PathFileExists(rootPath)) getRootPath(rootPath);

  if (!PathFileExists(path)) getSolutionPath(path);
  else                       pathUnits = path;

  wxd.read(SolutionSection, SolVersion, version);    ver = version.stod(pos);

  if (pathUnits.isEmpty() || name.isEmpty()) newProject(false);
  }


bool Solution::getRootPath(String& path)
                                    {return getDirPathDlg(_T("Project Root Directory"), path);}


void Solution::getSolutionPath(TCchar* fullPath) {
String s = fullPath;
String t;

  if (getSolutionPath(s)) {name = getMainName(s);   pathUnits = t = getPath(s);}
  }


bool Solution::getSolutionPath(String& path) {
PathDlgDsc dsc(_T("Solution File"), path, _T("sln"), _T("*.sln"));  return getOpenDlg(dsc, path);
  }


//IDR_WxdVersion

void Solution::writeWixData() {
Tchar  buf[32];
String version;
uint   pos;
String fullPath = solution.pathUnits + name + _T(".sln");

  LoadString(0, IDR_WxdVersion, buf, noElements(buf));    version = buf;   ver = version.stod(pos);

  wxd.write(SolutionSection, RootPathKey, rootPath);
  wxd.write(SolutionSection, SolPathKey,  fullPath);
  wxd.write(SolutionSection, SolNameKey,  name);
  wxd.write(SolutionSection, SolVersion,  ver);
  }




