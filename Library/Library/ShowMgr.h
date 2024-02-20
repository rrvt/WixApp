// Show Manager -- Parent class for DsplyMgr and PrintMgr


#pragma once
#include "ToDevBase.h"

class CScrView;
class NotePad;


class ShowMgr {

protected:

CScrView&  vw;
NotePad&   npd;
ToDevBase& dpDev;

String     font;
double     fontSize;                    // in points, e.g. 12.3

public:

bool wrapEnabled;

          ShowMgr(CScrView& view, NotePad& notePad, ToDevBase& dspPrtDv) : vw(view), npd(notePad),
                                dpDev(dspPrtDv), font(_T("Arial")), fontSize(12.0), wrapEnabled(false) { }
         ~ShowMgr() { }

          void     setFont(TCchar* f, double points = 12.0) {font = f; fontSize = points;}

          double   getFontScale()    {return dpDev.getFontScale();}
          void     setFontScale(double scl) {dpDev.setFontScale(scl);}

          NotePad& getNotePad() {return npd;}

          DevBase& getDevDC(CDC*& dc) {return dpDev.getDevDC(dc);}

private:

  ShowMgr() : vw(*(CScrView*)0), npd(*(NotePad*)0), dpDev(*(ToDevBase*)0) { }
  };
