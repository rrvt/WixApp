// A Single Component's data


#include "pch.h"
#include "Component.h"
#include "Feature.h"
#include "filename.h"
#include "Guid.h"
#include "IniFileEx.h"
#include "PFFdirectories.h"
#include "PMFdirectories.h"
#include "Product.h"
#include "ResourceData.h"
#include "Solution.h"
#include "WixUtilities.h"
#include "WixApp.h"


static TCchar*    Section         = _T("%s.cmp");
static TCchar*    IDkey           = _T("ID");
static TCchar*    PathKey         = _T("Path");
static TCchar*    GuidKey         = _T("Guid");
static TCchar*    DescKey         = _T("Description");
static TCchar*    IconKey         = _T("IconID");
static TCchar*    IsStartMenuKey  = _T("IsStartMenu");
static TCchar*    IsDeskTopKey    = _T("IsDeskTop");
static TCchar*    IsVersionKey    = _T("IsVersionAvail");
static TCchar*    IsOnPathKey     = _T("IsOnPath");
static TCchar*    IsStartupAppKey = _T("IsStartupApp");

static TCchar*    ProgKey         = _T("Program");
static TCchar*    StartKey        = _T("Start");

static TCchar*    ComponentExt    = _T("cmp");

static TCchar*    SetEnvSect      = _T("SetEnv");
static TCchar*    SetEnvPath      = _T("Path");

static TCchar*    CmpPathKey      = _T("Component");

static KeyedPathDsc  browseDsc = {_T("Cmp"), _T("Component File"), _T(""), _T("*"), _T("")};


Component::Component() : isStartMenu(false), isDesktop(false),   isVersionAvail(false),
                         isStartupApp(false),
                         isOnPath(false),    isUninstall(false), isUninstallDir(false),
                         isApp(false),       iconUsed(false) { }


void Component::readWixData(String& ftrID) {
String path;

  section.format(Section,      id.str());

  readOne(IDkey,   id);          wixID = getWixID(id, ComponentExt);

  browseDsc.dfltKey = ftrID;   relPath.readWixData(section, PathKey, browseDsc);

  path = relPath.localPath();   setApp(path);
  readOne(GuidKey, guid);
  readOne(DescKey, description);
  readOne(IconKey, iconID);

  isStartMenu    = wxd.readInt(section, IsStartMenuKey,  0) != 0;
  isDesktop      = wxd.readInt(section, IsDeskTopKey,    0) != 0;
  isVersionAvail = wxd.readInt(section, IsVersionKey,    0) != 0;
  isOnPath       = wxd.readInt(section, IsOnPathKey,     0) != 0;
  isStartupApp   = wxd.readInt(section, IsStartupAppKey, 0) != 0;

  if (isVersionAvail && isApp) product.updateVersion(path);
  }


void Component::readOne(TCchar* key, String& v) {
  v = _T("<>");   if (!wxd.readString(section, key, v) || v == _T("<>")) v.clear();
  }



void Component::readWixData2(String& ftrID, String& cmpSection) {
String path;

  wixID = getWixID(id, ComponentExt);

  browseDsc.dfltKey = ftrID; relPath.readWixData(cmpSection, PathKey, browseDsc);

  guid.clear();        wxd.readString(cmpSection, GuidKey, guid);
  description.clear(); wxd.readString(cmpSection, DescKey, description);
  iconID.clear();      wxd.readString(cmpSection, IconKey, iconID);

  isStartMenu    = wxd.readInt(cmpSection, IsStartMenuKey,  0) != 0;
  isDesktop      = wxd.readInt(cmpSection, IsDeskTopKey,    0) != 0;
  isVersionAvail = wxd.readInt(cmpSection, IsVersionKey,    0) != 0;
  isOnPath       = wxd.readInt(cmpSection, IsOnPathKey,     0) != 0;
  isStartupApp   = wxd.readInt(cmpSection, IsStartupAppKey, 0) != 0;

  path = relPath.localPath();   setApp(path);
  if (isVersionAvail && isApp) product.updateVersion(path);
  }


void Component::writeWixData2(String& cmpSection) {

                              wxd.writeString(cmpSection, IDkey,          id);
  relPath.writeWixData(cmpSection, PathKey);
  if (!guid.isEmpty())        wxd.writeString(cmpSection, GuidKey,         guid);
  if (!description.isEmpty()) wxd.writeString(cmpSection, DescKey,         description);
  if (!iconID.isEmpty())      wxd.writeString(cmpSection, IconKey,         iconID);
  if (isStartMenu)            wxd.writeInt(   cmpSection, IsStartMenuKey,  isStartMenu);
  if (isDesktop)              wxd.writeInt(   cmpSection, IsDeskTopKey,    isDesktop);
  if (isVersionAvail)         wxd.writeInt(   cmpSection, IsVersionKey,    isVersionAvail);
  if (isOnPath)               wxd.writeInt(   cmpSection, IsOnPathKey,     isOnPath);
  if (isStartupApp)           wxd.writeInt(   cmpSection, IsStartupAppKey, isStartupApp);
  }



