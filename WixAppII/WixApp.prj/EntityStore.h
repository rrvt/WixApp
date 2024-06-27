// Entity Store


#pragma once
#include "ComboBox.h"
#include "Expandable.h"
#include "MessageBox.h"


template <class Data, const int n> class EntityStore {
public:

String               curID;
Expandable <Data, n> data;

  EntityStore() {}
  EntityStore(EntityStore& es) {copyObj(es);}
 ~EntityStore() { }

  EntityStore& operator= (EntityStore& es) {copyObj(es); return *this;}

  void  clear() {data.clear(); curID.clear();}

  Data* curData() {return curID.isEmpty() ? 0 : findItem(curID);}

  void  defaultCurID(TCchar* anID);
  Data* addNil( TCchar* name);
  void  delNil( TCchar* name);
  Data* add(    TCchar* id);
  void  delItem(TCchar* id);

  Data* findItem(TCchar* id);
  Data* findItem(String& id) {TCchar* p = id; return findItem(p);}

  void  editUpdate(ComboBox& cb);
  void  loadCB(    ComboBox& cb);
  bool  add(String& id, ComboBox& cb)
                                  {if (id[0] == _T('<')) return false;   cb.add(id); return true;}
  Data* storeCB(   ComboBox& cb);

  void  setCur(TCchar* tc, ComboBox& cb);   // Set current icon in combobox to s
  void  setNewCur(ComboBox& cb);            // Respond to change in combo box
  void  leavingCB(ComboBox& cb);            // Leaving combo box, check for a new name in edit box
  void  delItem(  ComboBox& cb);

private:

  String nilName(TCchar* name);
  Data*  newItem(TCchar* id);
  void   setCurData(String& id);
  void   delData();
  void   copyObj(EntityStore& es);
  };


template <class Data, const int n>
class EStoreIter {
EntityStore<Data,n>& stor;
int                  i;
int                  nn;
public:

  EStoreIter(EntityStore<Data,n>& store) : stor(store), i(0), nn(stor.data.end()) {}
 ~EStoreIter() { }

  Data* operator() ()     {i = 0; return retNext();}
  Data* operator++  (int) {i++;   return retNext();}

private:

  Data* retNext() {return i < nn ? &stor.data[i] : 0;}
  EStoreIter() {}
  };


template <class Data, const int n>
void EntityStore<Data, n>::copyObj(EntityStore& es) {
int i;
int end = es.data.end();

  data.clear();   curID = es.curID;

  for (i = 0; i < end; i++) data[i] = es.data[i];
  }


template <class Data, const int n>
Data* EntityStore<Data, n>::addNil(TCchar* name) {return add(nilName(name));}


template <class Data, const int n>
String EntityStore<Data, n>::nilName(TCchar* name)
                                         {String s = _T("< "); s += name; s += _T(" >"); return s;}


// Add new Item if item not alrady in store

template <class Data, const int n>
Data* EntityStore<Data, n>::add(TCchar* id) {
Data* p = findItem(id);   if (p) return p;

  p = newItem(id);    return p;
  }


template <class Data, const int n>
Data* EntityStore<Data, n>::newItem(TCchar* id) {
String s = id;
Data&  d = data.nextData();

  if (s.isEmpty()) s = _T("< Name >");   d.id = curID = s;   return &d;
  }


template <class Data, const int n>
void EntityStore<Data, n>::delNil(TCchar* name) {delItem(nilName(name));}



template <class Data, const int n>
void EntityStore<Data, n>::delItem(TCchar* id) {
int   end = data.end();
int   i;

  for (i = 0; i < end; i++) if (data[i].id == id) {data.del(i); return;}
  }


template <class Data, const int n>
Data* EntityStore<Data, n>::findItem(TCchar* id) {
int i;

  if (!id || !*id) return 0;

  for (i = 0; i < data.end(); i++)
                                  {Data& d = data[i];   if (d.id == id) {curID = d.id; return &d;}}
  return 0;
  }


template <class Data, const int n>
void EntityStore<Data, n>::defaultCurID(TCchar* anID) {
int i;
int end = data.end();

  if (!curID.isEmpty() && curID[0] != _T('<') && anID && *anID) {
    for (i = 0; i < end; i++) {
      Data&   d  = data[i];
      String& id = d.id;

      if (id == anID) {curID = id; return;}
      }
    }

  for (i = 0; i < end; i++) {
    Data&   d  = data[i];
    String& id = d.id;

    if (!id.isEmpty() && id[0] != _T('<')) {curID = id; return;}
    }
  }


// Set current icon in combobox to s

template <class Data, const int n>
void EntityStore<Data, n>::setCur(TCchar* tc, ComboBox& cb) {
String s = tc;  setCurData(s);   cb.setCurSel(curID.isEmpty() ? -1 : curID);
  }


template <class Data, const int n>
void EntityStore<Data, n>::setCurData(String& id) {if (!findItem(id)) curID.clear();}


// Leaving combo box, check for a new name in edit box

template <class Data, const int n>
void EntityStore<Data, n>::leavingCB(ComboBox& cb) {
String s;
Data*  data;

  if (!cb.getWindowText(s)) return;
  data = add(s); cb.add(s); cb.setCurSel(s);
  }


template <class Data, const int n>
void EntityStore<Data, n>::delItem(ComboBox& cb) {
Data*  cur = curData();
String s;

  if (!cur) return;

  cb.del(curID); delData();

  if (!data.end()) addNil(_T("Name"));   curID = data[0].id;   cb.setCurSel(curID);
  }


template <class Data, const int n>
void EntityStore<Data, n>::delData() {
int i;

  if (curID.isEmpty()) return;

  for (i = 0; i < data.end(); i++)
                      {Data& d = data[i];    if (d.id == curID) {d.clear(); data.del(i); break;}}

  curID = data.end() ? data[0].id : String(_T(""));
  }


template <class Data, const int n>
void EntityStore<Data, n>::loadCB(ComboBox& cb) {
int i;

  cb.clear();

  for (i = 0; i < data.end(); i++) {String& id = data[i].id; add(id, cb);}

  if (curID.isEmpty() && data.end() > 0) curID = data[0].id;

  if (!curID.isEmpty()) cb.setCurSel(curID);
  }




template <class Data, const int n>
Data* EntityStore<Data, n>::storeCB(ComboBox& cb) {
CBIter  iter(cb);
String* s;

  if (!&cb) {messageBox(_T("Uninitialized ComboBox in EntityStore")); return 0;}

  cb.getWindowText(curID);

  for (s = iter(); s; s = iter++) add(*s);

  setCurData(curID);   return curData();
  }


class Data {
public:

String id;
String wixID;

  Data() {}
 ~Data() {}

  void clear()       {id.clear(); wixID.clear();}
  void copy(Data& d) {id = d.id;  wixID = d.wixID;}
  };


