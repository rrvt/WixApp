// Manage Ini File (A Place where stuff is remembered from execution to execution)
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.


#include "pch.h"
#include "IniFileBase.h"
#include "fileName.h"
#include "MessageBox.h"
#include <Shlobj.h>


const int BufSize = 1024;



String IniFileBase::getAppDataPath(TCchar* helpPath) {
Tchar  stg[1024];
String path;
String mainName = getMainName(helpPath);

HRESULT rslt     = SHGetFolderPath(0, CSIDL_APPDATA, 0, SHGFP_TYPE_DEFAULT, stg);

  if (rslt == S_OK) {path = stg; path += _T('\\'); path += mainName; path += _T('\\');}
  else               path = _T(".\\");

  iniFilePath = path + mainName; iniFilePath += _T(".ini"); checkPath();

  return path;
  }


void IniFileBase::setPath(TCchar* const filePath) {iniFilePath = filePath; checkPath();}


#if 1

static TCchar* NotImplMsg = _T("Not Implemented");
void IniFileBase::setAppDataPath(TCchar* helpPath) {messageBox(NotImplMsg);}
void IniFileBase::setFilePath(String& pth)         {messageBox(NotImplMsg);}
void IniFileBase::setFilePath(TCchar* pth)         {messageBox(NotImplMsg);}

#else
void IniFileBase::setAppDataPath(TCchar* helpPath) {
String  mainName = getMainName(helpPath);
Tchar    stg[1024];
HRESULT rslt;

  rslt = SHGetFolderPath(0, CSIDL_APPDATA, 0, SHGFP_TYPE_DEFAULT, stg);

  iniFilePath = stg; iniFilePath += _T('\\'); iniFilePath += mainName; iniFilePath += _T('\\');

  iniFilePath += mainName; iniFilePath += _T(".ini");

  checkPath(); setTheAppPath();
  }


void IniFileBase::setFilePath(String& pth) {iniFilePath = pth; checkPath(); setTheAppPath();}


void IniFileBase::setFilePath(TCchar* pth) {iniFilePath = pth; checkPath(); setTheAppPath();}


void IniFileBase::setTheAppPath() {

  clrTheAppPath();

  pathLng  = iniFilePath.size() + 4;

  theApp.m_pszProfileName = (LPCTSTR) malloc(pathLng*sizeof(TCchar));

  _tcscpy_s((Tchar*) theApp.m_pszProfileName, pathLng, iniFilePath);
  }


void IniFileBase::clrTheAppPath() {
TCchar*& p = theApp.m_pszProfileName;

  try {if (p) free((void*) p);} catch (...) {}

  p = 0;
  }
#endif

void IniFileBase::checkPath() {
String path = getPath(iniFilePath);

  CreateDirectory(path, 0);
  }


String IniFileBase::initIniDatum(TCchar* section, TCchar* name, TCchar* deflt, String& valu) {
  if (!readString(section, name, valu)) {valu = deflt; writeString(section, name, valu);}
  return valu;
  }


void IniFileBase::saveIniDatum(TCchar* section, TCchar* name, String& valu)
                                                        {writeString(section, name, valu);}


String IniFileBase::initIniPwd(TCchar* section, TCchar* name, TCchar* deflt, String& valu) {
  if (!readPwd(section, name, valu)) {valu = deflt; writePwd(section, name, valu);}
  return valu;
  }


void IniFileBase::saveIniPwd(TCchar* section, TCchar* name, String& valu)
                                                           {writePwd(section, name, valu);}


bool IniFileBase::writeString(TCchar* section, TCchar* key, CString& val) {
  return WritePrivateProfileString(section, key, val, iniFilePath) != 0;
  }


bool IniFileBase::write(TCchar* section, TCchar* key, CString& val) {
  return WritePrivateProfileString(section, key, val, iniFilePath) != 0;
  }


bool IniFileBase::writeString(TCchar* section, TCchar* key, TCchar* val) {
  return WritePrivateProfileString(section, key, val, iniFilePath) != 0;
  }


bool IniFileBase::write(TCchar* section, TCchar* key, TCchar*  val) {
  return WritePrivateProfileString(section, key, val, iniFilePath) != 0;
  }


bool IniFileBase::write(TCchar* section, TCchar* key, bool val) {
int v = val;   return writeInt(section, key, v);
}


