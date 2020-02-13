// Directory Store -- An Extension of the Entity Store


#pragma once
#include "Group.h"



template <class Data, const int n>

class DirStore : public EntityStore<Data, n> {
public:

String ext;

  DirStore(TCchar* extension) : ext(extension) {}
 ~DirStore() { }

  DirDesc* add(String& fullPath);

  DirDesc* getDefault() {EStoreIter<Data, n> iter(*this); return iter.startLoop();}

  void     identifyDir(String& id);

  void     outputSubs(String& parent, int tab);
  int      noSubs(String& parent);
  void     outputRemoves(int tab);

  void     prepareUninstalls(Group* grp);

private:
  DirDesc* addOne(String fullPath);
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
DirDesc* DirStore<Data, n>::add(String& fullPath) {

DirDesc* dsc = DirStore<Data, n>::addOne(fullPath);

  if (!dsc->parent.empty()) DirStore<Data, n>::addOne(dsc->parent);

  return find(fullPath);
  }


template <class Data, const int n>
DirDesc* DirStore<Data, n>::addOne(String fullPath) {
DirDesc* dsc;

  dsc = EntityStore<Data, n>::add(fullPath);

  dsc->wixID = getWixID(fullPath, ext);    dsc->inUse = false;

  parse(fullPath, dsc->parent, dsc->name);   return dsc;
  }



template <class Data, const int n>
void DirStore<Data, n>::identifyDir(String& id) {
DirDesc* dsc;

  if (!id.empty()) {dsc = find(id);  if (dsc) dsc->inUse = true;}
  }



template <class Data, const int n>
void DirStore<Data, n>::outputSubs(String& parent, int tab) {
DirStorIter iter(*this);
DirDesc*    dsc;
int         no;

  for (dsc = iter.startLoop(); dsc; dsc = iter.next()) {
    if (dsc->inUse && dsc->parent == parent) {
      no = noSubs(dsc->name);

      dsc->output(tab, no);

      outputSubs(dsc->name, tab+1);

      if (no) wix.lit(tab, _T("</Directory>\n"));
      }
    }
  }


template <class Data, const int n>
int DirStore<Data, n>::noSubs(String& parent) {
DirStorIter iter(*this);
DirDesc*    dsc;
int         no = 0;

  for (dsc = iter.startLoop(); dsc; dsc = iter.next()) {
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

  for (dsc = iter.startLoop(); dsc; dsc = iter.next()) {

    if (!dsc->inUse) continue;

    uniID = getWixID(dsc->id, _T("uni"));
    line = _T("<RemoveFolder Id=\"") + uniID + _T("\" Directory=\"");
    wix.out(tab, line, dsc->wixID, _T("\" On=\"uninstall\"/>"));
    }
  }


template <class Data, const int n>
void DirStore<Data, n>::prepareUninstalls(Group* grp) {
DirStorIter iter(*this);
DirDesc*    dsc;
Component*  cmp;

  for (dsc = iter.startLoop(); dsc; dsc = iter.next()) {

    if (dsc->id.empty()) continue;

    cmp = grp->newItem();

    cmp->id             = dsc->id;
    cmp->wixID          = getWixID(cmp->id, _T("uni"));
    cmp->progFile       = *dsc;
    cmp->startMenu      = *dsc;
    cmp->isUninstallDir = true;
    }
  }

