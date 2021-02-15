// Global Options


#include "stdafx.h"
#include "Options.h"
#include "IniFile.h"
#include "OptionsDlg.h"



static TCchar* Section        = _T("Options");
static TCchar* Orientation    = _T("Orientation");
static TCchar* TopMarginKey   = _T("TopMargin");
static TCchar* LeftMarginKey  = _T("LeftMargin");
static TCchar* RightMarginKey = _T("RightMargin");
static TCchar* BotMarginKey   = _T("BotMargin");

Options options;


void Options::operator() (CScrView* vw) {
OptionsDlg dlg;
String     s;
uint       x;

  load();

  s = dspScale;   dlg.dspScale = s;

  dlg.orient      = orient == Portrait ? PortraitKey : LandscapeKey;
  dlg.topMargin   = topMargin;
  dlg.leftMargin  = leftMargin;
  dlg.rightMargin = rightMargin;
  dlg.botMargin   = botMargin;

  s = prtScale;   dlg.prtScale = s;

  if (dlg.DoModal() == IDOK) {

    s = dlg.dspScale;  dspScale = s.stod(x);

    s = dlg.orient;   orient = s == LandscapeKey ? Landscape : Portrait;
    topMargin    = dlg.topMargin;
    leftMargin   = dlg.leftMargin;
    rightMargin  = dlg.rightMargin;
    botMargin    = dlg.botMargin;

    s = dlg.prtScale;  prtScale = s.stod(x);

    store();

    vw->setFontScale(false, dspScale);
    vw->setFontScale(true,  prtScale);
    }
  }


void Options::load() {
String excel;
String s;
uint   x;


  iniFile.readString(FontSection, DspScaleKey, s, _T("6060.0"));   dspScale = s.stod(x);

  orient = (PrtrOrient) iniFile.readInt(Section, Orientation, Portrait);
  iniFile.readString(Section, TopMarginKey,   topMargin,   _T("3.0"));
  iniFile.readString(Section, LeftMarginKey,  leftMargin,  _T("3.0"));
  iniFile.readString(Section, RightMarginKey, rightMargin, _T("3.0"));
  iniFile.readString(Section, BotMarginKey,   botMargin,   _T("3.0"));

  iniFile.readString(FontSection, PrtScaleKey, s, _T("7175.0"));   prtScale = s.stod(x);
  }


void Options::store() {
String s;

  s = dspScale;   iniFile.writeString(FontSection, DspScaleKey, s);

  iniFile.writeInt(   Section, Orientation,    orient);
  iniFile.writeString(Section, TopMarginKey,   topMargin);
  iniFile.writeString(Section, LeftMarginKey,  leftMargin);
  iniFile.writeString(Section, RightMarginKey, rightMargin);
  iniFile.writeString(Section, BotMarginKey,   botMargin);

  s = prtScale;   iniFile.writeString(FontSection, PrtScaleKey, s);
  }

