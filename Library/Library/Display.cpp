// Display attributes


#include "pch.h"
#include "Display.h"
#include "IniFile.h"


static TCchar* Section  = _T("Display");
static TCchar* ScaleKey = _T("DspScale");


Display display;


void Display::load() {
String s;
uint   x;

  iniFile.read(Section, ScaleKey, s, _T("6060.0"));   scale = s.stod(x);
  }


void Display::store() {String s = scale;   iniFile.writeString(Section, ScaleKey, s);}

