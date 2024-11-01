// CtySet Record


#pragma once
#include "AccRcdSet.h"


class CtySet : public AccRcdSet {
public:
int     cityStateID;
CString city;
CString state;
CString zip;

  CtySet();
 ~CtySet() {close();}

  bool open(TCchar* path);
  bool isOpen() {return opened;}
  void close() {AccRcdSet::close(); opened = false;}

  bool edit();
  bool addNew();
  bool update();
  bool remove();

private:

  virtual void DoFieldExchange(CFieldExchange* pFX);

  CtySet* moveFirst();
  CtySet* moveNext();
  CtySet* moveLast();
  CtySet* movePrev();
  friend class CtySetIter;
  };


// Iterator Declaration, see IterT.h for details

class CtySetIter {

CtySet& rcd;

public:

enum Dir {Fwd, Rev};

  CtySetIter(CtySet& record) : rcd(record) { }

  CtySet* find(int id);

  CtySet* operator() (Dir dir = Fwd) {return dir == Fwd ? rcd.moveFirst() : rcd.moveLast();}
  CtySet* operator++ (int)           {return rcd.moveNext();}
  CtySet* operator-- (int)           {return rcd.movePrev();}

private:

  CtySetIter() : rcd(*(CtySet*)0) { }
  };
