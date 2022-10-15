// Icon storage -- only one in the app


#include "stdafx.h"
#include "Icons.h"
#include "DefaultPath.h"
#include "filename.h"
#include "WixUtilities.h"


static TCchar*    IconsSection = _T("Icons");
static TCchar*    NoKeys       = _T("NoIcons");
static TCchar*    IconSection  = _T("Icon%02i");
static TCchar*    IconIDKey    = _T("IconID");
static TCchar*    WixIDKey     = _T("WixID");

static TCchar*    IconPath     = _T("IconPath");
static TCchar*    IconPathKey  = _T("Icon");

static BrowseDsc  browseDsc    = {IconPathKey, _T("Icon File"), _T(""), _T("ico"), _T("*.ico")};



Icons icons;


void Icons::oneIconAvail() {if (!nIcons()) iconList.add(String(_T("")));}


void Icons::readWixData() {
int       n = wxd.readInt(IconsSection, NoKeys, 99);
int       i;
String    section;
String    id;
IconDesc* dsc;

  for (i = 0; i < n; i++) {

    section.format(IconSection, i);

    if (!wxd.readString(section, IconIDKey, id)) continue;

    dsc = iconList.add(id);  dsc->readWixData(section);
    }
  }


void IconDesc::readWixData(TCchar* section) {
String path;

  wxd.readString(section, WixIDKey, wixID);

  pathDsc.readWixData(browseDsc, section, IconPath);   inUse = false;
  }


void Icons::writeWixData() {
ListIter  iter(iconList);
int       nToWrite;
String    section;
IconDesc* dsc;
int       i;

  for (nToWrite = 0, dsc = iter(); dsc; dsc = iter++) if (dsc->inUse && !dsc->id.isEmpty()) nToWrite++;

  wxd.writeInt(IconsSection, NoKeys, nToWrite);

  for (i = 0, dsc = iter(); dsc; dsc = iter++)
    if (dsc->inUse && !dsc->id.isEmpty())
                                      {section.format(IconSection, i); dsc->writeWixData(section); i++;}
  }


void IconDesc::writeWixData(TCchar* section) {
  wxd.writeString(section, IconIDKey, id);
  wxd.writeString(section, WixIDKey,  wixID);
  pathDsc.writeWixData(section, IconPath);
  }


String Icons::browse(bool isStartupApp) {
PathDesc  pathDsc;
String    path;
String    id;
IconDesc* dsc;

  path = pathDsc.browse(browseDsc);

  if (!path.isEmpty()) {

    id = getMainName(path);

    if (!id.isEmpty()) {

      dsc = find(id);

      if (!dsc) dsc = iconList.addNil(_T("Icon"));

      dsc->id = id;

      dsc->setWixID(isStartupApp);

      *dsc = pathDsc;
      }
    }

  return id;
  }


void IconDesc::setWixID(bool isStartupApp) {
String s   = id;

  s += isStartupApp ? _T("_Startup_App") : _T("_Icon");

  wixID = getWixID(s, _T("exe"));
  }


void Icons::markDfltDir() {defaultPath.mark(IconPathKey);}


bool Icons::getFromList(IconDesc& icon) {
String    id = icon.id;
IconDesc* p;

  if (id.isEmpty()) return false;

  p = find(id);

  if (p) {icon = *p; return true;}

  p = getNew();  *p = icon;   return true;
  }



bool Icons::updateList(IconDesc& icon) {
String    id = icon.id;
IconDesc* p;

  if (id.isEmpty()) return false;

  p = find(id);

  if (p) {*p = icon; return true;}

  p = getNew(); *p = icon; return true;
  }


bool Icons::validate(bool rptErrors) {
ListIter  iter(iconList);
IconDesc* dsc;
bool      rslt = true;

  for (dsc = iter(); dsc; dsc = iter++) if (!dsc->validate(rptErrors)) rslt &= false;;

  return rslt;
  }



void Icons::output() {
ListIter  iter(iconList);
IconDesc* dsc;

  for (dsc = iter(); dsc; dsc = iter++) {
    dsc->outputOne();
    }
  }


bool IconDesc::validate(bool rptErrors) {
struct _stat buffer;

  if (!inUse || _tstat(pathDsc.path(), &buffer) == 0 || !rptErrors) return true;

  String msg = _T("Icon file not found: ") + pathDsc.path();

  MessageBox(0, msg, _T("WixApp"), MB_OK);   return false;
  }


void IconDesc::outputOne() {
String line;
String relPath = pathDsc.relative();

  line  = _T("<Icon     Id=\"") + wixID + _T("\" SourceFile=\"") + relPath + _T("\"/>");
  wix(line);   wix.crlf();
  }

