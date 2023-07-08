// Template for incrementing through a subclass of CRecordset.

/*

  To complete the iterator the spcific lass must contain the following declarations:

  class MyTblRcd;
  typedef AccIter<MyTblRcd> MyTblIter;

  class MyTblRcd : public AccRcdSet {
    o o o
  private:
    MyTblRcd* moveFirst() {MoveFirst();  return IsEOF() ? 0 : this;}
    MyTblRcd* moveNext()  {MoveNext();   return IsEOF() ? 0 : this;}
    MyTblRcd* moveLast()  {MoveLast();   return IsEOF() ? 0 : this;}
    MyTblRcd* movePrev()  {MovePrev();   return IsBOF() ? 0 : this;}
    friend typename MyTblIter;
    }
*/


#pragma once


template <class RcdSet>
class AccIterT {

RcdSet& tbl;

public:

enum Dir {Fwd, Rev};

  AccIterT(RcdSet& table) : tbl(table) { }

  RcdSet* operator() (Dir dir = Fwd) {return dir == Fwd ? tbl.moveFirst() : tbl.moveLast();}
  RcdSet* operator++ (int)           {return tbl.moveNext();}
  RcdSet* operator-- (int)           {return tbl.movePrev();}

private:

  AccIterT() : tbl(*(RcdSet*)0) { }
  };




