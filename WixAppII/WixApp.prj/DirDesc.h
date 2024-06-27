// Dewscriptor of one Directory Element


#pragma once
#include "EntityStore.h"
#include "WixOut.h"


extern TCchar* ParentKy;
extern TCchar* ChildKey;


class DirDesc : public Data {
public:

String parent;
String name;
bool   hasChildren;
bool   inUse;

  DirDesc() : hasChildren(false), inUse(false) {}
  DirDesc(DirDesc& d) {copyObj(d);}
 ~DirDesc() { }

  DirDesc& operator= (DirDesc& d) {if (&d) copyObj( d); else clear(); return *this;}
  DirDesc& operator= (DirDesc* d) {if  (d) copyObj(*d); else clear(); return *this;}

  void clear()
     {id.clear(); wixID.clear(); parent.clear(); name.clear(); hasChildren = false; inUse = false;}

  void   writeWixData(TCchar* section, TCchar* prefix);

  void  prepareUninstalls();

  void  getOutput(String& line);

private:

  void copyObj(DirDesc& d) {id = d.id;   wixID = d.wixID;  parent = d.parent; name = d.name;
                                                     hasChildren = d.hasChildren; inUse = d.inUse;}
  };


template <class Data, const int n> class DirStore;

typedef DirStore<DirDesc, 1> DirStor;


