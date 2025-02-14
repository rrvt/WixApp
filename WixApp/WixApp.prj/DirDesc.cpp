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

  ar << aClrTabs << aSetTab(3);

  ar << aTab << _T("Directory: ") << id << aCrlf;

  ar << aClrTabs << aSetTab(6) << aSetTab(30);

  ar << aTab << _T("Wix ID:")       << aTab << wixID       << aCrlf;
  ar << aTab << _T("Name:")         << aTab << name        << aCrlf;
  ar << aTab << _T("Parent:")       << aTab << parent      << aCrlf;
  ar << aTab << _T("Has Children:") << aTab << hasChildren << aCrlf;
  ar << aTab << _T("In Use:")       << aTab << inUse       << aCrlf;
  }

