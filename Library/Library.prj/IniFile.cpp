// Manage Ini File (A Place where stuff is remembered from execution to execution)
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.


#include "stdafx.h"
#include "IniFile.h"
#include "fileName.h"
#include <Shlobj.h>


const int BufSize = 1024;


String IniFile::getAppDataPath(TCchar* helpPath) {
Tchar   stg[1024];
String path;
String mainName = getMainName(helpPath);

HRESULT rslt     = SHGetFolderPath(0, CSIDL_APPDATA, 0, SHGFP_TYPE_DEFAULT, stg);

  if (rslt == S_OK) {path = stg; path += _T('\\'); path += mainName; path += _T('\\');}
  else               path = _T(".\\");

  iniFilePath = path + mainName; iniFilePath += _T(".ini"); checkPath();

  return path;
  }


void IniFile::setPath(TCchar* const filePath) {iniFilePath = filePath; checkPath();}


void IniFile::setAppDataPath(TCchar* helpPath, CWinApp& theApp) {
String  mainName = getMainName(helpPath);
Tchar    stg[1024];
HRESULT rslt;

  rslt = SHGetFolderPath(0, CSIDL_APPDATA, 0, SHGFP_TYPE_DEFAULT, stg);

  iniFilePath = stg; iniFilePath += _T('\\'); iniFilePath += mainName; iniFilePath += _T('\\');

  iniFilePath += mainName; iniFilePath += _T(".ini");

  checkPath(); setTheAppPath(theApp);
  }


void IniFile::setFilePath(String& pth, CWinApp& theApp)
                                                {iniFilePath = pth; checkPath(); setTheAppPath(theApp);}



void IniFile::setFilePath(TCchar* pth, CWinApp& theApp)
                                                {iniFilePath = pth; checkPath(); setTheAppPath(theApp);}



void IniFile::setTheAppPath(CWinApp& theApp) {
TCchar* prfl = theApp.m_pszProfileName;

  try {if (prfl) free((void*) prfl);} catch (...) {}

  pathLng  = iniFilePath.size() + 4;

  theApp.m_pszProfileName = (LPCTSTR) malloc(pathLng*sizeof(TCchar));

  _tcscpy_s((Tchar*) theApp.m_pszProfileName, pathLng, iniFilePath);
  }


void IniFile::checkPath() {
String path = getPath(iniFilePath);

  CreateDirectory(path, 0);
  }




String IniFile::initIniDatum(TCchar* section, TCchar* name, TCchar* deflt, String& valu) {
  if (!iniFile.readString(section, name, valu))
                                              {valu = deflt; iniFile.writeString(section, name, valu);}
  return valu;
  }


void IniFile::saveIniDatum(TCchar* section, TCchar* name, String& valu)
                                                              {iniFile.writeString(section, name, valu);}


String IniFile::initIniPwd(TCchar* section, TCchar* name, TCchar* deflt, String& valu) {
  if (!iniFile.readPwd(section, name, valu)) {valu = deflt; iniFile.writePwd(section, name, valu);}
  return valu;
  }


void IniFile::saveIniPwd(TCchar* section, TCchar* name, String& valu)
                                                                 {iniFile.writePwd(section, name, valu);}


bool IniFile::writeString(TCchar* section, TCchar* key, CString& val) {
  return WritePrivateProfileString(section, key, val, iniFilePath) != 0;
  }


bool IniFile::writeString(TCchar* section, TCchar* key, String& val) {
  return WritePrivateProfileString(section, key, val, iniFilePath) != 0;
  }

bool IniFile::writeInt(TCchar* section, TCchar* key, int def) {
  return WritePrivateProfileString(section, key, toString(def), iniFilePath) != 0;
  }


bool IniFile::writePwd(  TCchar* section, TCchar* key, String&  val) {
String s = val;

  return WritePrivateProfileString(section, key, encodePassword(s), iniFilePath) != 0;
  }


void IniFile::writeEnd() {
CFile cFile(iniFilePath, CFile::modeWrite);

  cFile.SeekToEnd();   cFile.Write("[END]\r\n", 7);   cFile.Close();
  }



