// Entity Store


#pragma once
#include "ComboBox.h"
#include "Expandable.h"
#include "MessageBox.h"



template <class Data, const int n>

class EntityStore {
public:

int                  loopX;
String               curID;
Expandable <Data, n> data;

  EntityStore() : loopX(0) {}
  EntityStore(EntityStore& es) {copyObj(es);}
 ~EntityStore() { }

  EntityStore& operator= (EntityStore& es) {copyObj(es); return *this;}

  void  clear() {data.clr(); curID.clear(); loopX = 0;}

  Data* curData() {return curID.isEmpty() ? 0 : find(curID);}

  Data* find(TCchar* id);
  void  defaultCurID(TCchar* anID);
  Data* add(String id);
  Data* newItem(TCchar* itemName);
  Data* newItem(String& id);
  void  delItem(String& id);

  void  editUpdate(ComboBox& cb);
  void  loadCB(    ComboBox& cb);
  Data* storeCB(   ComboBox& cb);

  void  setCur(String& s, ComboBox& cb);         // Set current icon in combobox to s
  void  setNewCur(ComboBox& cb);                 // Respond to change in combo box
  void  leavingCB(ComboBox& cb);                 // Leaving combo box, check for a new name in edit box
  void  delItem(  ComboBox& cb);


private:
//  Data* startLoop() {loopX = 0; return loopX < data.end() ? &data[loopX] : 0;}
//  Data* nextItem()  {loopX++;   return loopX < data.end() ? &data[loopX] : 0;}

  void  setCurData(String& id);
  void  delData();
  void  copyObj(EntityStore& es);
  };


template <class Data, const int n>
class EStoreIter {
EntityStore<Data,n>& stor;
int                  i;
int                  nn;
public:

  EStoreIter(EntityStore<Data,n>& store) : stor(store), i(0), nn(stor.data.end()) {}
 ~EStoreIter() { }

  Data* startLoop() {i = 0; return retNext();}
  Data* nextItem()      {i++;   return retNext();}

private:

  Data* retNext() {return i < nn ? &stor.data[i] : 0;}
  EStoreIter() {}
  };


template <class Data, const int n>
void EntityStore<Data, n>::copyObj(EntityStore& es) {
int i;
int n = es.data.end();

  data.clr();   loopX = es.loopX;  curID = es.curID;

  for (i = 0; i < n; i++) data[i] = es.data[i];
  }




template <class Data, const int n>
Data* EntityStore<Data, n>::add(String id) {
Data* p = find(id);

  if (p) return p;

  p = newItem(id);   return p;
  }


template <class Data, const int n>
Data* EntityStore<Data, n>::find(TCchar* id) {
int i;

  if (!id || !*id) return 0;

  for (i = 0; i < data.end(); i++) {Data& d = data[i];   if (d.id == id) {curID = d.id; return &d;}}

  return 0;
  }


template <class Data, const int n>
void EntityStore<Data, n>::defaultCurID(TCchar* anID) {
int i;
int n = data.end();

  if (!curID.isEmpty() && curID[0] != _T('<') && anID && *anID) {
    for (i = 0; i < n; i++) {
      Data&   d  = data[i];
      String& id = d.id;

      if (id == anID) {curID = id; return;}
      }
    }

  for (i = 0; i < n; i++) {
    Data&   d  = data[i];
    String& id = d.id;

    if (!id.isEmpty() && id[0] != _T('<')) {curID = id; return;}
    }
  }


template <class Data, const int n>
Data* EntityStore<Data, n>::newItem(TCchar* itemName)
                          {String s = _T("< "); s += itemName; s += _T(" Name >");   return newItem(s);}


template <class Data, const int n>
Data* EntityStore<Data, n>::newItem(String& id) {
Data& d = data[data.end()];

  if (id.isEmpty()) id = _T("< Name >");   d.id = curID = id;   return &d;
  }


#if 0
template <class Data, const int n>
void EntityStore<Data, n>::editUpdate(ComboBox& cb) {
String s;

  if (!end()) curData = &data[end()++];

  cb.getWindowText(s);  if (s.isEmpty()) return;

  cb.del(curData->id);   if (cb.find(s) < 0) {cb.add(s); curData->id = s;}   cb.setCurSel(s);
  }


template <class Data, const int n>
void EntityStore<Data, n>::setNewCur(ComboBox& cb) {
String s;

  if (!cb.getCurSel(s) || !find(s))
    {if (!curData && end() > 0) {curData = &data[0]; cb.setCurSel(curData->id);}}
  }
#endif


// Set current icon in combobox to s

template <class Data, const int n>
void EntityStore<Data, n>::setCur(String& s, ComboBox& cb) {
  setCurData(s);   cb.setCurSel(curID.isEmpty() ? -1 : curID);
  }


template <class Data, const int n>
void EntityStore<Data, n>::setCurData(String& id) {if (!find(id)) curID.clear();}


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

  if (!data.end()) newItem(_T("< Name >"));   curID = data[0].id;   cb.setCurSel(curID);
  }


template <class Data, const int n>
void EntityStore<Data, n>::delItem(String& id) {
int   n = data.end();
int   i;

  for (i = 0; i < n; i++) if (data[i].id == id) {data.del(i); return;}
  }



template <class Data, const int n>
void EntityStore<Data, n>::delData() {
int i;

  if (curID.isEmpty()) return;

  for (i = 0; i < data.end(); i++)
                            {Data& d = data[i];    if (d.id == curID) {d.delData(); data.del(i); break;}}

  curID = data.end() ? data[0].id : _T("");
  }


template <class Data, const int n>
void EntityStore<Data, n>::loadCB(ComboBox& cb) {
int i;

  cb.clear();

  for (i = 0; i < data.end(); i++) cb.add(data[i].id);

  if (curID.isEmpty() && data.end() > 0) curID = data[0].id;

  if (!curID.isEmpty()) cb.setCurSel(curID);
  }



template <class Data, const int n>
Data* EntityStore<Data, n>::storeCB(ComboBox& cb) {
String s;
bool   rslt;

  if (!&cb) {messageBox(_T("Uninitialized ComboBox in EntityStore")); return 0;}

  cb.getWindowText(curID);

  for (rslt = cb.startLoop(s); rslt; rslt = cb.nextItem(s)) add(s);

  setCurData(curID);   return curData();
  }




class Data {
public:

String id;
String wixID;

  Data() {}
 ~Data() {}
  };

