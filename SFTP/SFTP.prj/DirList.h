// Directory List subject to a commom action


#pragma once
#include "Expandable.h"
#include "IterT.h"
#include "Utilities.h"


class SiteFileDsc;


struct DirItem{

int          level;
SiteFileDsc* dsc;

  DirItem() : level(0), dsc(0) { }
  DirItem(DirItem& item) {copy(item);}
 ~DirItem() { }

  DirItem& operator= (DirItem& item) {copy(item); return *this;}

  bool operator>= (DirItem& item) {return level <= item.level;}
  bool operator== (DirItem& item) {return level == item.level;}

private:

  void copy(DirItem& item) {level = item.level;   dsc= item.dsc;}
  };


class DirList;
typedef IterT<DirList, DirItem> DLIter;

class DirList {

Expandable<DirItem, 2> data;

public:

  DirList() { }
 ~DirList() { }

  void add(SiteFileDsc* dsc);
//                      {DirItem item;   item.path = path;   item.level = pathLevel(path);   data = item;}

  int  nData() {return data.end();}                       // returns number of data items in array

private:

  // returns either a pointer to datum at index i in array or zero

  DirItem* datum(int i) {return 0 <= i && i < nData() ? &data[i] : 0;}

  void     removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

  friend typename DLIter;
  };

