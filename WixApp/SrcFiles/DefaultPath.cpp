// Manage Default (i.e. last used) Paths


#include "stdafx.h"
#include "DefaultPath.h"
#include "filename.h"
#include "WixOut.h"


DefaultPath defaultPath;


static TCchar* Section = _T("DefaultPath");



TCchar* DefaultPath::get(TCchar* key) {
DefPathDsc* dsc;
String      s;

  if (dsc = find(key)) return dsc->path.str();

  return wxd.readString(Section, key, s) ? add(key, s) : 0;
  }



int DefaultPath::getInt(TCchar* key) {
int         val;
DefPathDsc* dsc;

  if (dsc = find(key)) return dsc->val;

  val = wxd.readInt(Section, key, 0);   add(key, val);   return val;
  }


TCchar* DefaultPath::add(TCchar* key, String& fullPath) {
String      path = getPath(fullPath);
DefPathDsc* dsc;

  if (dsc = find(key)) {dsc->path = path; return dsc->path.str();}

  dsc = &paths[nPaths++];

  dsc->key = key;   dsc->path = path;  dsc->dscType = StgType;   return dsc->path.str();
  }


int DefaultPath::add(TCchar* key, int v) {
DefPathDsc* dsc;

  if (dsc = find(key)) {dsc->val = v; return dsc->val;}

  dsc = &paths[nPaths++];   dsc->key = key;   dsc->val = v;  dsc->dscType = IntType;   return dsc->val;
  }


void DefaultPath::writeWixData() {
int i;

  for (i = 0; i < nPaths; i++) {
    DefPathDsc& dsc = paths[i];

    switch (dsc.dscType) {
      case StgType: wxd.writeString(Section, dsc.key, dsc.path); break;
      case IntType: wxd.writeInt(   Section, dsc.key, dsc.val);  break;
      }
    }
  }


DefPathDsc* DefaultPath::find(TCchar* key) {
int i;

  for (i = 0; i < nPaths; i++) {
    DefPathDsc& dsc = paths[i];   if (dsc.key == key) return &dsc;
    }

  return 0;
  }