void Component::writeWixData() {

  section.format(Section, id.str());

  wxd.writeString(section, IDkey,          id);
  relPath.writeWixData(section, PathKey);
  wxd.writeString(section, GuidKey,          guid);
  wxd.writeString(section, DescKey,          description);
  wxd.writeString(section, IconKey,          iconID);
  wxd.writeInt(   section, IsStartMenuKey,   isStartMenu);
  wxd.writeInt(   section, IsDeskTopKey,     isDesktop);
  wxd.writeInt(   section, IsVersionKey,     isVersionAvail);
  wxd.writeInt(   section, IsOnPathKey,      isOnPath);
  wxd.writeInt(   section, IsStartupAppKey,  isStartupApp);
  }


void Component::loadNew(WixAppDlg& dialog) {load(dialog);  dialog.componentCB.setFocus();}


void Component::load(WixAppDlg& dialog) {
String s;
ComboBox& cb = dialog.iconCB;

    dialog.componentCB.setCurSel(id);
         dialog.pathEB.SetReadOnly(false);
         dialog.pathEB.SetWindowText(relPath);
         dialog.pathEB.SetReadOnly(true);
  dialog.descriptionEB.SetWindowText(description);
    dialog.isStartMenu.SetCheck(isStartMenu);
      dialog.isDeskTop.SetCheck(isDesktop);
 dialog.isVersionAvail.SetCheck(isVersionAvail);
     dialog.isOnPathCH.SetCheck(isOnPath);
   dialog.isStartupApp.SetCheck(isStartupApp);

  icons.loadCB(cb);
  icons.setCur(iconID, cb);

  storeIsVersionAvail(dialog);
  }


void Component::store(WixAppDlg& dialog) {
CString cs;
String  s;

     dialog.componentCB.getWindowText(id);   wixID = getWixID(id, ComponentExt);
   dialog.descriptionEB.GetWindowText(cs);   description = cs;

  isStartMenu    = dialog.isStartMenu.GetCheck()    != 0;
  isDesktop      = dialog.isDeskTop.GetCheck()      != 0;
  isVersionAvail = dialog.isVersionAvail.GetCheck() != 0;
  isOnPath       = dialog.isOnPathCH.GetCheck()     != 0;
  isStartupApp   = dialog.isStartupApp.GetCheck()   != 0;
  }


DirDesc* Component::getProgFile()  {return pffDirectories.find(progFileID);}
DirDesc* Component::getStartMenu() {return pmfDirectories.find(startMenuID);}


void Component::browsePath(String& ftrID, WixAppDlg& dialog) {
String path;

  browseDsc.dfltKey = ftrID; relPath.getPath(browseDsc);

  dialog.pathEB.SetWindowText(relPath);

  path = relPath.localPath();   setApp(path);   storeIsVersionAvail(dialog);
  }


void Component::setApp(String& path) {
ResourceData res(path);
uint         fileType = res.getFileType();
String       ver;

  isApp = res.getVersion(ver) && !ver.isEmpty() && VFT_UNKNOWN < fileType && fileType <= VFT_DRV;
  }


void Component::storeIsVersionAvail(WixAppDlg& dialog) {
String path;

  isVersionAvail = dialog.isVersionAvail.GetCheck() != 0;

  if (isVersionAvail && isApp)
          {path = relPath.localPath();   product.updateVersion(path);   product.loadVerEB(dialog);}
  }


void Component::storeDescription(WixAppDlg& dialog) {description = getText(dialog.descriptionEB);}


void Component::storeIsStartMenu(WixAppDlg& dialog)
                                              {isStartMenu  = dialog.isStartMenu.GetCheck()  != 0;}
void Component::storeIsDeskTop(WixAppDlg& dialog)
                                              {isDesktop    = dialog.isDeskTop.GetCheck()    != 0;}
void Component::storeIsOnPath( WixAppDlg& dialog)
                                              {isOnPath     = dialog.isOnPathCH.GetCheck()   != 0;}

void Component::storeStartup(  WixAppDlg& dialog)
                                              {isStartupApp = dialog.isStartupApp.GetCheck() != 0;}
