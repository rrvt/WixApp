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
CString callSign;
CString fCCExpiration;
CString startDate;
CString dSWDate;
CString badgeExpDate;
CString responder;
CString secondaryEmail;
CString textMsgPh1;
CString textMsgPh2;
CString handHeld;
CString portMobile;
CString portPacket;
CString otherEquip;
CString multilingual;
CString otherCapabilities;
CString limitations;
CString comments;
CString shirtSize;
BOOL    isOfficer;
CString skillCertifications;
CString eOC_Certifications;
CString updateDate;
BOOL    badgeOK;
CString image;

  MbrSet();
 ~MbrSet() {close();}

  bool open(TCchar* path);
  bool isOpen() {return opened;}
  void close() {opened = false;}

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
