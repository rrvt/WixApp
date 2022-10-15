// A Single Component's data


#include "stdafx.h"
#include "Component.h"
#include "Feature.h"
#include "filename.h"
#include "Guid.h"
#include "PFFdirectories.h"
#include "PMFdirectories.h"
#include "Product.h"
#include "Resources.h"
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
static TCchar*    IsWin7Key       = _T("IsWin7");
static TCchar*    IsWinXPKey      = _T("IsWinXP");
static TCchar*    IsWin2KKey      = _T("IsWin2K");
static TCchar*    ProgKey         = _T("Program");
static TCchar*    StartKey        = _T("Start");

static TCchar*    ComponentExt    = _T("cmp");

static TCchar*    SetEnvSect      = _T("SetEnv");
static TCchar*    SetEnvPath      = _T("Path");

static TCchar*    CmpPathKey      = _T("Component");

static BrowseDsc  browseDsc       = {_T("Cmp"), _T("Component File"), _T(""), _T("*"), _T("")};


Component::Component() : isStartMenu(false), isDesktop(false),   isVersionAvail(false),
                         isStartupApp(false),
                         isOnPath(false),    isUninstall(false), isUninstallDir(false),
                         isApp(false),       isWin7(false),      isWinXP(false),
                         isWin2K(false),     iconUsed(false) { }


void Component::readWixData(String& ftrID) {
String path;

  section.format(Section,      id.str());

  readOne(IDkey,   id);          wixID = getWixID(id, ComponentExt);

  browseDsc.dfltKey = ftrID;   pathDsc.readWixData(browseDsc, section, PathKey);

  path = pathDsc.path();   setApp(path);
  readOne(GuidKey, guid);
  readOne(DescKey, description);
  readOne(IconKey, iconID);

  isStartMenu    = wxd.readInt(section, IsStartMenuKey,  0) != 0;
  isDesktop      = wxd.readInt(section, IsDeskTopKey,    0) != 0;
  isVersionAvail = wxd.readInt(section, IsVersionKey,    0) != 0;
  isOnPath       = wxd.readInt(section, IsOnPathKey,     0) != 0;
  isStartupApp   = wxd.readInt(section, IsStartupAppKey, 0) != 0;
  isWin7         = wxd.readInt(section, IsWin7Key,       0) != 0;
  isWinXP        = wxd.readInt(section, IsWinXPKey,      0) != 0;
  isWin2K        = wxd.readInt(section, IsWin2KKey,      0) != 0;

  if (isVersionAvail && isApp) product.updateVersion(path);
  }


void Component::readOne(TCchar* key, String& v) {
  v = _T("<>");   if (!wxd.readString(section, key, v) || v == _T("<>")) v.clear();
  }



void Component::readWixData2(String& ftrID, String& cmpSection) {
String path;

  wixID = getWixID(id, ComponentExt);

  browseDsc.dfltKey = ftrID; pathDsc.readWixData(browseDsc, cmpSection, PathKey);

  guid.clear();        wxd.readString(cmpSection, GuidKey, guid);
  description.clear(); wxd.readString(cmpSection, DescKey, description);
  iconID.clear();      wxd.readString(cmpSection, IconKey, iconID);

  isStartMenu    = wxd.readInt(cmpSection, IsStartMenuKey,  0) != 0;
  isDesktop      = wxd.readInt(cmpSection, IsDeskTopKey,    0) != 0;
  isVersionAvail = wxd.readInt(cmpSection, IsVersionKey,    0) != 0;
  isOnPath       = wxd.readInt(cmpSection, IsOnPathKey,     0) != 0;
  isStartupApp   = wxd.readInt(cmpSection, IsStartupAppKey, 0) != 0;
  isWin7         = wxd.readInt(cmpSection, IsWin7Key,       0) != 0;
  isWinXP        = wxd.readInt(cmpSection, IsWinXPKey,      0) != 0;
  isWin2K        = wxd.readInt(cmpSection, IsWin2KKey,      0) != 0;

  path = pathDsc.path();   setApp(path);
  if (isVersionAvail && isApp) product.updateVersion(path);
  }