#if 0
void Component::storeIsWin10(WixAppDlg& dialog){isWin10     = dialog.isWin10ch.GetCheck()    != 0;}
void Component::storeIsWin7(WixAppDlg& dialog) {isWin7      = dialog.isWin7ch.GetCheck()     != 0;}
void Component::storeIsWinXP(WixAppDlg& dialog){isWinXP     = dialog.isWinXPch.GetCheck()    != 0;}
#endif

void Component::delData() {
  id.clear();
  wixID.clear();
  section.clear();
  guid.clear();
  progFileID.clear();
  startMenuID.clear();
  iconID.clear();
  relPath.clear();
  description.clear();
  target.clear();
  arguments.clear();
  iconUsed       = false;
  isStartMenu    = false;
  isDesktop      = false;
  isVersionAvail = false;
  isOnPath       = false;
  isStartupApp   = false;

  isUninstall    = false;
  isUninstallDir = false;
  isApp          = false;
  }


void Component::getIconPath(WixAppDlg& dialog)
        {ComboBox& cb = dialog.iconCB;   iconID = icons.getPath(isStartupApp);   updateIconCB(cb);}


void Component::updateIconCB(ComboBox& cb) {icons.loadCB(cb);    icons.setCur(iconID, cb);}


void Component::updateIcon(WixAppDlg& dialog) {dialog.iconCB.getWindowText(iconID);}


void Component::markDirs(Feature& ftr) {
  progFileID  = ftr.progFileID;    markDirectories(progFileID,  pffDirectories.stor);
  startMenuID = ftr.startMenuID;   markDirectories(startMenuID, pmfDirectories.stor);

  pmfDirectories.setStartMenuSeen(isStartMenu);
  pmfDirectories.setDeskTopSeen(isDesktop);
  pmfDirectories.setStartupSeen(isStartupApp);
  }


void Component::markDirectories(String& id, DirStor& stor) {
DirDesc* dsc;
int      i;

  for (i = 0, dsc = stor.findItem(id); dsc; dsc = stor.findItem(dsc->parent), i++)
                                             {dsc->inUse = true;   if (i) dsc->hasChildren = true;}
  }



void Component::prepareUninstalls(String& newID, TCchar* wixid, String& progFileId) {
  id = newID; wixID = wixid; progFileID = progFileId; isUninstallDir = true;
  }


String Component::getSetEnvPath() {
String s;
String path;
String t;

  if (!iniFile.readString(SetEnvSect, SetEnvPath, s))
            {s = findExeFile(_T("SetEnv.exe"));    iniFile.writeString(SetEnvSect, SetEnvPath, s);}
  return s;
  }


bool Component::validate(bool rptErrors) {
struct _stat buffer;
String s = relPath.localPath();

  if (!s.isEmpty() && (_tstat(s, &buffer) != 0) && rptErrors) {

    String msg = _T("File not found: ") + relPath.localPath();

    MessageBox(0, msg, _T("WixApp"), MB_OK);   return false;
    }

  return true;
  }


void Component::output(int tab) {
String   line;
DirDesc* progDesc = getProgFile();
DirDesc* menuDesc = getStartMenu();
String   dirID    = isUninstall && isApp && menuDesc ? menuDesc->wixID :
                                                       progDesc ? progDesc->wixID : String(_T(""));
String   dir;

  if (dirID.isEmpty()) return;

  if (guid.isEmpty()) getGuid(guid);

  line  = _T("<Component Id=\"") + wixID + _T("\" Guid=\"") + guid + _T("\" ");
  line += _T("Directory=\"") + dirID + _T("\">");

  wix(tab, line);

  tab += TabVal;

  if (!isUninstall) outputSourcePath(tab);

  if (menuDesc) dir = menuDesc->wixID;

  if (isStartMenu) outputLink(tab, _T(".shc"), dir);

  if (isDesktop)   outputLink(tab, _T(".dsk"), DeskTopDir);

  if (isStartupApp) outputLink(tab, _T(".stu"), StartupDir);

  if (isOnPath) outputSetEnvPath(tab);

  if (isUninstall) pmfDirectories.outputRemoves(tab);

  if (relPath.isEmpty())          outputRegistry(tab);

  product.outputOScondition(tab);

  tab -= TabVal;

  wix(tab, _T("</Component>"));
  }


void Component::outputSourcePath(int tab) {
String line;
String targetPath;

  if (relPath.isEmpty()) return;

  line = _T("<File Id=\"") + wixID + _T("\" Source=\"");

  targetPath = _T("$(var.") + solution.name + _T(".TargetDir)\\");  targetPath += relPath;

  wix(tab, line, targetPath, _T("\" KeyPath=\"yes\" />"));
  }


