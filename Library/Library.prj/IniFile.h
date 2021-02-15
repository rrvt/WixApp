// Manage Ini File (A Place where stuff is remembered from execution to execution)
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.


#pragma once
#include "Random.h"


class IniFile {
CWinApp* app;
String   iniFilePath;
int      pathLng;

Tchar*   p;
Tchar*   q;
Tchar*   buf;
Random   rand;

public:

  IniFile() : app(0), p(0), q(0), buf(0), rand() {}
 ~IniFile();

//  String  getFilePath(TCchar* helpPath);    // Sets ini File Name & path to exe directory,
                                              // returns path to directory
  String  getAppDataPath(TCchar* helpPath);   // Sets ini File Name & path to appData/Roming directory
                                              // returns path to directory
  void    setPath(TCchar* filePath);          // Sets ini file name & path.
  String  path() {return iniFilePath;}

  void    setFilePath(String& pth,         CWinApp& theApp);
  void    setFilePath(TCchar* pth,         CWinApp& theApp);
  void    setAppDataPath(TCchar* helpPath, CWinApp& theApp);

  String  initIniDatum(TCchar* section, TCchar* name, TCchar* deflt, String& valu);
  void    saveIniDatum(TCchar* section, TCchar* name, String& valu);
  String  initIniPwd(  TCchar* section, TCchar* name, TCchar* deflt, String& valu);
  void    saveIniPwd(  TCchar* section, TCchar* name, String& valu);

  bool    writeString(TCchar* section, TCchar* key, String&  val);
  bool    writeString(TCchar* section, TCchar* key, CString& val);
  bool    writeInt(   TCchar* section, TCchar* key, int      def);
  bool    writePwd(   TCchar* section, TCchar* key, String&  val);
  void    writeEnd();

  bool    readString( TCchar* section, TCchar* key, String&  val);
  bool    readString( TCchar* section, TCchar* key, CString& val);
  bool    readString( TCchar* section, TCchar* key, String&  val, TCchar* dflt);
  bool    readString( TCchar* section, TCchar* key, CString& val, TCchar* dflt);
  int     readInt(    TCchar* section, TCchar* key, int      def);
  bool    readPwd(    TCchar* section, TCchar* key, String&  val);
  Tchar*  startReadSection();
  Tchar*  nextSection();

  void    deleteString(TCchar* section, TCchar* key);
  void    deleteSection(TCchar* section);

String    getIniPath() {return iniFilePath;}

protected:
  void    checkPath();
  void    setTheAppPath(CWinApp& theApp);
  String  encodePassword(String& password);
  Tchar   getRandCh();
  String  decodePassword(String& cipher);
  Tchar*  getSection() {return p < q ? p : 0;}
  };


extern IniFile iniFile;
