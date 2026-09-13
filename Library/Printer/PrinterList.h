// List of printers on current PC -- uses EnumPrinters


#pragma once
#include "Expandable.h"
#include "IterT.h"


class PrinterList;
typedef IterT<PrinterList, String> PLIter;


class PrinterList {

Expandable<String, 2> data;

public:

  PrinterList() { }
 ~PrinterList() { }

  void get();

private:

  // returns either a pointer to datum at index i in array or zero

  String* datum(int i) {return 0 <= i && i < nData() ? &data[i] : 0;}

  int     nData()      {return data.end();}               // Returns number of data items in array,
                                                          // not necessarily private
  void   removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

  friend typename PLIter;
  };

