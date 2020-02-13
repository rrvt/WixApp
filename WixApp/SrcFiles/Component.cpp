// A Single Component's data


#include "stdafx.h"
#include "Component.h"
#include "DefaultPath.h"
#include "Directory.h"
#include "filename.h"
#include "GetPathDlg.h"
#include "Guid.h"
#include "Product.h"
#include "Resources.h"
#include "Solution.h"
#include "WixUtilities.h"
#include "WixApp.h"


static TCchar* Section        = _T("%s.cmp");
static TCchar* IDkey          = _T("ID");
static TCchar* PathKey        = _T("Path");
static TCchar* GuidKey        = _T("Guid");
static TCchar* DescKey        = _T("Description");
static TCchar* IsStartMenuKey = _T("IsStartMenu");
static TCchar* IsDeskTopKey   = _T("IsDeskTop");
static TCchar* IsVersionKey   = _T("IsVersionAvail");
static TCchar* IsOnPathKey    = _T("IsOnPath");
static TCchar* IsWin7Key      = _T("IsWin7");
static TCchar* IsWinXPKey     = _T("IsWinXP");
static TCchar* IsWin2KKey     = _T("IsWin2K");
static TCchar* ProgKey        = _T("Program");
static TCchar* StartKey       = _T("Start");

static TCchar* ComponentExt   = _T("cmp");

static TCchar* SetEnvSect     = _T("SetEnv");
static TCchar* SetEnvPath     = _T("Path");

static TCchar* CmpPathKey     = _T("Component");



Component::Component() : isStartMenu(false), isDesktop(false),   isVersionAvail(false),
                         isOnPath(false),    isUninstall(false), isUninstallDir(false),
                         regOut(false),      isApp(false),       isWin7(false),
                         isWinXP(false),     isWin2K(false),     iconUsed(false) {defaultPath.get(CmpPathKey);}



void Component::readWixData( ) {
String path;

  section.format(Section,      id.str());

  readOne(IDkey,   id);          wixID = getWixID(id, ComponentExt);
  readOne(PathKey, path);      pathDsc = path;   setApp(path);
  readOne(GuidKey, guid);
  readOne(DescKey, description);
  icon.readWixData(section);
  isStartMenu    = wxd.readInt(section, IsStartMenuKey, isStartMenu)    != 0;
  isDesktop      = wxd.readInt(section, IsDeskTopKey,   isDesktop)      != 0;
  isVersionAvail = wxd.readInt(section, IsVersionKey,   isVersionAvail) != 0;
  isOnPath       = wxd.readInt(section, IsOnPathKey,    isOnPath)       != 0;
  isWin7         = wxd.readInt(section, IsWin7Key,      isWin7)         != 0;
  isWinXP        = wxd.readInt(section, IsWinXPKey,     isWinXP)        != 0;
  isWin2K        = wxd.readInt(section, IsWin2KKey,     isWin2K)        != 0;

  readDir(ProgKey,  progFile,  directory.pff);
  readDir(StartKey, startMenu, directory.pmf);

  if (isVersionAvail && isApp) product.updateVersion(path);
  }


void Component::readOne(TCchar* key, String& v) {
  v = _T("<>");   if (!wxd.readString(section, key, v) || v == _T("<>")) v.clear();
  }


void Component::readDir(TCchar* key, DirDesc& dir, DirStor& stor)
         {DirDesc* dsc = stor.add( dir.readWixData(section, key));   dir.wixID = dsc->wixID;}


void Component::writeWixData() {

  section.format(Section, id.str());

  wxd.writeString(section, IDkey,          id);
  wxd.writeString(section, PathKey,        pathDsc.full());
  wxd.writeString(section, GuidKey,        guid);
  wxd.writeString(section, DescKey,        description);
  icon.writeWixData(section);
  wxd.writeInt(   section, IsStartMenuKey, isStartMenu);
  wxd.writeInt(   section, IsDeskTopKey,   isDesktop);
  wxd.writeInt(   section, IsVersionKey,   isVersionAvail);
  wxd.writeInt(   section, IsOnPathKey,    isOnPath);
  wxd.writeInt(   section, IsWin7Key,      isWin7);
  wxd.writeInt(   section, IsWinXPKey,     isWinXP);
  wxd.writeInt(   section, IsWin2KKey,     isWin2K);

   progFile.writeWixData(section, ProgKey);
  startMenu.writeWixData(section, StartKey);
  }



