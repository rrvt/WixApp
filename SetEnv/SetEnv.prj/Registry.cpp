// Registry Class for dealing with the Registry


#include "pch.h"
#include "Registry.h"
#include "qsort.h"
#include "RestartAsAdmin.h"
#include "Utilities.h"


// The registry location under 2000/XP/etc. where environment variables are stored

static TCchar* LocalMachBase =
                            _T("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment");
                                                          // System Variables (HKEY_LOCAL_MACHINE)
static TCchar* CurrentUserBase = _T("Environment");       // User Variables (HKEY_CURRENT_USER)



Registry::Registry(EnvironmentBase base, Tchar separator) :
  baseH(      base == LocalMachine ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER),
  levelOneKey(base == LocalMachine ? LocalMachBase      : CurrentUserBase),
  lvlOneKeyH(0), tgtOpen(false), sepChar(separator), tgtType(0), tgtLng(0), dirty(false) {

  // If a computer name is specified, then connect to it...
  // This will set up the base key to point to the remote machine

  if (!openBase()) baseH = 0;
  }


Registry::~Registry() {closeBase();}


bool Registry::closeBase() {
DWORD   result2 = 0;
LRESULT result;

  closeLvlOne();   if (baseH) RegCloseKey(baseH);   baseH = 0;

  if (!dirty) return true;

  result = SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, 0,
                                        (LPARAM) L"Environment", SMTO_ABORTIFHUNG, 5000, &result2);
  if (result) return true;

  Output(_T("Propagation failed\n")); return false;
  }


bool Registry::openBase() {return openBase(false);}


bool Registry::openBase(bool create) {
ulong rslt;
DWORD dwDisp;

  if (create) {
    rslt = RegCreateKeyEx(baseH, levelOneKey, 0, 0,
                                REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0, &lvlOneKeyH, &dwDisp);
    }

  else {
    if (!RegOpenKeyEx(baseH, levelOneKey, NULL, KEY_ALL_ACCESS, &lvlOneKeyH)) return true;

    rslt = RegOpenKeyEx(baseH, levelOneKey, NULL, KEY_READ, &lvlOneKeyH); if (!rslt) return true;

    if (rslt) {dspErr(_T("Unable to open key, Error: "), rslt); lvlOneKeyH = 0; return false;}
    }

  return true;
  }


// Close the active key

void Registry::closeLvlOne() {if (lvlOneKeyH) RegCloseKey(lvlOneKeyH);   lvlOneKeyH = 0;}


// Obtains the handle, value string, length and type

bool Registry::openTgt(String& key) {
Byte* buf;

  if (!lvlOneKeyH || key.isEmpty()) return false;

  tgtOpen = true; tgtName = key; tgtValue.clear(); tgtLng = 0;

  if (RegQueryValueEx(lvlOneKeyH, tgtName, 0, &tgtType, 0, &bufLng) != ERROR_SUCCESS ||
                                                      bufLng == 0) {tgtType = REG_SZ; return true;}
  buf = new Byte[bufLng];

  if (RegQueryValueEx(lvlOneKeyH, key, 0, &tgtType, buf, &bufLng)) return false;

  tgtValue = (Tchar*) buf;   tgtLng = tgtValue.length();   tgtOpen = true;   delete[] buf;

  return true;
  }



bool Registry::appendValue(String& val) {

  if (!tgtOpen) return false;

  if (tgtValue.isEmpty()) {setValue(val); return true;}   // No separator added

  if (find(val) >= 0)                     return true;    // Already in the current value

  if (tgtValue[tgtLng-1] != sepChar) tgtValue += sepChar;

  tgtValue += val;   set();   return true;
  }



bool Registry::prefixValue(String& val) {

  if (!tgtOpen) return false;

  if (tgtValue.isEmpty()) {setValue(val); return true;}   // No separator added

  if (find(val) >= 0)                     return true;    // Already in the current value

  if (tgtValue[0] != sepChar) tgtValue = sepChar + tgtValue;   tgtValue = val + tgtValue;

  set();   return true;
  }


bool Registry::deleteValue(String& value) {
int  lng = value.length();
int  i;
int  j;

  if (!tgtOpen || tgtName.isEmpty()) return false;

  if (value.isEmpty()) return del();

  i = find(value);

  if (i < 0) return false;

  j = i + lng;

  String left  = tgtValue.substr(0, prevSep(i));
  String right;

  j = nextSep(j);

  if (j < tgtLng) right = tgtValue.substr(j);

  if (right.isEmpty() && left.isEmpty()) return del();

  if  (left.isEmpty())  return setValue(right);
  if (right.isEmpty())  return setValue(left);

  tgtValue = left + _T(';') + right;   return set();
  }