void Component::writeWixData2(String& cmpSection) {

  wxd.writeString(cmpSection, IDkey,          id);
  pathDsc.writeWixData(cmpSection, PathKey);
  if (!guid.isEmpty())        wxd.writeString(cmpSection, GuidKey,         guid);
  if (!description.isEmpty()) wxd.writeString(cmpSection, DescKey,         description);
  if (!iconID.isEmpty())      wxd.writeString(cmpSection, IconKey,         iconID);
  if (isStartMenu)            wxd.writeInt(   cmpSection, IsStartMenuKey,  isStartMenu);
  if (isDesktop)              wxd.writeInt(   cmpSection, IsDeskTopKey,    isDesktop);
  if (isVersionAvail)         wxd.writeInt(   cmpSection, IsVersionKey,    isVersionAvail);
  if (isOnPath)               wxd.writeInt(   cmpSection, IsOnPathKey,     isOnPath);
  if (isStartupApp)           wxd.writeInt(   cmpSection, IsStartupAppKey, isStartupApp);
  if (isWin7)                 wxd.writeInt(   cmpSection, IsWin7Key,       isWin7);
  if (isWinXP)                wxd.writeInt(   cmpSection, IsWinXPKey,      isWinXP);
  if (isWin2K)                wxd.writeInt(   cmpSection, IsWin2KKey,      isWin2K);
  }



void Component::writeWixData() {

  section.format(Section, id.str());

  wxd.writeString(section, IDkey,          id);
  pathDsc.writeWixData(section, PathKey);
  wxd.writeString(section, GuidKey,          guid);
  wxd.writeString(section, DescKey,          description);
  wxd.writeString(section, IconKey,          iconID);
  wxd.writeInt(   section, IsStartMenuKey,   isStartMenu);
  wxd.writeInt(   section, IsDeskTopKey,     isDesktop);
  wxd.writeInt(   section, IsVersionKey,     isVersionAvail);
  wxd.writeInt(   section, IsOnPathKey,      isOnPath);
  wxd.writeInt(   section, IsStartupAppKey,  isStartupApp);
  wxd.writeInt(   section, IsWin7Key,        isWin7);
  wxd.writeInt(   section, IsWinXPKey,       isWinXP);
  wxd.writeInt(   section, IsWin2KKey,       isWin2K);
  }



void Component::loadNew(WixAppDlg& dialog) {load(dialog);  dialog.componentCB.setFocus();}


void Component::load(WixAppDlg& dialog) {
String s;
ComboBox& cb = dialog.iconCB;

    dialog.componentCB.setCurSel(id);
         dialog.pathEB.SetReadOnly(false);
         dialog.pathEB.SetWindowText(pathDsc);
         dialog.pathEB.SetReadOnly(true);
  dialog.descriptionEB.SetWindowText(description);
    dialog.isStartMenu.SetCheck(isStartMenu);
      dialog.isDeskTop.SetCheck(isDesktop);
 dialog.isVersionAvail.SetCheck(isVersionAvail);
     dialog.isOnPathCH.SetCheck(isOnPath);
     dialog.isStartupApp.SetCheck(isStartupApp);
       dialog.isWin7ch.SetCheck(isWin7);
      dialog.isWinXPch.SetCheck(isWinXP);
      dialog.isWin2Kch.SetCheck(isWin2K);

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
  isWin7         = dialog.isWin7ch.GetCheck()       != 0;
  isWinXP        = dialog.isWinXPch.GetCheck()      != 0;
  isWin2K        = dialog.isWin2Kch.GetCheck()      != 0;
  }



DirDesc* Component::getProgFile()  {return pffDirectories.find(progFileID);}
DirDesc* Component::getStartMenu() {return pmfDirectories.find(startMenuID);}

#if 0
void Component::updateComponent(WixAppDlg& dialog) {
  dialog.componentCB.getWindowText(id);  wixID = getWixID(id, ComponentExt);
  }
#endif


void Component::browsePath(String& ftrID, WixAppDlg& dialog) {
String path;

  browseDsc.dfltKey = ftrID; pathDsc.browse(browseDsc);

  dialog.pathEB.SetWindowText(pathDsc);

  path = pathDsc.path();   setApp(path);   storeIsVersionAvail(dialog);
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
                    {path = pathDsc.path();   product.updateVersion(path);   product.loadVerEB(dialog);}
  }


void Component::storeDescription(WixAppDlg& dialog) {description = getText(dialog.descriptionEB);}


void Component::storeIsStartMenu(WixAppDlg& dialog)
                                                 {isStartMenu    = dialog.isStartMenu.GetCheck()!= 0;}
void Component::storeIsDeskTop(WixAppDlg& dialog) {isDesktop    = dialog.isDeskTop.GetCheck()  != 0;}
void Component::storeIsOnPath( WixAppDlg& dialog)  {isOnPath    = dialog.isOnPathCH.GetCheck() != 0;}

