// Dewscriptor of one Directory Element


#include "pch.h"
#include "DirDesc.h"
//#include "Directory.h"
#include "WixUtilities.h"


TCchar* ParentKy = _T("%sParent");
TCchar* ChildKey = _T("%sChild");


void DirDesc::writeWixData(TCchar* section, TCchar* prefix) {
String key;

  key.format(ParentKy, prefix);   wxd.writeString(section, key,  parent);
  key.format(ChildKey, prefix);   wxd.writeString(section, key,  name);
  }


void DirDesc::getOutput(String& line) {
  line =  _T("<Directory Id=\""); line += wixID;
  line += _T("\" Name=\"") + cleanName(name) + _T("\"");
  }