void Component::loadNew(WixDataDlg& dialog) {
DirDesc* d;

  if (id.empty() && progFile.wixID.empty() && startMenu.wixID.empty()) {
    d = directory.pff.getDefault();   if (d) progFile  = *d;
    d = directory.pmf.getDefault();   if (d) startMenu = *d;
    }

  load(dialog);  dialog.componentCB.SetFocus();
  }


void Component::load(WixDataDlg& dialog) {
String s;
ComboBox& cb = dialog.iconCB;

          dialog.componentCB.setCurSel(id);
               dialog.pathEB.SetReadOnly(false);
               dialog.pathEB.SetWindowText(pathDsc);
               dialog.pathEB.SetReadOnly(true);
        dialog.descriptionEB.SetWindowText(description);
          dialog.startMenuCH.SetCheck(isStartMenu);
            dialog.deskTopCH.SetCheck(isDesktop);
       dialog.versionAvailCH.SetCheck(isVersionAvail);
           dialog.isOnPathCH.SetCheck(isOnPath);
             dialog.isWin7ch.SetCheck(isWin7);
            dialog.isWinXPch.SetCheck(isWinXP);
            dialog.isWin2Kch.SetCheck(isWin2K);



        icons.loadCB(cb);    icons.setCur(icon.id, cb);
           dialog.progFileEB.SetWindowText(progFile.fullPath());
          dialog.startMenuEB.SetWindowText(startMenu.fullPath());

  storeIsVersionAvail(dialog);
  }


void Component::store(WixDataDlg& dialog) {
CString   cs;
String    s;
//IconDesc* d;

     dialog.componentCB.getWindowText(id);   wixID = getWixID(id, ComponentExt);
   dialog.descriptionEB.GetWindowText(cs);   description = cs;

//      dialog.iconCB.getWindowText(s);    d = icons.find(s);  if (d) icon = d;

  isStartMenu    = dialog.startMenuCH.GetCheck()    != 0;
  isDesktop      = dialog.deskTopCH.GetCheck()      != 0;
  isVersionAvail = dialog.versionAvailCH.GetCheck() != 0;
  isOnPath       = dialog.isOnPathCH.GetCheck()     != 0;
  isWin7         = dialog.isWin7ch.GetCheck()       != 0;
  isWinXP        = dialog.isWinXPch.GetCheck()      != 0;
  isWin2K        = dialog.isWin2Kch.GetCheck()      != 0;

  dialog.progFileEB.GetWindowText(cs);   progFile  = *directory.pff.add(String(cs));
  dialog.startMenuEB.GetWindowText(cs);  startMenu = *directory.pmf.add(String(cs));
  }


void Component::updateComponent(WixDataDlg& dialog)
                           {dialog.componentCB.getWindowText(id);  wixID = getWixID(id, ComponentExt);}



void Component::browsePath(WixDataDlg& dialog) {
TCchar* defPath = defaultPath.get(CmpPathKey);
String path;

  getPathDlg(_T("Get Component Directory"),  defPath, _T("*"), _T(""), path);  pathDsc = path;

  defaultPath.add(CmpPathKey, path);

  dialog.pathEB.SetWindowText(pathDsc);

  setApp(path);   storeIsVersionAvail(dialog);
  }


void Component::setApp(String& path) {
ResourceData res(path);
uint         fileType = res.getFileType();
String       ver;


  isApp = res.getVersion(ver) && !ver.empty() && VFT_UNKNOWN < fileType && fileType <= VFT_DRV;
  }


