// Get Version of exe file

#include "pch.h"
#include "ResourceData.h"
#include <atltime.h>

#include "MessageBox.h"


ResourceData::ResourceData() : resData(0), dataSize(0), handle(0), fileInfo(0), success(false)
                                                            {static Tchar tch;   initEntity(&tch);}



ResourceData::ResourceData(void* staticEntity) : resData(0), dataSize(0), handle(0),
                                            fileInfo(0), success(false) {initEntity(staticEntity);}


// Initialize exe or dll by static address

void ResourceData::initEntity(void* staticEntity) {
Tchar path[MAX_PATH];
HMODULE modl;

  if (!GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (Tchar*)staticEntity, &modl)) return;

  if (!GetModuleFileName(modl, path, noElements(path))) return;

  initialize(path);
  }



ResourceData::ResourceData(String& path) : resData(0), dataSize(0), handle(0),
                                                    fileInfo(0), success(false) {initialize(path);}


ResourceData::~ResourceData() {NewArray(Byte); FreeArray(resData); resData = 0; success = false;}


void ResourceData::initialize(TCchar* path) {
struct LANGANDCODEPAGE {WORD wLanguage; WORD wCodePage;} *lpTranslate;
uint   cbTranslate;
uint   lng;

  this->path = path;

  dataSize = GetFileVersionInfoSize( path, &handle);    if (!dataSize) return;

  NewArray(Byte); resData = AllocArray(dataSize);

  if (!GetFileVersionInfo(path, handle, dataSize, resData)) return;

// Read the list of languages and code pages.

  VerQueryValue(resData, _T("\\VarFileInfo\\Translation"), (LPVOID*) &lpTranslate, &cbTranslate);

  prefix.format(_T("\\StringFileInfo\\%04x%04x\\"),
                                                   lpTranslate->wLanguage, lpTranslate->wCodePage);

  if (!VerQueryValue(resData, _T("\\"), (LPVOID*) &fileInfo, &lng) || !lng) return;

  success = true;
  }


void ResourceData::threeSect(String& t) {
int i;
int pos;

  for (i = 0, pos = -1; i < 3; i++) {pos = t.find(_T('.'), ++pos);  if (pos < 0) return;}

  if (pos > 0) t = t.substr(0, pos);
  }



  // Unique ID String format for String is CompanyName.ProductName.SubProduct.VersionInformation

bool ResourceData::getAppID(String& s) {
String cn;
String pn;
String ver;

  if (!getCompanyName(cn))  return false;
  if (!getInternalName(pn)) return false;
  if (!getVersion(ver))     return false;

  s = cn + _T(".") + pn + _T("..") + ver;

  return true;
  }



bool ResourceData::getVersion(String& s) {

  s.clear();    if (!success) return false;

  if (fileInfo->dwSignature != 0xfeef04bd) return false;

  // Doesn't matter if you are on 32 bit or 64 bit, DWORD is always 32 bits, so first
  // two revision numbers come from dwFileVersionMS, last two come from dwFileVersionLS

  s.format(_T("%d.%d.%d.%d"), (fileInfo->dwFileVersionMS >> 16) & 0xffff,
                               fileInfo->dwFileVersionMS        & 0xffff,
                              (fileInfo->dwFileVersionLS >> 16) & 0xffff,
                               fileInfo->dwFileVersionLS        & 0xffff
                              );
  return true;
  }


uint ResourceData::getFileType() {return success ? fileInfo->dwFileType : VFT_UNKNOWN;}



bool ResourceData::getStg(TCchar* pat, String& s) {
UINT   size      = 0;
LPBYTE lpBuffer  = NULL;
Tchar  buf[512];

  if (!success) return false;

  if (!VerQueryValue(resData, prefix + pat, (LPVOID*) &lpBuffer, &size)) return false;

  if (!size) return false;

  _tcscpy_s(buf, noElements(buf), (Tchar*) lpBuffer);

  s = buf;  return true;
  }

