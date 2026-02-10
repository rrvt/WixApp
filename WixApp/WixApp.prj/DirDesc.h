// Dewscriptor of one Directory Element


#pragma once
#include "EntityStore.h"
#include "WixOut.h"

class Archive;


extern TCchar* ParentKy;
extern TCchar* ChildKey;

extern TCchar* DDID;

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

  bool isValid() {return inUse && !id.isEmpty();}

  void writeWixData(TCchar* section, TCchar* prefix);

  void prepareUninstalls();

  void getOutput(String& line);

  void readWixData(String& section);

  bool writeWixData(String& section);

  void saveData(Archive& ar);

private:

  void copyObj(DirDesc& d) {id = d.id;   wixID = d.wixID;  parent = d.parent; name = d.name;
                                                     hasChildren = d.hasChildren; inUse = d.inUse;}
  };


template <class Data, const int n> class DirStore;

typedef DirStore<DirDesc, 1> DirStor;


