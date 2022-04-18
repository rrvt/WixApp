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
#if 1

RcdSet& tbl;

public:

enum Dir {Fwd, Rev};

  AccIterT(RcdSet& table) : tbl(table) { }

  RcdSet* operator() (Dir dir = Fwd) {return dir == Fwd ? tbl.moveFirst() : tbl.moveLast();}
  RcdSet* operator++ (int)           {return tbl.moveNext();}
  RcdSet* operator-- (int)           {return tbl.movePrev();}

private:

  AccIterT() : tbl(*(RcdSet*)0) { }

#else
int     iterX;
RcdSet& rcdSet;

public:

enum Dir {Fwd, Rev};

  AccIterT(RcdSet& dataStore) : iterX(0),          rcdSet(dataStore)  { }
  AccIterT(AccIterT& iter)    : iterX(iter.iterX), rcdSet(iter.rcdSet) { }

  Datum* operator() (Dir rev = Fwd) {iterX = rev ? rcdSet.nData() : 0; return rev ? decr() : current();}
  Datum* operator++ (int)           {return iterX < rcdSet.nData() ? incr() : 0;}
  Datum* operator-- (int)           {return iterX > 0             ? decr() : 0;}

  int    index()                    {return iterX;}
  Datum* current()                  {return rcdSet.datum(iterX);}

  AccIterT& operator= (AccIterT& iter)    {iterX = iter.iterX; rcdSet = iter.rcdSet;}

  bool   isLast()                   {return iterX + 1 == rcdSet.nData();}
  bool   isFirst()                  {return iterX <= 0;}

  void   remove(Dir rev = Fwd)      {rcdSet.removeDatum(rev ? iterX++ : iterX--);}

private:

  Datum* incr()  {return iterX < rcdSet.nData() ? rcdSet.datum(++iterX) : 0;}
  Datum* decr()  {return iterX > 0             ? rcdSet.datum(--iterX) : 0;}

  AccIterT() : rcdSet(*(RcdSet*) 0), iterX(0) { }      // This prevents an uninitizlized iterator
#endif
  };

