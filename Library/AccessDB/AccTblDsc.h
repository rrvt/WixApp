// Defines an iterator to see all the tables in the database and an descriptor that shows the details
// of a single table in the database


#pragma once
#include "AccIterT.h"
#include "AccRcdSet.h"


class AccTblDsc;
typedef AccIterT<AccTblDsc> TblIter;


class AccTblDsc : public AccRcdSet {
DECLARE_DYNAMIC(AccTblDsc)
public:

CString qualifier;
CString owner;
CString name;
CString type;
CString remarks;

  AccTblDsc();
 ~AccTblDsc() {close();}

  bool open(TCchar* path);
  void close() {AccRcdSet::close();}

private:

  virtual void DoFieldExchange(CFieldExchange*);

  AccTblDsc* moveFirst() {MoveFirst();  return IsEOF() ? 0 : this;}
  AccTblDsc* moveNext()  {MoveNext();   return IsEOF() ? 0 : this;}
  AccTblDsc* moveLast()  {MoveLast();   return IsEOF() ? 0 : this;}
  AccTblDsc* movePrev()  {MovePrev();   return IsBOF() ? 0 : this;}
  friend typename TblIter;
  };


extern AccTblDsc accTblDsc;
