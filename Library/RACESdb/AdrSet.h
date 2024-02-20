// AdrSet Record


#pragma once
#include "AccRcdSet.h"


class AdrSet : public AccRcdSet {
public:
int     addressID;
CString address1;
CString address2;

  AdrSet();
 ~AdrSet() {close();}

  bool open(TCchar* path);
  bool isOpen() {return opened;}
  void close() {opened = false;}

  bool edit();
  bool addNew();
  bool update();
  bool remove();

private:

  virtual void DoFieldExchange(CFieldExchange* pFX);

  AdrSet* moveFirst();
  AdrSet* moveNext();
  AdrSet* moveLast();
  AdrSet* movePrev();
  friend class AdrSetIter;
  };


// Iterator Declaration, see IterT.h for details

class AdrSetIter {

AdrSet& rcd;

public:

enum Dir {Fwd, Rev};

  AdrSetIter(AdrSet& record) : rcd(record) { }

  AdrSet* find(int id);

  AdrSet* operator() (Dir dir = Fwd) {return dir == Fwd ? rcd.moveFirst() : rcd.moveLast();}
  AdrSet* operator++ (int)           {return rcd.moveNext();}
  AdrSet* operator-- (int)           {return rcd.movePrev();}

private:

  AdrSetIter() : rcd(*(AdrSet*)0) { }
  };