void Component::storeIsVersionAvail(WixDataDlg& dialog) {

  isVersionAvail = dialog.versionAvailCH.GetCheck() != 0;

  if (isVersionAvail && isApp) {product.updateVersion(pathDsc.full());   product.loadVerEB(dialog);}
  }


void Component::storeDescription(WixDataDlg& dialog) {description = getText(dialog.descriptionEB);}


void Component::storeIsStartMenu(WixDataDlg& dialog)
                                                 {isStartMenu  = dialog.startMenuCH.GetCheck()    != 0;}
void Component::storeIsDeskTop(WixDataDlg& dialog) {isDesktop  = dialog.deskTopCH.GetCheck()      != 0;}
void Component::storeIsOnPath(WixDataDlg& dialog)  {isOnPath   = dialog.isOnPathCH.GetCheck()     != 0;}
void Component::storeIsWin7(WixDataDlg& dialog)    {isWin7     = dialog.isWin7ch.GetCheck()       != 0;}
void Component::storeIsWinXP(WixDataDlg& dialog)   {isWinXP    = dialog.isWinXPch.GetCheck()      != 0;}
void Component::storeIsWin2K(WixDataDlg& dialog)   {isWin2K    = dialog.isWin2Kch.GetCheck()      != 0;}


void Component::delData() {
  id.clear();
  section.clear();
  guid.clear();
  wixID.clear();
  pathDsc.clear();
  description.clear();
  target.clear();
  arguments.clear();
  progFile.clear();
  startMenu.clear();
  icon;
  isStartMenu    = false;
  isDesktop      = false;
  isVersionAvail = false;
  isOnPath       = false;

  isUninstallDir = false;
  regOut         = false;
  isApp          = false;
  isWin7         = false;
  isWinXP        = false;
  isWin2K        = false;
  }


void Component::browseIcon(WixDataDlg& dialog) {
ComboBox& cb = dialog.iconCB;
String    id = icon.id;

  icons.getFromList(icon);

  icon.browse();

  icons.updateList(icon);

  icons.loadCB(cb);  id = icon.id;

  if (!id.isEmpty()) icons.setCur(id, cb);
  else               cb.SetWindowText(_T(""));
  }


void Component::updateIcon(WixDataDlg& dialog) {
String    id;
IconDesc* p;

  if (dialog.iconCB.getWindowText(id)) {
    p = icons.find(id); if (p) icon = *p;
    }
  }



void Component::storeProgFileName(WixDataDlg& dialog) {
  progFile = *directory.pff.add(getText(dialog.progFileEB));
  }

void Component::storeMenuName(WixDataDlg& dialog)
                                        {startMenu = *directory.pmf.add(getText(dialog.startMenuEB));}


void Component::identifyIconUsed() {
IconDesc* ic = icons.find(icon.id);

  if (ic) ic->inUse = true;
  }


void Component::identifyDirectoriesUsed() {
  directory.pff.identifyDir(progFile.parent);   directory.pff.identifyDir(progFile.id);
  directory.pmf.identifyDir(startMenu.parent);  directory.pmf.identifyDir(startMenu.id);
  }



