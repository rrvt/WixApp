// Dewscriptor of one Directory Element


#pragma once
#include "EntityStore.h"
#include "WixOut.h"



class DirDesc : public Data {
public:

String parent;
String name;

bool   inUse;

  DirDesc() : inUse(false) {}
  DirDesc(DirDesc& d) {copyObj(d);}
 ~DirDesc() { }

  DirDesc& operator= (DirDesc& d) {copyObj(d); return *this;}

  void clear() {id.clear(); wixID.clear(); parent.clear(); name.clear(); inUse = false;}

  String readWixData(TCchar* section, TCchar* prefix);
  void   writeWixData(TCchar* section, TCchar* prefix);

  void   store(CString& cs, TCchar* ext);
  String fullPath() {return (!parent.empty() ? parent + _T("\\") : _T("")) + name;}

  void  prepareUninstalls();

  void  output(int tab, int noSubs);

private:

  void copyObj(DirDesc& d)
                      {id = d.id;   wixID = d.wixID;  parent = d.parent; name = d.name; inUse = d.inUse;}
  };


template <class Data, const int n> class DirStore;

typedef DirStore<DirDesc, 1> DirStor;


