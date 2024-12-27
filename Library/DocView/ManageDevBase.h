// Show Manager -- Parent class for DsplyMgr and PrintMgr


#pragma once
#include "NtPdToDev.h"

class CScrView;
class NotePad;


class ManageDevBase {

protected:

CScrView&  vw;
NotePad&   npd;
NtPdToDev& dpDev;

String     font;
double     fontSize;                        // in points, e.g. 12.3

public:

bool       wrapEnabled;

           ManageDevBase(CScrView& view, NotePad& notePad, NtPdToDev& dspPrtDv) :
                    vw(view), npd(notePad), dpDev(dspPrtDv), font(_T("Arial")), fontSize(12.0),
                    wrapEnabled(false) { }

          ~ManageDevBase() { }

  void     setFont(TCchar* f, double points = 12.0) {font = f; fontSize = points;}

  double   getFontScale()    {return dpDev.getFontScale();}
  void     setFontScale(double scl) {dpDev.setFontScale(scl);}

  NotePad& getNotePad() {return npd;}

private:

  ManageDevBase() : vw(*(CScrView*)0), npd(*(NotePad*)0), dpDev(*(NtPdToDev*)0) { }
  };
