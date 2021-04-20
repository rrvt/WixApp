// Iterator


#pragma once


/*
Data should be seen, sometimes all the data needs to be seen.  The iterator class implements an
object that serves up each Data entry in the array (not the pointer, the Data object).  It is used:
   DataStoreIter iter(dataStore);
   Data*         data;

     for (data = iter(); data; data = iter++) {
       String& s = data->get();   Use data as a pointer to the record, it is guaranteed to be non-zero

last gives a heads up when the last entry is being processed
The template requires two functions be part of Store:
  int nData() -- returns number of data items in array
  Data* datum(int i) -- returns either a pointer to data (or datum) at index i in array or zero

private:

  // returns either a pointer to data (or datum) at index i in array or zero

  Data* datum(int i) {return 0 <= i && i < nData() ? &data[i] : 0;}       // or data[i].p

  int   nData()      {return data.end();}                       // returns number of data items in array

  void  removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

  friend typename DataIter;
*/


template <class Store, class Data>

class IterT {
int    iterX;
Store& store;

public:

enum Dir {Fwd, Rev};

  IterT(Store& dataStore) : iterX(0),          store(dataStore)  { }
  IterT(IterT& iter)      : iterX(iter.iterX), store(iter.store) { }

  Data* operator() (Dir rev = Fwd) {iterX = rev ? store.nData() : 0; return rev ? decr() : current();}
  Data* operator++ (int)           {return iterX < store.nData() ? incr() : 0;}
  Data* operator-- (int)           {return iterX > 0             ? decr() : 0;}

  int   index()                    {return iterX;}
  Data* current()                  {return store.datum(iterX);}

  IterT& operator= (IterT& iter)   {iterX = iter.iterX; store = iter.store;}

  bool  isLast()                   {return iterX + 1 == store.nData();}
  bool  isFirst()                  {return iterX <= 0;}

  void  remove(Dir rev = Fwd)      {store.removeDatum(rev ? iterX++ : iterX--);}

private:

  Data* incr() {return iterX < store.nData() ? store.datum(++iterX) : 0;}
  Data* decr() {return iterX > 0             ? store.datum(--iterX) : 0;}

  IterT() : store(*(Store*) 0), iterX(0) { }      // This prevents an uninitizlized iterator
  };


//Obj Iterator -- i.e. the actual Object is contained in the iterator, therefore use it immediately after
// a pointer is returned to it...
/*
Data* Store::getDatum(int i, Data& d) {

  if (0 <= i && i < nData()) {<obtain data as t>;   d = t;   return &d;}

  return 0;
  }
*/

template <class Store, class Data>

class ObjIterT {
int    iterX;
Store& store;
Data   datum;

public:

  enum Dir {Fwd, Rev};

  ObjIterT(Store& dataStore) : store(dataStore), iterX(0) { }

  Data* operator() (Dir rev = Fwd) {iterX = rev ? store.nData() : 0; return rev ? decr() : current();}
  Data* operator++ (int) {return iterX < store.nData() ? incr() : 0;}
  Data* operator-- (int) {return iterX > 0             ? decr() : 0;}

  Data* current()        {return store.getDatum(iterX, datum);}

  bool  isLast()         {return iterX + 1 == store.nData();}
  bool  isFirst()        {return iterX <= 0;}

  void  remove(Dir rev = Fwd) {store.removeDatum(rev ? iterX++ : iterX--);}

private:

  Data* incr() {return iterX < store.nData() ? store.getDatum(++iterX, datum) : 0;}
  Data* decr() {return iterX > 0             ? store.getDatum(--iterX, datum) : 0;}

  ObjIterT() : store(*(Store*) 0), iterX(0) { }      // This prevents an uninitizlized iterator
  };

