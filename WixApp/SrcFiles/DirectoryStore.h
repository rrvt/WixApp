// Directory Store -- An Extension of the Entity Store


#pragma once
#include "Feature.h"
#include "WixUtilities.h"



template <class Data, const int n>

class DirStore : public EntityStore<Data, n> {
public:

String ext;

  DirStore(TCchar* extension) : ext(extension) {}
 ~DirStore() { }

  Data* add(String& fullPath);

  Data* getDefault() {EStoreIter<Data, n> iter(*this); return iter();} //

  void  outputSubs(String& parent, int tab);
  int   noSubs(String& parent);
  void  outputRemoves(int tab);

  void  prepareUninstalls(Feature* ftr);

  void  readWixData();
  void  writeWixData();

private:
  Data* addOne(String fullPath);
  void  readDesc(String& section);
  void  writeDesc(   DirDesc& dsc, String& section);
  void  outputOne(   DirDesc* dsc, int tab);
  void  outputParent(DirDesc* dsc, int tab);


  DirStore() {}
  };


template <class Data, const int n>
class DirStoreIter : public EStoreIter<Data, n> {
public:

  DirStoreIter(DirStore<Data,n>& store) : EStoreIter<Data, n>(store) {}
 ~DirStoreIter() { }

private:

  DirStoreIter() {}
  };


typedef DirStoreIter<DirDesc, 1> DirStorIter;



template <class Data, const int n>
Data* DirStore<Data, n>::add(String& fullPath) {
Data* dsc;

  fullPath.trim();

  for (dsc = DirStore<Data, n>::addOne(fullPath); dsc; dsc = DirStore<Data, n>::addOne(dsc->parent))
                                                                                                continue;
  return find(fullPath);
  }


template <class Data, const int n>
Data* DirStore<Data, n>::addOne(String fullPath) {
Data* dsc;

  if (fullPath.isEmpty()) return 0;

  dsc = EntityStore<Data, n>::add(fullPath);  dsc->wixID = getWixID(fullPath, ext);  dsc->inUse = false;

  findLastName(fullPath, dsc->parent, dsc->name);   return dsc;
  }




template <class Data, const int n>
void DirStore<Data, n>::outputSubs(String& parent, int tab) {
DirStorIter iter(*this);
DirDesc*    dsc;

  for (dsc = iter(); dsc; dsc = iter++) {             // .startLoop .nextItem()

if (dsc->parent == _T("MakeApp\\TestApp.prj") || dsc->id == _T("MakeApp\\TestApp.prj")) {
int x = 1;
}

    if (dsc->inUse && dsc->parent == parent) {

      if (!dsc->hasChildren) outputOne(dsc, tab);
      else {
        outputParent(dsc, tab);

        String s = dsc->parent.isEmpty() ? dsc->name : dsc->parent + _T("\\") + dsc->name;

        outputSubs(s, tab+1);

        wix.lit(tab, _T("</Directory>\n"));
        }
      }
    }
  }


template <class Data, const int n>
void DirStore<Data, n>::outputOne(DirDesc* dsc, int tab) {
String line;

  dsc->getOutput(line); line += _T(" />\n");   wix.stg(tab, line);
  }


template <class Data, const int n>
void DirStore<Data, n>::outputParent(DirDesc* dsc, int tab) {
String line;

  dsc->getOutput(line);  line += _T(">\n"); wix.stg(tab, line);
  }


template <class Data, const int n>
int DirStore<Data, n>::noSubs(String& parent) {
DirStorIter iter(*this);
DirDesc*    dsc;
int         no = 0;

  for (dsc = iter(); dsc; dsc = iter++) {
    if (dsc->parent == parent) no++;
    }

  return no;
  }