void Component::storeStartup(  WixAppDlg& dialog) {isStartupApp = dialog.isStartupApp.GetCheck() != 0;}

void Component::storeIsWin7(WixAppDlg& dialog)    {isWin7     = dialog.isWin7ch.GetCheck()     != 0;}
void Component::storeIsWinXP(WixAppDlg& dialog)   {isWinXP    = dialog.isWinXPch.GetCheck()    != 0;}
void Component::storeIsWin2K(WixAppDlg& dialog)   {isWin2K    = dialog.isWin2Kch.GetCheck()    != 0;}


void Component::delData() {
  id.clear();
  wixID.clear();
  section.clear();
  guid.clear();
  progFileID.clear();
  startMenuID.clear();
  iconID.clear();
  pathDsc.clear();
  description.clear();
  target.clear();
  arguments.clear();
  iconUsed       = false;
  isStartMenu    = false;
  isDesktop      = false;
  isVersionAvail = false;
  isOnPath       = false;
  isStartupApp   = false;
  isWin7         = false;
  isWinXP        = false;
  isWin2K        = false;

  isUninstall    = false;
  isUninstallDir = false;
  isApp          = false;
  }


void Component::browseIcon(WixAppDlg& dialog)
               {ComboBox& cb = dialog.iconCB;   iconID = icons.browse(isStartupApp);   updateIconCB(cb);}


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


void Component::outputSetPath(int tab, bool& crlfOut) {
String   installActionID;
String   uninstallActionID;
String   setEnvPath;
String   line;
DirDesc* progDesc = getProgFile();

  if (!isOnPath) return;

  installActionID   = getWixID(id, _T("pth"));
  uninstallActionID = getWixID(id, _T("rmp"));
  setEnvPath        = getSetEnvPath();

  if (setEnvPath.isEmpty()) {
    TCchar* msg = _T("Please install Bob -- K6RWY's SetEnv as it is needed to execute the installer.");
    MessageBox(0, msg, _T("WixApp"), MB_OK);   return;
    }

  if (!crlfOut) {
    wix.crlf(); crlfOut = true; wix(tab);
    wix(_T("<!-- Custom Action to set the system (Machine) Path Variable -->"));   wix.crlf(2);
    wix(_T("<Binary Id=\"SetEnvExe\" SourceFile=\""), setEnvPath, _T("\" />"));    wix.crlf();
    }

  if (progDesc) {
    line = _T("<CustomAction Id=\"") + installActionID +
                                                    _T("\" BinaryKey=\"SetEnvExe\" Impersonate=\"no\" ");
    line += _T("Execute=\"deferred\" ");
    line += _T("ExeCommand='-a Path \" [") + progDesc->wixID + _T("] \"'   Return=\"check\"/>");
    wix(tab);   wix(line);   wix.crlf();

    line = _T("<CustomAction Id=\"") + uninstallActionID +
                                                    _T("\" BinaryKey=\"SetEnvExe\" Impersonate=\"no\" ");
    line += _T("Execute=\"deferred\" ");
    line += _T("ExeCommand='-d Path \" [") + progDesc->wixID + _T("] \"'   Return=\"check\"/>");
    wix(tab);   wix(line); wix.crlf(2);
    }

  wix(tab);   wix(_T("<InstallExecuteSequence>"));   wix.crlf();

  line = _T("<Custom Action=\"") + installActionID + _T("\" Before=\"InstallFinalize\">");
  wix(tab+1);   wix(line);   wix.crlf();
  wix(tab+2);   wix(_T("(UPGRADINGPRODUCTCODE) OR NOT (REMOVE=\"ALL\")"));   wix.crlf();
  wix(tab+1);   wix(_T("</Custom>"));   wix.crlf();

  line = _T("<Custom Action=\"") + uninstallActionID + _T("\" Before=\"") + installActionID + _T("\">");
  wix(tab+1);   wix(line);   wix.crlf();
  wix(tab+2);   wix(_T("(NOT UPGRADINGPRODUCTCODE) AND (REMOVE=\"ALL\")"));   wix.crlf();
  wix(tab+1);   wix(_T("</Custom>"));   wix.crlf();

  wix(tab);     wix(_T("</InstallExecuteSequence>"));   wix.crlf();
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
String s = pathDsc.path();

  if (!s.isEmpty() && (_tstat(s, &buffer) != 0) && rptErrors) {

    String msg = _T("File not found: ") + pathDsc.path();

    MessageBox(0, msg, _T("WixApp"), MB_OK);   return false;
    }

  return true;
  }


