// Manage Default (i.e. last used) Paths


#pragma once
#include "Expandable.h"
#include "PathDesc.h"

struct DefPathDsc {
String         key;
PathDesc       pathDsc;
bool           inUse;

  DefPathDsc() : inUse(false) { }
  DefPathDsc(DefPathDsc& dsc) {copyDsc(dsc);}
 ~DefPathDsc() { }

  DefPathDsc& operator= (DefPathDsc& dsc) {return copyDsc(dsc);}

private:

  DefPathDsc& copyDsc(DefPathDsc& dsc)
                                {key = dsc.key; pathDsc = dsc.pathDsc; inUse = dsc.inUse; return *this;}
  };


class DefaultPath {

DefPathDsc*                curPath;
Expandable <DefPathDsc, 1> paths;

public:

  DefaultPath() : curPath(0) { }
 ~DefaultPath() { }

  void        readWixData();
  void        writeWixData();

  void        setCurPath(TCchar* key);
  void        del(TCchar* key);

  void        mark(TCchar* key) {DefPathDsc* dsc = find(key);  if (dsc) dsc->inUse = true;}

private:

  String      getCurPath();
  void        save(String& path);

  int         nPaths() {return paths.end();}
  DefPathDsc* find(TCchar* key);

  friend class PathDesc;
  };


extern DefaultPath defaultPath;

