// Dewscriptor of one Directory Element


#include "pch.h"
#include "DirDesc.h"
#include "Archive.h"
#include "WixUtilities.h"


TCchar* ParentKy = _T("%sParent");
TCchar* ChildKey = _T("%sChild");

       TCchar* DDID          = _T("ID");
static TCchar* DDWixID       = _T("WixID");
static TCchar* DDParent      = _T("Parent");
static TCchar* DDName        = _T("Name");
static TCchar* DDHasChildren = _T("HasChildren");


void DirDesc::writeWixData(TCchar* section, TCchar* prefix) {
String key;

  key.format(ParentKy, prefix);   wxd.writeString(section, key,  parent);
  key.format(ChildKey, prefix);   wxd.writeString(section, key,  name);
  }


void DirDesc::getOutput(String& line) {
  line =  _T("<Directory Id=\""); line += wixID;
  line += _T("\" Name=\"") + cleanName(name) + _T("\"");
  }

void DirDesc::readWixData(String& section) {
  wxd.readString(section, DDWixID,  wixID);
  wxd.readString(section, DDParent, parent);
  wxd.readString(section, DDName,   name);
  hasChildren = wxd.readInt(section, DDHasChildren, 0);  inUse = false;
  }


bool DirDesc::writeWixData(String& section) {

  if (!isValid()) return false;

  wxd.writeString(section, DDID,          id);
  wxd.writeString(section, DDWixID,       wixID);
  wxd.writeString(section, DDParent,      parent);
  wxd.writeString(section, DDName,        name);
  wxd.writeInt(   section, DDHasChildren, hasChildren);
  return true;
  }


void  DirDesc::saveData(Archive& ar) {
String k;

  ar.tab(1);   ar << _T("Name: ");   ar << name << aCrlf;
  ar.tab(2);   ar << _T("ID:");                                ar.tab(8);   ar << id << aCrlf;
  ar.tab(2);   ar << _T("Wix ID:");                            ar.tab(8);   ar << wixID << aCrlf;
  ar.tab(2);   ar << _T("Parent:");                            ar.tab(8);   ar << parent << aCrlf;
  ar.tab(2);   ar << _T("Has Children:");   k = hasChildren;   ar.tab(8);   ar << k << aCrlf;
  ar.tab(2);   ar << _T("In Use:");         k = inUse;         ar.tab(8);   ar << k << aCrlf;
  }