void Component::outputSetPath(int tab, bool& crlfOut) {
String installActionID;
String uninstallActionID;
String setEnvPath;
String line;

  if (!isOnPath) return;

  installActionID   = getWixID(id, _T("pth"));
  uninstallActionID = getWixID(id, _T("rmp"));
  setEnvPath        = getSetEnvPath();

  if (setEnvPath.empty()) {
    TCchar* msg = _T("Please install Bob -- K6RWY's SetEnv as it is needed to execute the installer.");
    MessageBox(0, msg, _T("WixApp"), MB_OK);   return;
    }

  if (!crlfOut) {
    wix.crlf(); crlfOut = true;
    wix.lit(tab, _T("<!-- Custom Action to set the system (Machine) Path Variable -->\n\n"));
    wix.out(tab, _T("<Binary Id=\"SetEnvExe\" SourceFile=\""), setEnvPath, _T("\" />")); wix.crlf();
    }

  line = _T("<CustomAction Id=\"") + installActionID + _T("\" BinaryKey=\"SetEnvExe\" Impersonate=\"no\" ");
  line += _T("Execute=\"deferred\" ");
  line += _T("ExeCommand='-a Path \" [") + progFile.wixID + _T("] \"'   Return=\"check\"/>\n");
  wix.stg(tab, line);

  line = _T("<CustomAction Id=\"") + uninstallActionID + _T("\" BinaryKey=\"SetEnvExe\" Impersonate=\"no\" ");
  line += _T("Execute=\"deferred\" ");
  line += _T("ExeCommand='-d Path \" [") + progFile.wixID + _T("] \"'   Return=\"check\"/>\n");
  wix.stg(tab, line); wix.crlf();

  wix.lit(tab, _T("<InstallExecuteSequence>\n"));

  line = _T("<Custom Action=\"") + installActionID + _T("\" Before=\"InstallFinalize\">\n");
  wix.stg(tab+1, line);
  wix.lit(tab+2, _T("(UPGRADINGPRODUCTCODE) OR NOT (REMOVE=\"ALL\")\n"));
  wix.lit(tab+1, _T("</Custom>\n"));

  line = _T("<Custom Action=\"") + uninstallActionID + _T("\" Before=\"") + installActionID + _T("\">\n");
  wix.stg(tab+1, line);
  wix.lit(tab+2, _T("(NOT UPGRADINGPRODUCTCODE) AND (REMOVE=\"ALL\")\n"));
  wix.lit(tab+1, _T("</Custom>\n"));

  wix.lit(tab, _T("</InstallExecuteSequence>\n"));
  }


String Component::getSetEnvPath() {
String s;
String path;
String t;

  if (!iniFile.readString(SetEnvSect, SetEnvPath, s)) {
    s = findExeFile(_T("SetEnv.exe"));
    iniFile.writeString(SetEnvSect, SetEnvPath, s);
    }

  return s;
  }


bool Component::validate() {
struct _stat buffer;

  if (_tstat(pathDsc.full(), &buffer) != 0) {

    String msg = _T("File not found: ") + pathDsc.full();

    MessageBox(0, msg, _T("WixApp"), MB_OK);   return false;
    }

  return true;
  }


void Component::output(int tab) {
String line;
String relPath = pathDsc.relative();
int    lower;
int    upper;

  regOut = pathDsc.empty();

//  solution.getRelSolution(pathDsc.full(), relPath);

  if (guid.empty()) getGuid(guid);

  wix.crlf();

  line  = _T("<Component Id=\"") + wixID + _T("\"  Guid=\"") + guid + _T("\" ");
  line += _T("Directory=\"") + progFile.wixID + _T("\">\n");

  wix.stg(tab, line);

  line = _T("<File Id=\"") + wixID + _T("\" Source=\"");
  wix.out(tab+1, line, relPath, _T("\" KeyPath=\"yes\"/>"));

  if (isStartMenu) outputStartMenu(tab+1);

  if (isDesktop)   outputDesktop(tab+1);

  if (isUninstall) directory.outputRemoves(tab+1);           //|| isUninstallDir

  if (regOut)      outputRegistry(tab+1);

  if (isWin7 || isWinXP || isWin2K) {
    lower = isWin2K ? _WIN32_WINNT_WIN2K : isWinXP ? _WIN32_WINNT_WINXP :
                                                                        isWin7  ? _WIN32_WINNT_WIN7  : 0;
    upper = isWin7  ? 0                  : isWinXP ? _WIN32_WINNT_WINXP :
                                                                        isWin2K ? _WIN32_WINNT_WIN2K : 0;
    outputCondition(tab + 1, lower, upper);
    }

  wix.lit(tab, _T("</Component>\n"));
  }