void Component::output(int tab) {
String   line;
String   relPath = pathDsc.relative();
int      lower;
int      upper;
DirDesc* progDesc = getProgFile();
DirDesc* menuDesc = getStartMenu();
String   dirID    = isUninstall && isApp && menuDesc ? menuDesc->wixID :
                                                            progDesc ? progDesc->wixID : String(_T(""));
String   dir;

  if (dirID.isEmpty()) return;

  if (guid.isEmpty()) getGuid(guid);

  wix.crlf();

  line  = _T("<Component Id=\"") + wixID + _T("\"  Guid=\"") + guid + _T("\" ");
  line += _T("Directory=\"") + dirID + _T("\">");

  wix(tab);   wix(line);   wix.crlf();

  line = _T("<File Id=\"") + wixID + _T("\" Source=\"");
  wix(tab+1); wix(line, relPath, _T("\" KeyPath=\"yes\"/>"));

  if (menuDesc) dir = menuDesc->wixID;

  if (isStartMenu) outputLink(tab+1, _T(".shc"), dir);

  if (isDesktop)   outputLink(tab+1, _T(".dsk"), DeskTopDir);

  if (isStartupApp) outputLink(tab+1, _T(".stu"), StartupDir);

  if (isUninstall) pmfDirectories.outputRemoves(tab+1);           //|| isUninstallDir

  if (pathDsc.isEmpty())      outputRegistry(tab+1);

  if (isWin7 || isWinXP || isWin2K) {
    lower = isWin2K ? _WIN32_WINNT_WIN2K : isWinXP ? _WIN32_WINNT_WINXP :
                                                                        isWin7  ? _WIN32_WINNT_WIN7  : 0;
    upper = isWin7  ? 0                  : isWinXP ? _WIN32_WINNT_WINXP :
                                                                        isWin2K ? _WIN32_WINNT_WIN2K : 0;
    outputCondition(tab + 1, lower, upper);
    }

  wix(tab);   wix(_T("</Component>"));   wix.crlf();
  }


void Component::outputLink(int tab, TCchar* suffix, TCchar* dir) {
int       tb       = tab + 5;
String    sfx      = suffix; sfx += _T("\"");
DirDesc*  progDesc = getProgFile();
IconDesc* icon     = icons.find(iconID);
String    s;

  wix.crlf();

  s = removeBlnks(id);   wix(tab); wix(_T("<Shortcut Id               = \""), s, sfx);
                         wix(tb);
                         wix( _T("Name             = \""), id,                       _T("\""));
                         wix( _T("Description      = \""), description,              _T("\""));

  if (!isUninstall)      wix(_T("Advertise        = \""), String("yes"),            _T("\""));

  if (!isUninstall && *dir)
                         wix(_T("Directory        = \""), String(dir),              _T("\""));

  if (isUninstall) {     wix(_T("Target           = \""), target,                   _T("\""));
                         wix(_T("Arguments        = \""), arguments,                _T("\""));
                         }
  else if (progDesc)     wix(_T("WorkingDirectory = \""), progDesc->wixID,          _T("\""));
  if (icon)              wix(_T("Icon             = \""), removeBlnks(icon->wixID), _T("\""));

                         wix(_T("/>"));   wix.crlf();
  }


void Component::outputRegistry( int tab) {
  wix(tab);   wix(_T("<RegistryValue Root=\"HKCU\" Key=\"Software\\"), wixID, _T("\" Name=\"installed\""));
  wix(tab+7); wix(_T(" Type=\"integer\" Value=\""), _T("1"), _T("\" KeyPath=\"yes\"/>"));
  }


// There are three cases
//  -- One OS
//  -- OS1 to OS2
//  -- OS to infinity
//  <Condition>
//    <![CDATA[Installed OR VersionNT >= 501 AND VersionNT < 601]]>         <!-- Win XP, Vista -->
//  </Condition>

void Component::outputCondition(int tab, int firstCond, int lastCond) {
String line;

  if (!firstCond) return;

  wix(tab);   wix(_T("<Condition>"));   wix.crlf();



  if (firstCond == lastCond) line.format(_T("<![CDATA[Installed OR VersionNT = %x]]>"), firstCond);

  else if (firstCond && lastCond)
    line.format(_T("<![CDATA[Installed OR VersionNT >= %x AND VersionNT <= %x]]>"), firstCond, lastCond);

  else                       line.format(_T("<![CDATA[Installed OR VersionNT >= %x]]>"), firstCond);

  wix(tab+1);   wix(line);   wix.crlf();

  wix(tab);   wix(_T("</Condition>"));   wix.crlf();
  }


