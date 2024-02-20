// StsSet Record


#pragma once
#include "AccRcdSet.h"


class StsSet : public AccRcdSet {
public:
int     stsID;
CString abbreviation;
CString description;

  StsSet();
 ~StsSet() {close();}

  bool open(TCchar* path);
  bool isOpen() {return opened;}
  void close() {opened = false;}

  bool edit();
  bool addNew();
  bool update();
  bool remove();

private:

  virtual void DoFieldExchange(CFieldExchange* pFX);

  StsSet* moveFirst();
  StsSet* moveNext();
  StsSet* moveLast();
  StsSet* movePrev();
  friend class StsSetIter;
  };


// Iterator Declaration, see IterT.h for details

class StsSetIter {

StsSet& rcd;

public:

enum Dir {Fwd, Rev};

  StsSetIter(StsSet& record) : rcd(record) { }

  StsSet* find(int id);

  StsSet* operator() (Dir dir = Fwd) {return dir == Fwd ? rcd.moveFirst() : rcd.moveLast();}
  StsSet* operator++ (int)           {return rcd.moveNext();}
  StsSet* operator-- (int)           {return rcd.movePrev();}

private:

  StsSetIter() : rcd(*(StsSet*)0) { }
  };
