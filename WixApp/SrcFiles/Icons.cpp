// Icon storage -- only one in the app


#include "stdafx.h"
#include "Icons.h"
#include "DefaultPath.h"
#include "filename.h"
#include "GetPathDlg.h"
#include "Solution.h"
#include "WixUtilities.h"


static TCchar* IconPath    = _T("IconPath");
static TCchar* IconExt     = _T("ico");
static TCchar* Icon        = _T("Icon");
static TCchar* NoIcon      = _T(" <No Icon>");

static TCchar* IconPathKey = _T("Icon");

Icons icons;


IconDesc::IconDesc() : Data(), inUse(false) {

  id = NoIcon;  setWixID();
  defaultPath.get(IconPathKey);
  }


void IconDesc::readWixData(TCchar* section) {
String path;

  wxd.readString(section, IconPath, path);

  id = getMainName(path);   setWixID();   pathDsc = path;   inUse = false;

  icons.updateList(*this);
  }


void IconDesc::writeWixData(TCchar* section) {wxd.writeString(section, IconPath, pathDsc.full());}


void IconDesc::browse() {
TCchar* defPath = defaultPath.get(IconPathKey);
String path;

  if (getPathDlg(_T("Icon"), defPath, _T("ico"), _T("*.ico"), path)) {

    defaultPath.add(IconPathKey, path);

    id = getMainName(path);   pathDsc = path;    setWixID();

    icons.updateList(*this);
    }
  }


Icons::Icons() {
IconDesc  icon;
IconDesc* p = getNew();

  icon.id = NoIcon;  icon.setWixID();  *p = icon;
  }


void Icons::oneIconAvail() {if (!iconList.nData) iconList.add(String(_T("")));}




void IconDesc::setWixID() {String s = id + Icon;  wixID = getWixID(s, IconExt);}


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


bool Icons::validate() {
ListIter  iter(iconList);
IconDesc* dsc;
bool      rslt = true;

  for (dsc = iter.startLoop(); dsc; dsc = iter.next()) if (!dsc->validate()) rslt &= false;;

  return rslt;
  }



void Icons::output(int tab) {
ListIter  iter(iconList);
IconDesc* dsc;

  for (dsc = iter.startLoop(); dsc; dsc = iter.next()) {
    dsc->outputOne(tab);
    }
  }


void IconDesc::parse(String& fullPath) {
String main = getMainName(fullPath);

  id = main; pathDsc = fullPath;
  }


bool IconDesc::validate() {
struct _stat buffer;

  if (id == NoIcon) inUse = false;

  if (!inUse || _tstat(pathDsc.full(), &buffer) == 0) return true;

  String msg = _T("Icon file not found: ") + pathDsc.full();

  MessageBox(0, msg, _T("WixApp"), MB_OK);   return false;
  }


void IconDesc::outputOne(int tab) {
String line;
String relPath = pathDsc.relative();

  if (!inUse || id == NoIcon) return;

//  solution.getRelSolution(pathDsc.full(), relPath);

  line  = _T("<Icon     Id=\"") + wixID + _T("\" SourceFile=\"") + relPath + _T("\"/>\n");
  wix.stg(tab, line);
  }





#if 0

IconDesc* Icons::update(String& id, String& path) {

  }

IconDesc* Icons::add(IconDesc& d) {
IconDesc* p;

  if (d.id.empty()) return 0;

  p = iconList.add(d.id); *p = d;  p->setWixID();   // p->wixID = getWixID(p->id, IconExt);

  p->inUse = false;

  return p;
  }
#endif