void Component::copyObj(Component& c) {

  id             = c.id;
  wixID          = c.wixID;
  progFileID     = c.progFileID;
  startMenuID    = c.startMenuID;
  iconID         = c.iconID;

  section        = c.section;
  guid           = c.guid;

  pathDsc        = c.pathDsc;
  description    = c.description;
  target         = c.target;
  arguments      = c.arguments;
  isStartMenu    = c.isStartMenu;
  isDesktop      = c.isDesktop;
  isVersionAvail = c.isVersionAvail;
  isOnPath       = c.isOnPath;
  isStartupApp   = c.isStartupApp;
  isWin7         = c.isWin7;
  isWinXP        = c.isWinXP;
  isWin2K        = c.isWin2K;

  isUninstallDir = c.isUninstallDir;
  isApp          = c.isApp;
  }



#if 0
void Component::storeProgFileName(WixAppDlg& dialog) {
//  progFile = directory.pff.add(getText(dialog.progFileEB));
  }


void Component::storeMenuName(WixAppDlg& dialog) {
//  startMenu = directory.pmf.add(getText(dialog.startMenuEB));
  }
#endif


#if 0
      // Working
void Component::readDir(TCchar* key, DirDesc& dir, DirStor& stor)
            {DirDesc* dsc = stor.add( dir.readWixData(section, key));   if (dsc) dir.wixID = dsc->wixID;}
#endif



#if 0
void Component::outputStartMenu(int tab) {
int       tb = tab + 5;
String    line;
DirDesc*  progDesc = getProgFile();
DirDesc*  menuDesc = getStartMenu();
IconDesc* icon = icons.find(iconID);

  wix.crlf();

  wix.out(tab, _T("<Shortcut Id               = \""), removeBlnks(id), _T(".shc\""));

  wix.out(tb,  _T("Name             = \""), id,                       _T("\""));
  wix.out(tb,  _T("Description      = \""), description,              _T("\""));

  if (target.isEmpty())
    wix.out(tb, _T("Advertise        = \""), String("yes"),           _T("\""));

  if (!isUninstall && menuDesc)
    wix.out(tb, _T("Directory        = \""), menuDesc->wixID,         _T("\""));

  wix.out(tb,   _T("Target           = \""), target,                  _T("\""));
  wix.out(tb,   _T("Arguments        = \""), arguments,               _T("\""));
  if (!isUninstall && progDesc)
    wix.out(tb, _T("WorkingDirectory = \""), progDesc->wixID,          _T("\""));
  if (icon)
    wix.out(tb, _T("Icon             = \""), removeBlnks(icon->wixID), _T("\""));

  wix.lit(tb, _T("/>\n"));
  }



void Component::outputDesktop(int tab) {
int       tb = tab + 5;
String    line;
DirDesc*  progDesc = getProgFile();
IconDesc* icon = icons.find(iconID);

  wix.crlf();

  wix.out(tab, _T("<Shortcut Id               = \""), removeBlnks(id), _T(".dsk\""));

  wix.out(tb,   _T("Name             = \""), id,                      _T("\""));
  wix.out(tb,   _T("Description      = \""), description,             _T("\""));
  wix.out(tb,   _T("Advertise        = \""), String("yes"),           _T("\""));

  if (isStartupApp)
       {wix.out(tb,   _T("Directory        = \""), String(StartupDir),      _T("\""));}
  else {wix.out(tb,   _T("Directory        = \""), String(DeskTopDir),      _T("\""));}

  if (progDesc)
    wix.out(tb, _T("WorkingDirectory = \""), progDesc->wixID,         _T("\""));
  if (icon)
    wix.out(tb, _T("Icon             = \""), removeBlnks(icon->wixID), _T("\""));

  wix.lit(tb, _T("/>\n"));
  }
//  if (isStartMenu) outputStartMenu(tab+1);
//  if (isDesktop)   outputDesktop(tab+1);
#endif
#if 0
  if (isMenu && !isUninstall && menuDesc)
                         wix.out(tb, _T("Directory        = \""), menuDesc->wixID,          _T("\""));
  else if (isDesktop)    wix.out(tb, _T("Directory        = \""), String(DeskTopDir),       _T("\""));
  else if (isStartupApp) wix.out(tb, _T("Directory        = \""), String(StartupDir),       _T("\""));
#else
#endif