bool Registry::copyTo(String& newKey) {
  if (!tgtOpen || newKey.isEmpty()) return false;

  tgtName = newKey; set();  return true;
  }


// L"c:\\windows\\system32\\windowspowershell\\v1.0\\"

bool Registry::sanitizeValue() {
String s      = tgtValue;  s.lowerCase();
int    leftX  = 0;
int    rightX = 0;
String t;

  for (rightX = s.find(sepChar); rightX >= 0; rightX = s.find(sepChar, leftX)) {

    t = s.substr(leftX, rightX-leftX);    insertInTable(t);

    leftX = rightX + 1;
    }

  rightX = s.length();

  if (rightX > leftX) {t = s.substr(leftX, rightX-leftX);   insertInTable(t);}

  qsort(&tbl[0], &tbl[tbl.end()-1]);

  for (int i = 0; i < tbl.end(); i++) {Output(tbl[i]);  Output(_T("\n"));}

  Output(_T("\n"));   return true;
  }


void Registry::insertInTable(String& t) {
int i;

  for (i = 0; i < tbl.end(); i++)
      if (t == tbl[i]) {deleteValue(t); t = _T("Duplicated: ") + t + _T("\n");  Output(t); return;}

  tbl += t;
  }


bool Registry::set() {
bool expandVal = tgtValue.find(_T('%')) >= 0;

  tgtLng = tgtValue.length();   bufLng = tgtLng * sizeof(Tchar);

  if (!tgtOpen || !bufLng) return false;

  if (tgtType == REG_SZ        &&  expandVal) tgtType = REG_EXPAND_SZ;
  if (tgtType == REG_EXPAND_SZ && !expandVal) tgtType = REG_SZ;

  if (RegSetValueEx(lvlOneKeyH, tgtName, NULL, tgtType, (Byte*) tgtValue.str(), bufLng))
                                                                                      return false;
  dirty = true;  return true;
  }


int Registry::prevSep(int i) {
  if (i <= 0) return 0;

  for (--i; i >= 0; i--) if (tgtValue[i] != _T(' ')) return i;

  return 0;
  }


int Registry::nextSep(int i) {

  for (; i < tgtLng; i++) if (tgtValue[i] != _T(' ')) return i + 1;

  return tgtLng;
  }


int Registry::find(String& val) {
String tgt = tgtValue;        tgt.lowerCase();
String v   = val;               v.lowerCase();
int    lng = val.length();
int    i;
int    j;

  if (!tgtLng || !lng) return -1;

  for (i = tgt.find(v); i != String::npos; i = tgt.find(v, j)) {
    j = i + lng;
    if (isLeftSepCh(i) && isRightSepCh(i + lng)) return i;
    }
  return -1;
  }


bool Registry::isLeftSepCh(int i) {
Tchar ch = 0;

  if (!i) return true;

  for (--i; i >= 0; i--) {
    ch = tgtValue[i];   if (ch != _T(' ')) break;
    }

  return ch == sepChar;
  }


bool Registry::isRightSepCh(int i) {
Tchar ch;

  if (i >= tgtLng) return true;

  for (i; i < tgtLng; i++) {ch = tgtValue[i];   if (ch != _T(' ')) break;}

  return ch == sepChar;
  }


bool Registry::del() {
bool rslt = RegDeleteValue(lvlOneKeyH, tgtName) == ERROR_SUCCESS;

  if (rslt) {
    tgtOpen = false;   tgtName.clear();   tgtValue.clear();   tgtType = REG_NONE;

    tgtLng = 0;   dirty = true;
    }

  return rslt;
  }


void  Registry::displayTgt() {
String line;

  if (tgtOpen) {

    line = tgtName + _T(" = ") + tgtValue + _T("   Type = ");

    switch (tgtType) {
      case REG_NONE     : line += _T("None");          break;
      case REG_SZ       : line += _T("REG_SZ");        break;
      case REG_EXPAND_SZ: line += _T("REG_EXPAND_SZ"); break;
      default           : line += _T("Other");         break;
      }

    ToAnsi toAnsi(line);  cout << toAnsi() << "   Length: " << tgtLng << "\n";
    }

  else cout << "Key/Value not open\n";
  }


