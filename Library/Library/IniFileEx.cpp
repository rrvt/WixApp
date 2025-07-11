


#include "pch.h"
#include "IniFileEx.h"
#include "fileName.h"



void IniFileEx::setAppDataPath(TCchar* helpPath) {
String  mainName = getMainName(helpPath);
Tchar    stg[1024];
HRESULT rslt;

  rslt = SHGetFolderPath(0, CSIDL_APPDATA, 0, SHGFP_TYPE_DEFAULT, stg);

  iniFilePath = stg; iniFilePath += _T('\\'); iniFilePath += mainName; iniFilePath += _T('\\');

  iniFilePath += mainName; iniFilePath += _T(".ini");

  checkPath(); setTheAppPath();
  }


void IniFileEx::setFilePath(String& pth) {iniFilePath = pth; checkPath(); setTheAppPath();}


void IniFileEx::setFilePath(TCchar* pth) {iniFilePath = pth; checkPath(); setTheAppPath();}


void IniFileEx::setTheAppPath() {

  clrTheAppPath();

  pathLng  = iniFilePath.size() + 4;

  theApp.m_pszProfileName = (LPCTSTR) malloc(pathLng*sizeof(TCchar));

  _tcscpy_s((Tchar*) theApp.m_pszProfileName, pathLng, iniFilePath);
  }


void IniFileEx::clrTheAppPath() {
TCchar*& p = theApp.m_pszProfileName;

  try {if (p) free((void*) p);} catch (...) {}

  p = 0;
  }

