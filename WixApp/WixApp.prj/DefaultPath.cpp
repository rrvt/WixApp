// Manage Default (i.e. last used) Paths


#include "stdafx.h"
#include "DefaultPath.h"
#include "filename.h"
#include "WixOut.h"


DefaultPath defaultPath;


static TCchar*   Section    = _T("DefaultPath");
static TCchar*   NPathsKey  = _T("NPaths");
static TCchar*   DefKey     = _T("DefKey%i");
static TCchar*   DscPathKey = _T("%s.Path");

static BrowseDsc browseDsc  = {_T(""), _T(""), _T(""), _T(""), _T("")};


void DefaultPath::readWixData() {
int         n = wxd.readInt(Section, NPathsKey, 99);
int         i;
String      key;
String      entity;
DefPathDsc* dsc;


  for (i = 0; i < n; i++) {
    key.format(DefKey, i);   if (!wxd.readString(Section, key, entity)) continue;
    dsc = find(entity);      if (!dsc) dsc = &paths.nextData();

    dsc->key = entity;
    key.format(DscPathKey, entity.str());   dsc->pathDsc.readWixData(browseDsc, Section, key);
    }
  }



void DefaultPath::writeWixData() {
DPIter      iter(*this);
DefPathDsc* dsc;
int         i;
int         n;
String      key;
String      entity;

  for (n = 0, dsc = iter(); dsc; dsc = iter++) if (dsc->inUse) n++;

  wxd.writeInt(Section, NPathsKey, n);

  for (i = 0, dsc = iter(); dsc; i++, dsc = iter++) {

    if (!dsc->inUse) continue;

    entity = dsc->key;

    key.format(DefKey, i);                  wxd.writeString(Section, key, entity);
    key.format(DscPathKey, entity.str());   dsc->pathDsc.writeWixData(Section, key);
    }
  }


#if 0
void DefaultPath::setKey(TCchar* key) {
  curPath = find(key);

  if (!curPath) {curPath = &paths.nextData(); curPath->key = key;}
  }
#endif


String DefaultPath::getPath(TCchar* key) {
String s;

  if (!nData()) return _T("");

  curPath = find(key);

  if (!curPath) curPath = &paths[0];

  return curPath->pathDsc.path();
  }


void DefaultPath::save(TCchar* key, String& path) {
  curPath = find(key);

  if (!curPath) curPath = &paths.nextData();

  curPath->key = key;   curPath->pathDsc = ::getPath(path);
  }


void DefaultPath::del(TCchar* key) {
DPIter      iter(*this);
DefPathDsc* dsc;

  for (dsc = iter(); dsc; dsc = iter++) if (dsc->key == key) {iter.remove(); return;}
  }


DefPathDsc* DefaultPath::find(TCchar* key) {
DPIter      iter(*this);
DefPathDsc* dsc;

  for (dsc = iter(); dsc; dsc = iter++) if (dsc->key == key) return dsc;

  return 0;
  }




#if 0
TCchar* DefaultPath::add(TCchar* key, String& fullPath) {
String      path = getPath(fullPath);
DefPathDsc* dsc;

  if (dsc = find(key)) {dsc->path = path; return dsc->path.str();}

  dsc = &paths[nPaths()];   dsc->key = key;   dsc->path = path;  return dsc->path.str();
  }
#endif