void Component::outputLink(int tab, TCchar* suffix, TCchar* dir) {
String    sfx      = suffix; sfx += _T("\"");
DirDesc*  progDesc = getProgFile();
IconDesc* icon     = icons.find(iconID);
String    s;

  wix.crlf();

  s = removeBlnks(id);   wix(tab, _T("<Shortcut Id               = \""), s, sfx);
                         tab += 5 * TabVal;

                         wix(tab, _T("Name             = \""), id,                       _T("\""));
                         wix(tab, _T("Description      = \""), description,              _T("\""));

  if (!isUninstall)      wix(tab, _T("Advertise        = \""), String("yes"),            _T("\""));

  if (!isUninstall && *dir)
                         wix(tab, _T("Directory        = \""), String(dir),              _T("\""));

  if (isUninstall) {     wix(tab, _T("Target           = \""), target,                   _T("\""));
                         wix(tab, _T("Arguments        = \""), arguments,                _T("\""));
                         }
  else if (progDesc)     wix(tab, _T("WorkingDirectory = \""), progDesc->wixID,          _T("\""));
  if (icon)              wix(tab, _T("Icon             = \""), removeBlnks(icon->wixID), _T("\""));

                         wix(tab, _T("/>"));
  }


/*
  <StandardDirectory Id="ProgramFiles64Folder">
    <Directory Id="INSTALLFOLDER" Name="MyApp" />
  </StandardDirectory>

  <Component Id="SetSystemPath" Guid="*" System="yes">
    <File Source="MyApplication.exe" />

    <!-- Set the System PATH -->
    <Environment Id="PATH"
                 Name="PATH"
                 Value="[INSTALLFOLDER]"
                 Permanent="no"
                 Part="last"
                 Action="set"
                 System="yes" />
  </Component>*/

bool Component::outputSetEnvPath(int tab) {
String s;
DirDesc* pffDir = getProgFile();   if (!pffDir) return false;

  wix(tab, _T("<Environment Id=\"PATH\""));
  tab += 13;
  wix(tab, _T("Name=\"PATH\""));
  s.format(_T("Value=\"[%s]\""), pffDir->wixID.str());  wix(tab, s);
  wix(tab, _T("Permanent=\"no\""));
  wix(tab, _T("Part=\"last\""));
  wix(tab, _T("Action=\"set\""));
  wix(tab, _T("System=\"yes\""));
  wix(tab, _T("/>"));

  return true;
  }


void Component::outputRegistry(int tab) {
String prefix = _T("<RegistryValue Root=\"HKCU\" Key=\"Software\\");
String suffix = _T("\" Name=\"installed\" Type=\"integer\" Value=\"1\" KeyPath=\"yes\" />");

  wix(tab, prefix, wixID, suffix);
  }


void Component::copyObj(Component& c) {

  id             = c.id;
  wixID          = c.wixID;
  progFileID     = c.progFileID;
  startMenuID    = c.startMenuID;
  iconID         = c.iconID;

  section        = c.section;
  guid           = c.guid;

  relPath        = c.relPath;
  description    = c.description;
  target         = c.target;
  arguments      = c.arguments;
  isStartMenu    = c.isStartMenu;
  isDesktop      = c.isDesktop;
  isVersionAvail = c.isVersionAvail;
  isOnPath       = c.isOnPath;
  isStartupApp   = c.isStartupApp;

  isUninstallDir = c.isUninstallDir;
  isApp          = c.isApp;
  }


void Component::saveData(Archive& ar) {
String k;

  ar << aClrTabs << aSetTab(9) << aSetTab(30);

  ar << aTab << _T("Component: ") << id << aCrlf;
  ar << aClrTabs << aSetTab(12) << aSetTab(30);

  ar << aTab << _T("Wix ID:")         << aTab << wixID          << aCrlf;
  ar << aTab << _T("Section:")        << aTab << section        << aCrlf;
  ar << aTab << _T("GUID:")           << aTab << guid           << aCrlf;
  ar << aTab << _T("Prog File ID:")   << aTab << progFileID     << aCrlf;
  ar << aTab << _T("Start Menu ID:")  << aTab << startMenuID    << aCrlf;
  ar << aTab << _T("Icon ID:")        << aTab << iconID         << aCrlf;
  ar << aTab << _T("Rel Path:")       << aTab << relPath        << aCrlf;
  ar << aTab << _T("Description:")    << aTab << description    << aCrlf;
  ar << aTab << _T("Target:")         << aTab << target         << aCrlf;
  ar << aTab << _T("Arguments:")      << aTab << arguments      << aCrlf;
  ar << aTab << _T("iconUsed:")       << aTab << iconUsed       << aCrlf;
  ar << aTab << _T("isStartMenu:")    << aTab << isStartMenu    << aCrlf;
  ar << aTab << _T("isDesktop:")      << aTab << isDesktop      << aCrlf;
  ar << aTab << _T("isVersionAvail:") << aTab << isVersionAvail << aCrlf;
  ar << aTab << _T("isOnPath:")       << aTab << isOnPath       << aCrlf;
  ar << aTab << _T("isStartupApp:")   << aTab << isStartupApp   << aCrlf;
  ar << aTab << _T("isUninstall:")    << aTab << isUninstall    << aCrlf;
  ar << aTab << _T("isUninstallDir:") << aTab << isUninstallDir << aCrlf;
  ar << aTab << _T("isApp:")          << aTab << isApp          << aCrlf;
  }