bool IniFileBase::writeInt(TCchar* section, TCchar* key, int val) {
  return WritePrivateProfileString(section, key, toString(val), iniFilePath) != 0;
  }


bool IniFileBase::write(TCchar* section, TCchar* key, double val) {
  return WritePrivateProfileString(section, key, toString(val), iniFilePath) != 0;
  }


bool IniFileBase::writePwd(  TCchar* section, TCchar* key, String&  val) {
String s = val;

  return WritePrivateProfileString(section, key, encodePassword(s), iniFilePath) != 0;
  }


void IniFileBase::writeEnd() {
CFile cFile(iniFilePath, CFile::modeWrite);

  cFile.SeekToEnd();   cFile.Write("[END]\r\n", 7);   cFile.Close();
  }


void IniFileBase::deleteString(TCchar* section, TCchar* key)
                                         {WritePrivateProfileString(section, key, 0, iniFilePath);}


void IniFileBase::deleteSection(TCchar* section)
                                         {WritePrivateProfileString(section,   0, 0, iniFilePath);}


bool IniFileBase::readString( TCchar* section, TCchar* key, Cstring& val) {
Tchar stg[1024];

  if (GetPrivateProfileString(section, key, val, stg, noElements(stg), iniFilePath))
                                                                          {val = stg; return true;}
  return false;
  }


bool IniFileBase::readString(TCchar* section, TCchar* key, String& val) {
Tchar stg[1024];

  if (GetPrivateProfileString(section, key, val, stg, noElements(stg), iniFilePath))
                                                                          {val = stg; return true;}
  return false;
  }


bool IniFileBase::read(TCchar* section, TCchar* key, String&  val, TCchar* dflt)
                                                      {return readString(section, key, val, dflt);}


bool IniFileBase::readString( TCchar* section, TCchar* key, String&  val, TCchar* dflt) {
Tchar stg[1024];

  if (GetPrivateProfileString(section, key, dflt, stg, noElements(stg), iniFilePath))
                                                                          {val = stg; return true;}
  return false;
  }


bool IniFileBase::read(TCchar* section, TCchar* key, Cstring& val, TCchar* dflt)
                                                      {return readString(section, key, val, dflt);}


bool IniFileBase::readString( TCchar* section, TCchar* key, Cstring& val, TCchar* dflt) {
Tchar stg[1024];

  if (GetPrivateProfileString(section, key, dflt, stg, noElements(stg), iniFilePath))
                                                                          {val = stg; return true;}
  return false;
  }


bool IniFileBase::read(TCchar* section, TCchar* key, int& val, int dflt) {

  val = GetPrivateProfileInt(section, key, dflt, iniFilePath);

  if (val == -1) {val = 0; return false;}

  return true;
  }


bool IniFileBase::read(TCchar* section, TCchar* key, bool& val, bool dflt) {
int v;

  if (!read(section, key, v, dflt)) return false;

  val = v != 0;   return true;
  }


int IniFileBase::readInt(TCchar* section, TCchar* key, int def)
                                     {return GetPrivateProfileInt(section, key, def, iniFilePath);}


bool IniFileBase::read(TCchar* section, TCchar* key, int& val) {

  val = GetPrivateProfileInt(section, key, -1, iniFilePath);

  if (val == -1) {val = 0; return false;}

  return true;
  }


bool IniFileBase::readPwd(TCchar* section, TCchar* key, String&  val) {
Tchar  stg[64];
String s;

  if (GetPrivateProfileString(section, key, 0, stg, noElements(stg), iniFilePath))
                                                   {s = stg; val = decodePassword(s); return true;}
  return false;
  }


static const int BaseCh = ' ';
static const int Range  = 128 - BaseCh;

String IniFileBase::encodePassword(String& password) {
int    lng    = password.length();
Tchar  lower  = getRandCh();
Tchar  upper  = getRandCh();
Random r((upper << 8) | lower);
float  rng    = (float) Range;
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


Tchar IniFileBase::getRandCh() {
float rng = (float) Range;
Tchar ch;

  do {
    ch = (Tchar) (rng * rand.next() + BaseCh);

    } while (ch == _T('\\') || ch == _T('"'));

  return ch;
  }


String IniFileBase::decodePassword(String& cipher) {
int    lng    = cipher.length();   if (!lng) return cipher;
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

