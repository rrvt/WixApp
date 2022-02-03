// A little history class for real time data collection


#pragma once
#include "Expandable.h"
#include "IterT.h"

class FileIO;


struct HistData {

uint id;
uint flags;

  HistData() : id(0), flags(0) { }
  HistData(HistData& data) {copy(data);}
 ~HistData() { }

  HistData& operator= (HistData& data) {copy(data); return *this;}

  void output(FileIO& file);

private:

  void copy(HistData& data) {id = data.id; flags = data.flags;}
  };


class History;
typedef IterT<History, HistData> HistIter;


class History {

Expandable<HistData, 128> data;

public:

  History() { }
 ~History() { }

  HistData& nextData() {return data.nextData();}

  void saveData();

private:

  // returns either a pointer to datum at index i in array or zero

  HistData* datum(int i) {return 0 <= i && i < nData() ? &data[i] : 0;}

  int   nData()      {return data.end();}                       // returns number of data items in array

  void  removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

  friend typename HistIter;
  };



extern History history;
