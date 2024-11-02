// Site ID Data


#include "pch.h"
#include "SiteID.h"
#include "IniFile.h"
#include "NotePad.h"
#include "SftpSSL.h"
#include "Sftp.h"
#include "SiteLoginDlg.h"
#include "SitePickerDlg.h"
#include "Utilities.h"
#include "WebFiles.h"


SiteID siteID;

       TCchar* GlobalSect    = _T("Global");
       TCchar* LastSiteKey   = _T("LastSite");
static TCchar* SiteNameKey   = _T("SiteName");                // IniFile key words
static TCchar* SitePathKey   = _T("Path");
static TCchar* URLNameKey    = _T("URLName");
       TCchar* UserIDKey     = _T("UserID");
       TCchar* PasswordKey   = _T("Password");
static TCchar* RemoteRootKey = _T("RemoteRoot");
static TCchar* NoWebFlsKey   = _T("NoWebFiles");


void SiteID::clear() {
  name.clear(); localRoot.clear(); url.clear(); userID.clear(); password.clear();
  remoteRoot.clear();
  noWebFiles = 0;
  if (loggedIn) {logout(); webFiles.clear();}
  }


bool SiteID::siteAvail() {
  if (siteLoaded) return true;

  return pickSite();
  }


// Pick a site for a list of site the application knows and load it

bool SiteID::pickSite() {
SitePickerDlg dlg;
String        section;

  if (dlg.DoModal() != IDOK) return false;

  section = dlg.section;

  return siteLoaded = loadData(section);
  }


bool SiteID::edit() {
SiteLoginDlg dlg;

  dlg.name = name;   dlg.rootTxt = localRoot;   dlg.urlName = url;
  userID >> dlg.userID;   password >> dlg.password;
  dlg.remoteRoot = remoteRoot;

  if (dlg.DoModal() != IDOK) return false;

  name = dlg.name;   localRoot = dlg.rootTxt;   url = dlg.urlName;
  userID << dlg.userID;   password << dlg.password;
  remoteRoot = dlg.remoteRoot;

  name = ensureSite(name);

  saveSiteData();  return true;
  }



bool SiteID::login() {
String user;   userID   >> user;    ToAnsi uid(user);
String pswd;   password >> pswd;    ToAnsi pwd(pswd);
String avail;
String rslt;
String s;
int    i;
bool   opened;

  if (loggedIn) return true;

  if (!isValid()) return false;

  loginPending = true;

  for (i = 0, opened = sftpSSL.open(url); i < 2 && !opened; i++, opened = sftpSSL.open(url)) continue;

  if (!opened) {loginPending = false;   return false;}

  loggedIn = sftpSSL.login(user, pswd);

  if (!loggedIn) {sftpSSL.noop(rslt);   loginPending = false;  return false;}

  notePad << _T("Web Site: ") << name << _T(" is Logged In") << nCrlf;

  String title = AppTitle;   title += _T(" for ") + name;   theApp.setTitle(title);

  loginPending = false;   return true;
  }


// All fields not empty

bool SiteID::isValid() {

  if (name.isEmpty())       return false;
  if (url.isEmpty())        return false;
  if (userID.isEmpty())     return false;
  if (password.isEmpty())   return false;
  if (remoteRoot.isEmpty()) return false;

  return true;
  }


bool SiteID::isCurDir(String& s) {
  if (s.find(_T("257")) != 0) return false;
  dspLines(s);
  return true;
  }


void SiteID::logout()
              {if (loggedIn) {sftpSSL.closeAll();  loggedIn = false;   theApp.setTitle(AppTitle);}}




bool SiteID::loadData(String& sectName) {
String user;
String pwd;

  if (sectName.isEmpty()) return false;

  iniFile.read(sectName, SiteNameKey,   name,       _T(""));
  iniFile.read(sectName, SitePathKey,   localRoot,  _T(""));
  iniFile.read(sectName, URLNameKey,    url,        _T(""));
  iniFile.read(sectName, UserIDKey,     user,       _T(""));    userID  = user;
  iniFile.read(sectName, PasswordKey,   pwd,        _T(""));   password = pwd;
  iniFile.read(sectName, RemoteRootKey, remoteRoot, _T(""));
  noWebFiles = iniFile.readInt(sectName, NoWebFlsKey, 0);

  fixLocalPath(localRoot);

  return true;
  }


Baffle SiteID::loadBaffle(TCchar* sectName, TCchar* key) {
String s;
Baffle b;

  iniFile.readString(sectName, key, s, _T(""));    b = s;   return b;
  }



void SiteID::saveSiteData() {
String user;
String pwd;

  if (!setCurrentSite()) return;

  iniFile.write(name,       SiteNameKey,   name);
  iniFile.write(name,       SitePathKey,   localRoot);
  iniFile.write(name,       URLNameKey,    url);
  iniFile.write(name,       UserIDKey ,    userID());
  iniFile.write(name,       PasswordKey,   password());
  iniFile.write(name,       RemoteRootKey, remoteRoot);
  iniFile.write(name,       NoWebFlsKey,   noWebFiles);
  }


bool SiteID::setCurrentSite() {

  if (name.isEmpty()) return false;

  iniFile.write(GlobalSect, LastSiteKey,   name);   return true;
  }


void SiteID::delSiteData() {
String sect = name;    if (sect.isEmpty()) return;

  iniFile.deleteSection(sect);
  iniFile.deleteString(GlobalSect, LastSiteKey);
  clear();
  }


String SiteID::webToLocal(String& path)
                {path = localRoot + path.substr(remoteRoot.length());   return path = toLocalPath(path);}


String SiteID::webToRelative(String& path) {

  if (path.find(remoteRoot) == 0) path = path.substr(remoteRoot.length());

  return path = toLocalPath(path);
  }


String SiteID::toRelative(TCchar* path) {
String s = path;

  if      (s.find(remoteRoot) == 0) s = s.substr(remoteRoot.length());
  else if (s.find(localRoot)  == 0) s = s.substr(localRoot.length());

  return toLocalPath(s);
  }