////////----------------

#if 0
  if (!isUninstall || !fileName.isEmpty()) {

    line = _T("<File Id=\"") + wixID + _T("\" Source=\"");

    targetPath = _T("$(var.") + solution.name + _T(".TargetDir)\\") + fileName;

    wix(tab, line, targetPath, _T("\" KeyPath=\"yes\" />"));
    }
#endif
#if 0
//String   prodPath = relPath.prodPath();
//String   fileName = relPath.getFileName();
//String   targetPath;
  if (isWin10 || isWin7 || isWinXP) {
    lower = isWin10 ? _WIN32_WINNT_WIN10 : isWin7  ? _WIN32_WINNT_WIN7 :
                                                                 isWinXP ? _WIN32_WINNT_WINXP  : 0;
    upper = isWinXP ? _WIN32_WINNT_WINXP : 0;

    outputCondition(tab, lower, upper);
    }
#endif
#if 0
bool Component::outputSetPath(int tab, bool& crlfOut) {
String   installActionID;
String   uninstallActionID;
String   setEnvPath;
String   line;
DirDesc* progDesc = getProgFile();

  if (!isOnPath) return false;

  installActionID   = getWixID(id, _T("pth"));
  uninstallActionID = getWixID(id, _T("rmp"));
  setEnvPath        = getSetEnvPath();

  if (setEnvPath.isEmpty()) {
    String msg = _T("Please install Bob -- K6RWY's SetEnv as it is needed to ");
    msg += _T("execute the installer.");
    MessageBox(0, msg, _T("WixApp"), MB_OK);   return false;
    }

  if (!crlfOut) {
    wix.crlf(); crlfOut = true;
    wix(tab, _T("<!-- Custom Action to set the system (Machine) Path Variable -->"));   wix.crlf();
    wix(tab, _T("<Binary Id=\"SetEnvExe\" SourceFile=\""), setEnvPath, _T("\" />"));
    }

  if (progDesc) {
    line = _T("<CustomAction Id=\"") + installActionID +
                                              _T("\" BinaryKey=\"SetEnvExe\" Impersonate=\"no\" ");
    line += _T("Execute=\"deferred\" ");
    line += _T("ExeCommand='-a Path \" [") + progDesc->wixID + _T("] \"'   Return=\"check\" />");
    wix(tab, line);

    line = _T("<CustomAction Id=\"") + uninstallActionID +
                                              _T("\" BinaryKey=\"SetEnvExe\" Impersonate=\"no\" ");
    line += _T("Execute=\"deferred\" ");
    line += _T("ExeCommand='-d Path \" [") + progDesc->wixID + _T("] \"'   Return=\"check\" />");
    wix(tab, line); wix.crlf();
    }

  wix(tab, _T("<InstallExecuteSequence>"));

  line = _T("<Custom Action=\"") + installActionID + _T("\" Before=\"InstallFinalize\">");
  tab += TabVal;   wix(tab, line);
  tab += TabVal;   wix(tab, _T("(UPGRADINGPRODUCTCODE) OR NOT (REMOVE=\"ALL\")"));
  tab -= TabVal;   wix(tab, _T("</Custom>"));

  line  = _T("<Custom Action=\"") + uninstallActionID + _T("\" Before=\"");
  line += installActionID + _T("\">");
  wix(tab, line);
  tab += TabVal;   wix(tab, _T("(NOT UPGRADINGPRODUCTCODE) AND (REMOVE=\"ALL\")"));
  tab -= TabVal;   wix(_T("</Custom>"));

  tab -= TabVal;   wix(tab, _T("</InstallExecuteSequence>"));

  return true;
  }
#endif

