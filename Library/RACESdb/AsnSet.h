// AsnSet Record


#pragma once
#include "AccRcdSet.h"


class AsnSet : public AccRcdSet {
public:
int     assgnPrefID;
CString aPKey;
CString txt;

  AsnSet();
 ~AsnSet() {close();}

  bool open(TCchar* path);
  bool isOpen() {return opened;}
  void close() {opened = false;}

  bool edit();
  bool addNew();
  bool update();
  bool remove();

private:

  virtual void DoFieldExchange(CFieldExchange* pFX);

  AsnSet* moveFirst();
  AsnSet* moveNext();
  AsnSet* moveLast();
  AsnSet* movePrev();
  friend class AsnSetIter;
  };


// Iterator Declaration, see IterT.h for details

class AsnSetIter {

AsnSet& rcd;

public:

enum Dir {Fwd, Rev};

  AsnSetIter(AsnSet& record) : rcd(record) { }

  AsnSet* find(int id);

  AsnSet* operator() (Dir dir = Fwd) {return dir == Fwd ? rcd.moveFirst() : rcd.moveLast();}
  AsnSet* operator++ (int)           {return rcd.moveNext();}
  AsnSet* operator-- (int)           {return rcd.movePrev();}

private:

  AsnSetIter() : rcd(*(AsnSet*)0) { }
  };
