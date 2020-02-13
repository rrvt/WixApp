// Dewscriptor of one Directory Element


#include "stdafx.h"
#include "DirDesc.h"
#include "Directory.h"
#include "WixUtilities.h"


static TCchar* ParentKy = _T("%sParent");
static TCchar* ChildKey = _T("%sChild");


String DirDesc::readWixData(TCchar* section, TCchar* prefix) {
String key;

  key.format(ParentKy, prefix);   wxd.readString(section, key,  parent);
  key.format(ChildKey, prefix);   wxd.readString(section, key,  name);

  return id = fullPath();   inUse = false;
  }


void DirDesc::writeWixData(TCchar* section, TCchar* prefix) {
String key;

  key.format(ParentKy, prefix);   wxd.writeString(section, key,  parent);
  key.format(ChildKey, prefix);   wxd.writeString(section, key,  name);
  }



void DirDesc::output(int tab, int noSubs) {
String line;

  line = _T("<Directory Id=\""); line += wixID; line += _T("\" Name=\"") + cleanName(name) + _T("\"");

  if (!noSubs) line += _T("/");   line += _T(">\n");   wix.stg(tab, line);
  }

