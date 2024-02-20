// Provieds an iterator for seeing all the fields in a table and a descriptor of a single field.


#pragma once
#include "AccIterT.h"
#include "AccRcdSet.h"


class AccFldDsc;
typedef AccIterT<AccFldDsc> FldIter;


class AccFldDsc : public AccRcdSet {
DECLARE_DYNAMIC(AccFldDsc)
public:
Cstring tblQualifier;
Cstring tblOwner;
Cstring tblName;
Cstring name;
int     type;
Cstring typeName;
long    precision;
long    length;
int     scale;
int     radix;
int     nullable;
Cstring remarks;

  AccFldDsc();
 ~AccFldDsc() {close();}

              void clear();

              bool open(TCchar* path, TCchar* tableName);
              void close() {AccRcdSet::close();}
private:

  virtual     void DoFieldExchange(CFieldExchange*);

  AccFldDsc* moveFirst() {MoveFirst();  return IsEOF() ? 0 : this;}
  AccFldDsc* moveNext()  {MoveNext();   return IsEOF() ? 0 : this;}
  AccFldDsc* moveLast()  {MoveLast();   return IsEOF() ? 0 : this;}
  AccFldDsc* movePrev()  {MovePrev();   return IsBOF() ? 0 : this;}
  friend typename FldIter;
  };


extern AccFldDsc accFldDsc;


#if 0
class AccFldDscIter {

AccFldDsc& tbl;

public:

  enum Dir {Fwd, Rev};

  AccFldDscIter(AccFldDsc& table) : tbl(table) { }

  AccFldDsc* operator() (Dir dir = Fwd) {return dir == Fwd ? tbl.moveFirst() : tbl.moveLast();}
  AccFldDsc* operator++ (int)           {return tbl.moveNext();}
  AccFldDsc* operator-- (int)           {return tbl.movePrev();}

private:

  AccFldDscIter() : tbl(*(AccFldDsc*)0) { }
  };
#endif


