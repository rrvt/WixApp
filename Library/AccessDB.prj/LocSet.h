// LocSet Record


#pragma once
#include "AccRcdSet.h"


class LocSet : public AccRcdSet {
public:
int     locationPrefID;
CString key;
CString txt;

  LocSet();
 ~LocSet() {close();}

  bool open(TCchar* path);
  bool isOpen() {return opened;}
  void close() {AccRcdSet::close(); opened = false;}

  bool edit();
  bool addNew();
  bool update();
  bool remove();

private:

  virtual void DoFieldExchange(CFieldExchange* pFX);

  LocSet* moveFirst();
  LocSet* moveNext();
  LocSet* moveLast();
  LocSet* movePrev();
  friend class LocSetIter;
  };


// Iterator Declaration, see IterT.h for details

class LocSetIter {

LocSet& rcd;

public:

enum Dir {Fwd, Rev};

  LocSetIter(LocSet& record) : rcd(record) { }

  LocSet* find(int id);

  LocSet* operator() (Dir dir = Fwd) {return dir == Fwd ? rcd.moveFirst() : rcd.moveLast();}
  LocSet* operator++ (int)           {return rcd.moveNext();}
  LocSet* operator-- (int)           {return rcd.movePrev();}

private:

  LocSetIter() : rcd(*(LocSet*)0) { }
  };
