// Site ID Data


#pragma once
#include "SFTP.h"
#include "Utilities.h"


extern TCchar* GlobalSect;
extern TCchar* LastSiteKey;
extern TCchar* UserIDKey;
extern TCchar* PasswordKey;



class SiteID {
bool   siteLoaded;
bool   loginPending;
public:

String name;                            // Overall name for Site
String localRoot;                       // Local Root Path
String url;                             // name to use in sftpSSL login
Baffle userID;                          // user ID for sftp login
Baffle password;                        // password for sftp login
String remoteRoot;                      // User Supplied remote root directory
int    noWebFiles;                      // ~Number of files on Web Host
bool   loggedIn;


  SiteID() : siteLoaded(false), loginPending(false), noWebFiles(0), loggedIn(false) { }
 ~SiteID() {logout();}

  void    clear();

  bool    siteAvail();
  bool    pickSite();

  bool    edit();

  bool    login();
  bool    isLoginPending() {return loginPending;}
  void    logout();

  bool    loadData(String& sectName);
  Baffle  loadBaffle(TCchar* sectName, TCchar* key);
  bool    setCurrentSite();
  void    saveSiteData();
  void    delSiteData();

  String& dbPath(String& path)
                    {path = theApp.roamingPath() + removeSpaces(name) + _T(".csv");   return path;}

  String  webToLocal(   String& path);
  String  webToRelative(String& path);
  String  toRelative(   TCchar* path);

private:

  bool    isCurDir(String& s);
  bool    isValid();          // All fields not empty
  };


extern SiteID siteID;



//  void    defaultSite();

