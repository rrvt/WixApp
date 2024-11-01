// Manage Default (i.e. last used) Paths


#include "pch.h"
#include "DefaultPath.h"
#include "filename.h"
#include "Solution.h"
#include "WixOut.h"


DefaultPath defaultPath;

static TCchar*   Section    = _T("DefaultPath");
static TCchar*   NPathsKey  = _T("NPaths");
static TCchar*   DefKey     = _T("DefKey%i");
static TCchar*   DscPathKey = _T("%s.Path");

static KeyedPathDsc browseDsc  = {_T(""), _T(""), _T(""), _T(""), _T("")};


void DefaultPath::readWixData() {
int         n = wxd.readInt(Section, NPathsKey, 99);
int         i;
String      key;
String      entity;
DefPathDsc* dsc;

  for (i = 0; i < n; i++) {
    key.format(DefKey, i);   if (!wxd.readString(Section, key, entity)) continue;
    dsc = find(entity);      if (!dsc) dsc = &data.nextData();

    dsc->key = entity;
    key.format(DscPathKey, entity.str());   dsc->relPath.readWixData(Section, key, browseDsc);
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
    key.format(DscPathKey, entity.str());   dsc->relPath.writeWixData(Section, key);
    }
  }


String DefaultPath::getPath(TCchar* key) {
String s;

  curPath = find(key);

  if (!curPath) return solution.getRootPath();  // curPath = &paths[0];

  return curPath->relPath.localPath();
  }


void DefaultPath::save(TCchar* key, String& path) {
  curPath = find(key);

  if (!curPath) curPath = &data.nextData();

  curPath->key = key;   curPath->relPath = ::getPath(path);
  }


DefPathDsc* DefaultPath::find(TCchar* key) {
DPIter      iter(*this);
DefPathDsc* dsc;

  for (dsc = iter(); dsc; dsc = iter++) if (dsc->key == key) return dsc;

  return 0;
  }


void DefaultPath::del(TCchar* key) {
DPIter      iter(*this);
DefPathDsc* dsc;

  for (dsc = iter(); dsc; dsc = iter++) if (dsc->key == key) {iter.remove(); return;}
  }


void DefaultPath::saveData(Archive& ar) {
DPIter      iter(defaultPath);
DefPathDsc* dsc;
String      k;

  ar << _T("Default Path") << aCrlf;

  for (dsc = iter(); dsc; dsc = iter++) {
    ar.tab(1);  ar << dsc->key;   ar.tab(7);  dsc->relPath.saveData(ar);
    ar.tab(15);   k = dsc->inUse;   ar << k << aCrlf;
    }
  }


