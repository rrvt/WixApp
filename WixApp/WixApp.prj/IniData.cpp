// Get data from ini file determined by debug state



#include "pch.h"
#include "IniData.h"
#include "IniFileEx.h"


IniData iniData;

static TCchar* CurSection = _T("Current");
static TCchar* DbgSection = _T("Debug");


bool IniData::read(TCchar* key, String& val, TCchar* dflt) {
#ifdef _DEBUG
String section = DbgSection;
#else
String section = CurSection;
#endif

  return iniFile.read(section, key, val, dflt);
  }


void   IniData::write(TCchar* key, TCchar* val) {
#ifdef _DEBUG
String section = DbgSection;
#else
String section = CurSection;
#endif

  iniFile.write(section, key, val);
  }


void   IniData::delStg(TCchar* key) {
#ifdef _DEBUG
String section = DbgSection;
#else
String section = CurSection;
#endif

  iniFile.deleteString(section, key);
  }

