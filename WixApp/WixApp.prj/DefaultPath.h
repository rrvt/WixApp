// Manage Default (i.e. last used) Paths


#pragma once
#include "ExpandableP.h"
#include "IterT.h"
#include "RelPath.h"


class DefPathDsc {
String  key;
RelPath relPath;
bool    inUse;

public:

  DefPathDsc() : inUse(false) { }
  DefPathDsc(DefPathDsc& dsc) {copyDsc(dsc);}
 ~DefPathDsc() { }

  DefPathDsc& operator= (DefPathDsc& dsc) {copyDsc(dsc); return *this;}

  void   saveData(Archive& ar);

private:

  void copyDsc(DefPathDsc& dsc) {key = dsc.key; relPath = dsc.relPath; inUse = dsc.inUse;}
  friend class DefaultPath;
  };


typedef DatumPtrT<DefPathDsc, String> DefPathP;             // Usually defined just before iterator
class DefaultPath;
typedef IterT<DefaultPath, DefPathDsc> DPIter;


class DefaultPath {

DefPathDsc*                                   curPath;
ExpandableP <DefPathDsc, String, DefPathP, 1> data;

public:

         DefaultPath() : curPath(0) { }
        ~DefaultPath() { }

  void   clearMarks();

  void   readWixData();
  void   writeWixData();

  String getPath(TCchar* key);
  void   save(   TCchar* key, String& path);

  void   del(TCchar* key);

  void   mark(TCchar* key) {DefPathDsc* dsc = find(key);  if (dsc) dsc->inUse = true;}

  int    nData() {return data.end();}         // returns number of data items in array

  void   saveData(Archive& ar);

private:

  DefPathDsc* find(TCchar* key);

  // returns either a pointer to data (or datum) at index i in array or zero

  DefPathDsc* datum(int i) {return 0 <= i && i < nData() ? data[i] : 0;}       // or data[i].p

  void  removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

  friend typename DPIter;
  };


extern DefaultPath defaultPath;

