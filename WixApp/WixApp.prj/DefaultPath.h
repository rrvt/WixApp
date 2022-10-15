// Manage Default (i.e. last used) Paths


#pragma once
#include "Expandable.h"
#include "IterT.h"
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


class DefaultPath;
typedef IterT<DefaultPath, DefPathDsc> DPIter;


class DefaultPath {

DefPathDsc*                curPath;
Expandable <DefPathDsc, 1> paths;

public:

         DefaultPath() : curPath(0) { }
        ~DefaultPath() { }

  void   readWixData();
  void   writeWixData();

  String getPath(TCchar* key);
  void   save(   TCchar* key, String& path);

  void   del(TCchar* key);

  void   mark(TCchar* key) {DefPathDsc* dsc = find(key);  if (dsc) dsc->inUse = true;}

  int    nData() {return paths.end();}         // returns number of data items in array

private:

  DefPathDsc* find(TCchar* key);

  // returns either a pointer to data (or datum) at index i in array or zero

  DefPathDsc* datum(int i) {return 0 <= i && i < nData() ? &paths[i] : 0;}       // or data[i].p

  void  removeDatum(int i) {if (0 <= i && i < nData()) paths.del(i);}

  friend typename DPIter;

//  friend class PathDesc;
  };


extern DefaultPath defaultPath;

