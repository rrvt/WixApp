// Is App Available

#include "stdafx.h"
#include "AppAvail.h"


static TCchar* sRoot = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall");
//static TCchar* root2 = "SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall";



//InstalledSoftware installedSoftware;


void InstalledSoftware::init() {
  init(sRoot);
//  init(root2);
  }

void InstalledSoftware::init(TCchar* root) {
DWORD  i;
String name;
String subKey;
String dspName;

  if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, root, 0, KEY_READ, &lclMachKey)) return;

  for (i = 0; getName(i, name); i++) {

      subKey.format(_T("%s\\%s"), root, name.str());

      if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, subKey, 0, KEY_READ, &appKey))
                                                                        {RegCloseKey(appKey);  continue;}
      SoftwareDesc& desc = descs[nDescs++];   desc.name = name;  desc.root = root;

      desc.dspName = queryApp(_T("DisplayName"), dspName) ? dspName : name;

      RegCloseKey(appKey);   appKey = 0;
      }

  RegCloseKey(lclMachKey);   lclMachKey = 0;
  }


bool InstalledSoftware::getName(int i, String& name) {
Tchar buf[1024];
DWORD nBuf = noElements(buf);

  if (RegEnumKeyEx(lclMachKey, i, buf, &nBuf, NULL, NULL, NULL, NULL)) return false;

  if (nBuf < noElements(buf)) {name = buf; return true;}

  return false;
  }



bool InstalledSoftware::queryApp(TCchar* query, String& dspName) {
Byte   buf[1024];
DWORD  nBuf = noElements(buf);
DWORD  type;

  if (RegQueryValueEx(appKey, _T("DisplayName"), NULL, &type, buf, &nBuf)) return false;

  dspName = (Tchar*) buf; return true;
  }


bool InstalledSoftware::find(TCchar* appName, SoftwareDesc*& desc)
                                                {findName = appName; index = -1; return findNext(desc);}


bool InstalledSoftware::findNext(SoftwareDesc*& desc) {

  for (index++; index < nDescs; index++) {
    SoftwareDesc& dsc = descs[index];

    if (dsc.dspName == findName) {desc = &dsc; return true;}
    }

  desc = 0; return false;
  }


bool InstalledSoftware::delKey(SoftwareDesc& desc) {
HKEY   machKey;
DWORD  rslt;
Tchar  buf[512];

  if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, desc.root, 0, KEY_READ, &machKey)) return false;

  rslt = RegDeleteKey(machKey, desc.name);    RegCloseKey(machKey);

  if (!rslt) return true;

  FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, rslt, 0, buf, noElements(buf), 0);

  return false;
  }