void Component::outputStartMenu(int tab) {
int    tb = tab + 5;
String line;

  wix.crlf();

  wix.out(tab, _T("<Shortcut Id               = \""), removeBlnks(id), _T(".shc\""));

  wix.out(tb,  _T("Name             = \""), id,                       _T("\""));
  wix.out(tb,  _T("Description      = \""), description,              _T("\""));

  if (target.empty())
    wix.out(tb, _T("Advertise        = \""), String("yes"),           _T("\""));

  if (!isUninstall)
    wix.out(tb, _T("Directory        = \""), startMenu.wixID,         _T("\""));

  wix.out(tb,   _T("Target           = \""), target,                  _T("\""));
  wix.out(tb,   _T("Arguments        = \""), arguments,               _T("\""));
  if (!isUninstall)
    wix.out(tb, _T("WorkingDirectory = \""), progFile.wixID,          _T("\""));
  wix.out(tb,   _T("Icon             = \""), removeBlnks(icon.wixID), _T("\""));

  wix.lit(tb, _T("/>\n"));
  }



void Component::outputDesktop(int tab) {
int    tb = tab + 5;
String line;

  wix.crlf();

  wix.out(tab, _T("<Shortcut Id               = \""), removeBlnks(id), _T(".dsk\""));

  wix.out(tb, _T("Name             = \""), id,                      _T("\""));
  wix.out(tb, _T("Description      = \""), description,             _T("\""));
  wix.out(tb, _T("Advertise        = \""), String("yes"),           _T("\""));
  wix.out(tb, _T("Directory        = \""), String(DeskTopDir),      _T("\""));
  wix.out(tb, _T("WorkingDirectory = \""), progFile.wixID,          _T("\""));
  wix.out(tb, _T("Icon             = \""), removeBlnks(icon.wixID), _T("\""));

  wix.lit(tb, _T("/>\n"));
  }




void Component::outputRegistry( int tab) {

  wix.out(tab, _T("<RegistryValue Root=\"HKCU\" Key=\"Software\\"), wixID, _T("\" Name=\"installed\""));
  wix.out(tab+7, _T(" Type=\"integer\" Value=\""), String("1"), _T("\" KeyPath=\"yes\"/>"));
  }


// There are three cases
//  -- One OS
//  -- OS1 to OS2
//  -- OS to infinity
#if 0
<Condition>
  <![CDATA[Installed OR VersionNT >= 501 AND VersionNT < 601]]>         <!-- Win XP, Vista -->
</Condition>
#endif

void Component::outputCondition(int tab, int firstCond, int lastCond) {
String line;

  if (!firstCond) return;

  wix.lit(tab, _T("<Condition>\n"));

  if (firstCond == lastCond) {
    line.format(_T("<![CDATA[Installed OR VersionNT = %x]]>\n"), firstCond);
    wix.stg(tab+1, line);
    }

  else if (firstCond && lastCond) {
    line.format(_T("<![CDATA[Installed OR VersionNT >= %x AND VersionNT <= %x]]>\n"), firstCond, lastCond);
    wix.stg(tab+1, line);
    }

  else {
    line.format(_T("<![CDATA[Installed OR VersionNT >= %x]]>\n"), firstCond);
    wix.stg(tab+1, line);
    }

  wix.lit(tab, _T("</Condition>\n"));
  }



//  void outputComponents(FileIO& file);


void Component::copyObj(Component& c) {

  id             = c.id;
  wixID          = c.wixID;

  section        = c.section;
  guid           = c.guid;

  pathDsc        = c.pathDsc;
  description    = c.description;
  target         = c.target;
  arguments      = c.arguments;
  progFile       = c.progFile;
  startMenu      = c.startMenu;
  icon           = c.icon;
  isStartMenu    = c.isStartMenu;
  isDesktop      = c.isDesktop;
  isVersionAvail = c.isVersionAvail;
  isOnPath       = c.isOnPath;
  isWin7         = c.isWin7;
  isWinXP        = c.isWinXP;
  isWin2K        = c.isWin2K;

  isUninstallDir = c.isUninstallDir;
  regOut         = c.regOut;
  isApp          = c.isApp;
  }



