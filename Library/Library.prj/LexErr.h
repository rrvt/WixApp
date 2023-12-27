// Last Error in LexT


#pragma once
#include "ExpandableP.h"
#include "IterT.h"
#include "Strings.h"


struct LexDatum {

String s;

  LexDatum() { }
 ~LexDatum() { }
  };


typedef DatumPtrT<LexDatum, String> LexErrP;
class LexErr;
typedef IterT<LexErr, LexDatum> LexErrIter;


class LexErr {

ExpandableP<LexDatum, String, LexErrP, 2> data;

public:

  LexErr() { }
 ~LexErr() { }

  void clear() {data.clear();}

  void addLine(TCchar* tc) {data.nextData().s = tc;}

private:

  // returns either a pointer to data (or datum) at index i in array or zero

  LexDatum* datum(int i) {return 0 <= i && i < nData() ? data[i].p : 0;}

  int       nData()      {return data.end();}                     // returns number of data items in array
                                                                  // not necessarily private
  void      removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

  friend typename LexErrIter;
  };

