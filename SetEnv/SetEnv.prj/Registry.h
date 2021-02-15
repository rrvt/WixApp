// Registry Class for dealing with the Registry
// For our purposes there are three levels.
// The Base is either:
//   *  HKEY_LOCAL_MACHINE
//   +  HKEY_CURRENT_USER
// Level 2 is either
//   *  the User ("Environment")
//   *  the System ("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment")
// The target key=value string is the third level.

#pragma once
#include "Expandable.h"
#include "RestartAsAdmin.h"


enum EnvironmentBase {nilBase, LocalMachine, CurrentUser};


class Registry {
HKEY   baseH;
String levelOneKey;
HKEY   lvlOneKeyH;

bool   tgtOpen;
String tgtName;

Tchar  sepChar;
DWORD  bufLng;

Expandable <String, 128> tbl;        // Sanitize Table

public:

String tgtValue;
int    tgtLng;
DWORD  tgtType;

bool   dirty;

  Registry(EnvironmentBase base, Tchar separator);
 ~Registry();

  bool  closeBase();
  bool  openTgt(    String& key);                           // Obtains the value string, length and type
  bool  appendValue(String& val);                                 // append val if not already present
  bool  prefixValue(String& val);
  bool  deleteValue(String& val);                     // Delete whole key if value empty, otherwise just
  bool  setValue(   String& val) {tgtValue = val; return set();}  // set new value, stays open.

  void  displayTgt();

  bool  copyTo(String& newKey);
  bool  sanitizeValue();

private:

  bool  openBase();
  bool  openBase(bool create);
  void  closeLvlOne();
  bool  connectRemote();
  bool  set();
  bool  del();
  int   find(String& val);
  bool  isLeftSepCh(int i);                         // Left non-space character is separator char
  bool  isRightSepCh(int i);                        // Right non-space character is separator char
  int   prevSep(int i);
  int   nextSep(int i);
  void  setTgtLng(DWORD bfLng) {tgtLng = bfLng/sizeof(Tchar) - 1;}
  void  insertInTable(String& t);
        Registry() {}
  };
