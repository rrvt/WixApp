// Entity Store


#pragma once
#include "ComboBox.h"
#include "Expandable.h"
#include "MessageBox.h"



template <class Data, const int n>

class EntityStore {
public:

int                  loopX;
int                  nData;
Expandable <Data, n> data;
Data*                curData;

  EntityStore() : loopX(0), nData(0), curData(0) {}
  EntityStore(EntityStore& es) {copyObj(es);}
 ~EntityStore() { }

  EntityStore& operator= (EntityStore& es) {copyObj(es); return *this;}

  void  clear() {nData = 0; data.clr(); curData = 0;}

  Data* find(String& id);
  Data* add(String& id);
  Data* newItem();

  void  editUpdate(ComboBox& cb);
  void  loadCB(ComboBox& cb);
  Data* storeCB(ComboBox& cb);

  void  setCur(String& s, ComboBox& cb);         // Set current icon in combobox to s
  void  setNewCur(ComboBox& cb);                 // Respond to change in combo box
  void  leavingCB(ComboBox& cb);                 // Leaving combo box, check for a new name in edit box
  void  delItem(ComboBox& cb);

  Data* startLoop() {loopX = 0; return loopX < nData ? &data[loopX] : 0;}
  Data* nextItem()  {loopX++;   return loopX < nData ? &data[loopX] : 0;}

private:

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

  EStoreIter(EntityStore<Data,n>& store) : stor(store), i(0), nn(stor.nData) {}
 ~EStoreIter() { }

  Data* startLoop() {i = 0; return retNext();}
  Data* next()      {i++;   return retNext();}

private:

  Data* retNext() {return i < nn ? &stor.data[i] : 0;}
  EStoreIter() {}
  };


template <class Data, const int n>
void EntityStore<Data, n>::copyObj(EntityStore& es) {

  for (nData = 0, curData = 0; nData < es.nData; nData++) {

    data[nData] = es.data[nData];

    if (es.curData = &es.data[nData]) curData = &data[nData];
    }
  }




template <class Data, const int n>
Data* EntityStore<Data, n>::add(String& id) {
Data* p;

  if (find(id)) return curData;

  p = newItem();   p->id = id;   return p;
  }


template <class Data, const int n>
Data* EntityStore<Data, n>::find(String& id) {
int i;

  for (i = 0; i < nData; i++) if (data[i].id == id) {curData = &data[i]; return curData;}

  return 0;
  }


template <class Data, const int n>
Data* EntityStore<Data, n>::newItem() {return curData = &data[nData++];}



template <class Data, const int n>
void EntityStore<Data, n>::editUpdate(ComboBox& cb) {
String s;

  if (!nData) curData = &data[nData++];

  cb.getWindowText(s);  if (s.empty()) return;

  cb.del(curData->id);   if (cb.find(s) < 0) {cb.add(s); curData->id = s;}   cb.setCurSel(s);
  }


template <class Data, const int n>
void EntityStore<Data, n>::setNewCur(ComboBox& cb) {
String s;

  if (!cb.getCurSel(s) || !find(s))
    {if (!curData && nData > 0) {curData = &data[0]; cb.setCurSel(curData->id);}}
  }


// Set current icon in combobox to s

template <class Data, const int n>
void EntityStore<Data, n>::setCur(String& s, ComboBox& cb) {
  setCurData(s);

  if (curData) cb.setCurSel(curData->id);
  }


template <class Data, const int n>
void EntityStore<Data, n>::setCurData(String& id)
                                         {if (!find(id)) if (!curData && nData > 0) curData = &data[0];}


// Leaving combo box, check for a new name in edit box

template <class Data, const int n>
void EntityStore<Data, n>::leavingCB(ComboBox& cb) {
String s;
Data*  data;

  if (!cb.getWindowText(s)) return;
  data = add(s); cb.AddString(s); cb.setCurSel(s);
  }



template <class Data, const int n>
void EntityStore<Data, n>::delItem(ComboBox& cb) {
String s;

  if (!curData) return;

  cb.del(curData->id); delData();

  if (!nData) newItem();

  curData = &data[0]; cb.setCurSel(curData->id);
  }


template <class Data, const int n>
void EntityStore<Data, n>::delData() {
int i;

  if (!curData) return;

  for (i = 0; i < nData; i++)
    if (&data[i] == curData) {
      curData->delData();
      data.del(i);
      nData--; break;
      }

  curData = nData ? &data[0] : 0;
  }


template <class Data, const int n>
void EntityStore<Data, n>::loadCB(ComboBox& cb) {
int i;

  cb.clear();

  for (i = 0; i < nData; i++) cb.AddString(data[i].id);

  if (!curData && nData > 0) curData = &data[0];

  if (curData) cb.SetWindowText(curData->id);
  }



template <class Data, const int n>
Data* EntityStore<Data, n>::storeCB(ComboBox& cb) {
int    n;
int    i;
String cur;
String s;
Data*  p;

  if (!cb) {messageBox(_T("Uninitialized ComboBox in EntityStore")); return 0;}

  cb.getWindowText(cur);

  n = cb.GetCount();

  for (i = 0; i < n; i++) if (cb.getText(i, s)) p = add(s);

  setCurData(cur);   return curData;
  }




class Data {
public:

String id;
String wixID;

  Data() {}
 ~Data() {}
  };

