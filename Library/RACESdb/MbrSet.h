// MbrSet Record


#pragma once
#include "AccRcdSet.h"


class MbrSet : public AccRcdSet {
public:
int     memberID;
int     badgeNumber;
int     mbrEntityID;
int     emplEntityID;
int     iCE_EntityID;
int     assgnPrefID;
int     locationPrefID;
int     statusID;
Cstring callSign;
Cstring fCCExpiration;
Cstring startDate;
Cstring dSWDate;
Cstring badgeExpDate;
Cstring responder;
Cstring secondaryEmail;
Cstring textMsgPh1;
Cstring textMsgPh2;
Cstring handHeld;
Cstring portMobile;
Cstring portPacket;
Cstring otherEquip;
Cstring multilingual;
Cstring otherCapabilities;
Cstring limitations;
Cstring comments;
Cstring shirtSize;
BOOL    isOfficer;
Cstring skillCertifications;
Cstring eOC_Certifications;
Cstring updateDate;
BOOL    badgeOK;
Cstring image;

  MbrSet();
 ~MbrSet() {close();}

  bool open(TCchar* path);
  bool isOpen() {return opened;}
  void close() {AccRcdSet::close();}

  bool edit();
  bool addNew();
  bool update();
  bool remove();

private:

  virtual void DoFieldExchange(CFieldExchange* pFX);

  MbrSet* moveFirst();
  MbrSet* moveNext();
  MbrSet* moveLast();
  MbrSet* movePrev();
  friend class MbrSetIter;
  };


// Iterator Declaration, see IterT.h for details

class MbrSetIter {

MbrSet& rcd;

public:

enum Dir {Fwd, Rev};

  MbrSetIter(MbrSet& record) : rcd(record) { }

  MbrSet* find(int id);

  MbrSet* operator() (Dir dir = Fwd) {return dir == Fwd ? rcd.moveFirst() : rcd.moveLast();}
  MbrSet* operator++ (int)           {return rcd.moveNext();}
  MbrSet* operator-- (int)           {return rcd.movePrev();}

private:

  MbrSetIter() : rcd(*(MbrSet*)0) { }
  };
