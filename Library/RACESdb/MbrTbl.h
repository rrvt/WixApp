// Member Table


#pragma once
#include "MbrSet.h"
#include "ExpandableP.h"
#include "IterT.h"


class MbrRcd {
int   id;
bool  dirty;
bool  remove;

public:

int    badgeNumber;
int    mbrEntityID;
int    emplEntityID;
int    iCE_EntityID;
int    assgnPrefID;
int    locationPrefID;
int    statusID;
String callSign;
String fCCExpiration;
String startDate;
String dSWDate;
String badgeExpDate;
String responder;
String secondaryEmail;
String textMsgPh1;
String textMsgPh2;
String handHeld;
String portMobile;
String portPacket;
String otherEquip;
String multilingual;
String otherCapabilities;
String limitations;
String comments;
String shirtSize;
BOOL   isOfficer;
String skillCertifications;
String eOC_Certifications;
String updateDate;
BOOL   badgeOK;
String image;

  MbrRcd() : id(0), dirty(false), remove(false) { }
 ~MbrRcd() { }

  void load(MbrSet* set);

  int getId() {return id;}

  void setDirty()  {dirty = true;}
  void setRemove() {dirty = true; remove = true;}

  void store(MbrSet& set);
  void add(  MbrSet& set);

  void display();

  // Needed for Insertion Sort of Primary Key
  bool operator== (MbrRcd& r) {return id == r.id;}
  bool operator>= (MbrRcd& r) {return id >= r.id;}

  // Needed for Binary Search of Primary Key
  bool operator== (long id) {return this->id == id;}
  bool operator<  (long id) {return this->id <  id;}
  bool operator>  (long id) {return this->id >  id;}

  // Needed for Linear Search with one or more arguments
  bool contains(TCchar* callSign) {
    return this->callSign == callSign;
    }

private:

  void copy(MbrSet& set);

  friend class MbrTbl;
  };


// Record Pointer Declaration, see ExpandableP.h for details
typedef DatumPtrT<MbrRcd, int> MbrRcdP;

// Iterator Declaration, see IterT.h for details
class MbrTbl;
typedef IterT<MbrTbl, MbrRcd> MbrIter;


class MbrTbl {

ExpandableP<MbrRcd, int, MbrRcdP, 2> data;

int    maxID;
MbrSet mbrSet;

public:

String name;

  MbrTbl() : maxID(0) { }
 ~MbrTbl() {clear();}

  void clear() {data.clear();}

  bool load(TCchar* path);      // load database table into memory

  MbrRcd* add(MbrRcd& rcd);     // Add a new record to table and database

  bool store(TCchar* path);     // Store/Del entities marked

  MbrRcd* find(int id) {return data.bSearch(id);}
  MbrRcd* find(TCchar* callSign);

  virtual void display();

private:

  bool open(TCchar* path) {return mbrSet.open(path);}
  void close() {mbrSet.close();}

  void setTabs();

  // returns either a pointer to data (or datum) at index i in array or zero

  MbrRcd* datum(int i) {return 0 <= i && i < nData() ? data[i].p : 0;}

  int   nData()      {return data.end();}   // returns number of data items in array

  void  removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

  friend typename MbrIter;
  };


extern MbrTbl mbrTbl;

