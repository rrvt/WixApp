// Manage Default (i.e. last used) Paths


#include "stdafx.h"
#include "DefaultPath.h"
#include "filename.h"
#include "WixOut.h"


DefaultPath defaultPath;


static TCchar* Section    = _T("DefaultPath");
static TCchar* NPathsKey  = _T("NPaths");
static TCchar* DefKey     = _T("DefKey%i");
static TCchar* DscPathKey = _T("%s.Path");


void DefaultPath::readWixData() {
int         n = wxd.readInt(Section, NPathsKey, 99);
int         i;
String      key;
String      entity;
DefPathDsc* dsc;


  for (i = 0; i < n; i++) {
    key.format(DefKey, i);   if (!wxd.readString(Section, key, entity)) continue;
    dsc = find(entity);      if (!dsc) dsc = &paths[nPaths()];

    dsc->key = entity;
    key.format(DscPathKey, entity.str());   dsc->pathDsc.readWixData(Section, key);
    }
  }



void DefaultPath::writeWixData() {
int    i;
String key;
String entity;

  wxd.writeInt(Section, NPathsKey, nPaths());

  for (i = 0; i < nPaths(); i++) {
    DefPathDsc& dsc = paths[i];

    if (!dsc.inUse) continue;

    entity = dsc.key;

    key.format(DefKey, i);                  wxd.writeString(Section, key, entity);
    key.format(DscPathKey, entity.str());   dsc.pathDsc.writeWixData(Section, key);
    }
  }


void DefaultPath::setCurPath(TCchar* key)
                 {curPath = find(key);   if (!curPath) {curPath = &paths[nPaths()]; curPath->key = key;}}


String DefaultPath::getCurPath() {
String s = _T("");

  if (!curPath && nPaths()) curPath = &paths[0];
  if (!curPath) return s;

  s = curPath->pathDsc.path();   return s;
  }


void DefaultPath::save(String& path) {if (curPath) {curPath->pathDsc = getPath(path);}}


void DefaultPath::del(TCchar* key) {
int i;
int n = nPaths();

  for (i = 0; i < n; i++) if (paths[i].key == key) {paths.del(i); return;}
  }



#if 0
TCchar* DefaultPath::add(TCchar* key, String& fullPath) {
String      path = getPath(fullPath);
DefPathDsc* dsc;

  if (dsc = find(key)) {dsc->path = path; return dsc->path.str();}

  dsc = &paths[nPaths()];   dsc->key = key;   dsc->path = path;  return dsc->path.str();
  }
#endif


DefPathDsc* DefaultPath::find(TCchar* key) {
int i;

  for (i = 0; i < nPaths(); i++) {
    DefPathDsc& dsc = paths[i];   if (dsc.key == key) return &dsc;
    }

  return 0;
  }