#if 0
Tchar* IniFile::startReadSection() {
int n;

  if (!buf) {NewArray(Tchar);   buf = AllocArray(BufSize); p = buf;}

  n = GetPrivateProfileString(0, 0, 0, buf, BufSize, iniFilePath);

  q = p + n;  return getSection();
  }


Tchar* IniFile::nextSection() {

  if (!p) {return 0;}

  p += _tcslen(p) + 1; return getSection();
  }

#endif



void IniFile::deleteString(TCchar* section, TCchar* key) {
  WritePrivateProfileString(section, key, 0, iniFilePath);
  }


void IniFile::deleteSection(TCchar* section) {
  WritePrivateProfileString(section, 0, 0, iniFilePath);
  }


bool IniFile::readString( TCchar* section, TCchar* key, CString& val) {
Tchar stg[1024];

  if (GetPrivateProfileString(section, key, val, stg, noElements(stg), iniFilePath))
                                                                               {val = stg; return true;}
  return false;
  }


bool IniFile::readString(TCchar* section, TCchar* key, String& val) {
Tchar stg[1024];

  if (GetPrivateProfileString(section, key, val, stg, noElements(stg), iniFilePath))
                                                                               {val = stg; return true;}
  return false;
  }


bool IniFile::readString( TCchar* section, TCchar* key, String&  val, TCchar* dflt) {
Tchar stg[1024];

  if (GetPrivateProfileString(section, key, dflt, stg, noElements(stg), iniFilePath))
                                                                               {val = stg; return true;}
  return false;
  }


bool IniFile::readString( TCchar* section, TCchar* key, CString& val, TCchar* dflt) {
Tchar stg[1024];

  if (GetPrivateProfileString(section, key, dflt, stg, noElements(stg), iniFilePath))
                                                                               {val = stg; return true;}
  return false;
  }



int IniFile::readInt(TCchar* section, TCchar* key, int def)
                                      {return GetPrivateProfileInt(section, key, def, iniFilePath);}


bool IniFile::readPwd(   TCchar* section, TCchar* key, String&  val) {
Tchar  stg[64];
String s;

  if (GetPrivateProfileString(section, key, 0, stg, noElements(stg), iniFilePath))
                                                        {s = stg; val = decodePassword(s); return true;}
  return false;
  }


static const int BaseCh = ' ';
static const int Range  = 128 - BaseCh;

String IniFile::encodePassword(String& password) {
int    lng    = password.length();
Tchar  lower  = getRandCh();
Tchar  upper  = getRandCh();
Random r((upper << 8) | lower);
float rng     = (float) Range;
int    i;
String s;
Tchar  ch;

  s = lower;

  for (i = 0; i < lng; i++) {
    int   offset = int(rng * r.next());
    Tchar ch1    = password[i] - BaseCh;
    Tchar ch2    = (ch1 + offset) % Range;
          ch     = ch2 + BaseCh;

    s += ch;
    }

  s += upper;   return s;
  }


Tchar IniFile::getRandCh() {
float rng = (float) Range;
Tchar  ch;

  do {
    ch = (Tchar) (rng * rand.next() + BaseCh);

    } while (ch == _T('\\') || ch == _T('"'));

  return ch;
  }


String IniFile::decodePassword(String& cipher) {
int    lng    = cipher.length();
Tchar  lower  = cipher[0];
Tchar  upper  = cipher[lng-1];
Random r((upper << 8) | lower);
float rng     = (float) Range;
int    i;
String s;

  for (i = 1; i < lng-1; i++) {
    int   offset = int(rng * r.next());
    short ch1    = cipher[i] - BaseCh;
    short ch2    = ch1 - offset;   if (ch2 < 0) ch2 += Range;
    Tchar  ch    = ch2 + BaseCh;

    s += ch;
    }

  return s;
  }




TCchar* IniSectIter::operator() () {
int n;

  do {
    if (buf) {bufSize *= 2;  clear();}

    if (!buf) {NewArray(Tchar);   buf = AllocArray(bufSize); p = buf;}

    n = GetPrivateProfileString(0, 0, 0, buf, bufSize, ini.iniFilePath);

    } while (n >= bufSize-2);

  endBuf = p + n;  return p < endBuf ? p : 0;
  }


TCchar* IniSectIter::operator++ (int) {

  if (!p) {return 0;}

  p += _tcslen(p) + 1; return p < endBuf ? p : 0;
  }

