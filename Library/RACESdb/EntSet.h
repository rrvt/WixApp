// EntSet Record


#pragma once
#include "AccRcdSet.h"


class EntSet : public AccRcdSet {
public:
int     entityID;
CString firstName;
CString middleInitial;
CString lastName;
CString suffix;
int     addrID;
int     cityStID;
BOOL    addrIsPO;
CString locationZip;
CString eMail;
CString phone1;
CString phone2;

  EntSet();
 ~EntSet() {close();}

  bool open(TCchar* path);
  bool isOpen() {return opened;}
  void close() {opened = false;}

  bool edit();
  bool addNew();
  bool update();
  bool remove();

private:

  virtual void DoFieldExchange(CFieldExchange* pFX);

  EntSet* moveFirst();
  EntSet* moveNext();
  EntSet* moveLast();
  EntSet* movePrev();
  friend class EntSetIter;
  };


// Iterator Declaration, see IterT.h for details

class EntSetIter {

EntSet& rcd;

public:

enum Dir {Fwd, Rev};

  EntSetIter(EntSet& record) : rcd(record) { }

  EntSet* find(int id);

  EntSet* operator() (Dir dir = Fwd) {return dir == Fwd ? rcd.moveFirst() : rcd.moveLast();}
  EntSet* operator++ (int)           {return rcd.moveNext();}
  EntSet* operator-- (int)           {return rcd.movePrev();}

private:

  EntSetIter() : rcd(*(EntSet*)0) { }
  };
