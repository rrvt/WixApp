// Show Manager -- Parent class for DsplyMgr and PrintMgr


#pragma once
#include "DisplayDev.h"

class CScrView;


class ShowMgr {

protected:

String      id;

CScrView&   vw;

NotePad&    npd;
DisplayDev  dspDev;

String      font;
double      fontSize;                 // in points, e.g. 12.3

double      leftMargin;
double      topMargin;
double      rightMargin;
double      botMargin;

public:

  ShowMgr(TCchar* src, CScrView& view, NotePad& notePad);
 ~ShowMgr() { }

  void     setFont(TCchar* f, double points = 12.0) {font = f; fontSize = points;}

  void     setMgns(double left, double top, double right, double bot);

  double   getFontScale() {return dspDev.getFontScale();}
  void     setFontScale(double scl) {dspDev.setFontScale(scl);}

  NotePad& getNotePad() {return npd;}
  Device&  getDev()     {return dspDev.getDisplay();}

private:

  ShowMgr() : npd(*(NotePad*)0), dspDev(_T(""), *(NotePad*)0), vw(*(CScrView*)0) { }
  };

