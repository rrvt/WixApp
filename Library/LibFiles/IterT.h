// Iterator


#pragma once


// Data should be seen, sometimes all the data needs to be seen.  The iterator class implements an
// object that serves up each Data entry in the array (not the pointer, the Data object).  It is used:
//    DataStoreIter iter(dataStore);
//    Data*         data;
//
//      for (data = iter(); data; data = iter++) {
//        String& s = data->get();   Use data as a pointer to the record, it is guaranteed to be non-zero
// last gives a heads up when the last entry is being processed
// The template requires two functions be part of Store:
//   int nData() -- returns number of data items in array
//   Data* datum(int i) -- returns either a pointer to data (or datum) at index i in array or zero

template <class Store, class Data>

class IterT {
int    iterX;
Store& store;

public:

  IterT(Store& dataStore) : store(dataStore), iterX(0) { }

  Data* operator() ()    {iterX = 0; return incr();}
  Data* operator++ (int) {iterX++;   return incr();}
  bool  last()           {return iterX + 1 == store.nData();}

private:

  Data* incr() {return iterX < store.nData() ? store.datum(iterX) : 0;}

  IterT() : store(*(Store*) 0), iterX(0) { }      // This prevents an uninitizlized iterator
  };