template <class Data, const int n>
void DirStore<Data, n>::outputRemoves(int tab) {
DirStorIter iter(*this);
DirDesc*    dsc;
String      uniID;
String      line;

  for (dsc = iter(); dsc; dsc = iter++) {

    if (!dsc->inUse) continue;

    uniID = getWixID(dsc->id, _T("uni"));
    line = _T("<RemoveFolder Id=\"") + uniID + _T("\" Directory=\"");
    wix.out(tab, line, dsc->wixID, _T("\" On=\"uninstall\"/>"));
    }
  }


template <class Data, const int n>
void DirStore<Data, n>::prepareUninstalls(Feature* ftr) {
DirStorIter iter(*this);
DirDesc*    dsc;
Component*  cmp;

  for (dsc = iter(); dsc; dsc = iter++) {

    if (dsc->id.isEmpty()) continue;

    cmp = ftr->newItem();

    cmp->prepareUninstalls(dsc->id, getWixID(cmp->id, _T("uni")), dsc->id);
    }
  }

//static TCchar* DLSection     = _T("%sDirectoryList");
static TCchar* NoKeys        = _T("NoDirectories");
static TCchar* DirDescSect   = _T("%sDirDesc%02i");
static TCchar* DDID          = _T("ID");
static TCchar* DDWixID       = _T("WixID");
static TCchar* DDParent      = _T("Parent");
static TCchar* DDName        = _T("Name");
static TCchar* DDHasChildren = _T("HasChildren");

static TCchar* DfltSection   = _T("%sDirectories");


template <class Data, const int n>
void DirStore<Data, n>::readWixData() {
int      n;
String   section;
String   e = ext;
int      i;

  e.upperCase();   section.format(DfltSection, e.str());

  n = wxd.readInt(section, NoKeys, 99);

  for (i = 0; i < n; i++) {

    section.format(DirDescSect, e.str(), i);

    readDesc(section);
    }

  }


template <class Data, const int n>
void DirStore<Data, n>::readDesc(String& section) {
String   id;
DirDesc* dsc;

  if (!wxd.readString(section, DDID, id)) return;

  dsc = newItem(id);   dsc->wixID = getWixID(id, ext);   dsc->inUse = false;
//  wxd.readString(section, DDWixID,  dsc->wixID);
  wxd.readString(section, DDParent, dsc->parent);
  wxd.readString(section, DDName,   dsc->name);
  dsc->hasChildren = wxd.readInt(section, DDHasChildren, 0);
  }



template <class Data, const int n>
void DirStore<Data, n>::writeWixData() {
EStoreIter<Data, n> iter(*this);
int                 nToWrite;
String              section;
String              e = ext;
DirDesc*            dsc;
int                 i;

  for (nToWrite = 0, dsc = iter(); dsc; dsc = iter++) if (dsc->inUse && !dsc->id.isEmpty()) nToWrite++;

  e.upperCase();   section.format(DfltSection, e.str());

  wxd.writeInt(section, NoKeys, nToWrite);

  for (i = 0, dsc = iter(); dsc; dsc = iter++, i++) {

    section.format(DirDescSect, e.str(), i);

    if (dsc->inUse && !dsc->id.isEmpty()) writeDesc(*dsc, section);;                     //dsc->inUse &&
    }
  }


template <class Data, const int n>
void DirStore<Data, n>::writeDesc(DirDesc& dsc, String& section) {

  wxd.writeString(section, DDID,          dsc.id);
  wxd.writeString(section, DDWixID,       dsc.wixID);
  wxd.writeString(section, DDParent,      dsc.parent);
  wxd.writeString(section, DDName,        dsc.name);
  wxd.writeInt(   section, DDHasChildren, dsc.hasChildren);
  }



#if 0
template <class Data, const int n>
void DirStore<Data, n>::markDir(String& id) {
DirDesc* dsc;

  if (!id.isEmpty()) {dsc = find(id);  if (dsc) dsc->inUse = true;}
  }
#endif


#if 0
    cmp->id             = dsc->id;
    cmp->wixID          = getWixID(cmp->id, _T("uni"));
    cmp->progFileID     = dsc->id;
    cmp->isUninstallDir = true;
#endif

