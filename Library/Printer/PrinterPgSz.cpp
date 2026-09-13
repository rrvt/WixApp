// Printer Page Size -- array of parameter records


#include "pch.h"
#include "PrinterPgSz.h"


struct PageSizeItem {
String  name;
PaperSz item;                 // DevMode page size enum
String  width;                // Width in inches (often a floating point value)
String  length;               // Length in inches

  PageSizeItem(TCchar* n, PaperSz itm, TCchar* w, TCchar* l)
                                                     {name = n; item = itm; width = w; length = l;}
private:

  PageSizeItem() {  }
  };



static const String NilS;

static const PageSizeItem pgSz[] =
                                  {{_T("Letter"),       LetterPSz,      _T("8.5"),   _T("11")},
                                   {_T("Legal"),        LegalPSz,       _T("8.5"),   _T("14")},
                                   {_T("10X14"),        TenX14PSz,      _T("10"),    _T("14")},
                                   {_T("11X17"),        ElevenX17PSz,   _T("11"),    _T("17")},
                                   {_T("#9 Envelope"),  Env_9PSz,       _T("3.875"), _T("8.875")},
                                   {_T("#10"),          Env_10PSz,      _T("4.125"), _T("9.5")},
                                   {_T("#11"),          Env_11PSz,      _T("4.5"),   _T("10.375")},
                                   {_T("#12"),          Evn_12PSz,      _T("4.75"),  _T("11")},
                                   {_T("#14"),          Env_14PSz,      _T("5"),     _T("11.5")},
                                   {_T("C Sheet"),      CSheetPSz,      _T("17"),    _T("22")},
                                   {_T("D Sheet"),      DSheetPSz,      _T("22"),    _T("34")},
                                   {_T("E Sheet"),      ESheetPSz,      _T("34"),    _T("44")},
                                   {_T("User Defined"), UserDefinedPSz, _T(""),      _T("")}
                                   };


void PrinterPgSz::set(CComboBox& cb, int initial) {
int i;
int j;
int init = -1;

  for (i = 0; i < noElements(pgSz); i++) {j = cb.AddString(pgSz[i].name);   cb.SetItemData(j, i);}

  init = cb.FindStringExact(-1, pgSz[0].name);

  if (init >= 0) cb.SetCurSel(init);
  }


int PrinterPgSz::findItem(PaperSz item) {
int i;

  for (i = 0; i < noElements(pgSz); i++) if (pgSz[i].item == item) return i;

  return -1;
  }


int PrinterPgSz::find(TCchar* width, TCchar* length) {
int i;

  for (i = 0; i < noElements(pgSz); i++) {
    const PageSizeItem& item = pgSz[i];

    if (item.width == width && item.length == length) return i;
    }

  return i-1;     // User Defined Entry
  }


bool PrinterPgSz::isUserDefined(int i) {return i == noElements(pgSz) - 1;}


TCchar* PrinterPgSz::getName(int i)
                             {return 0 <= i && i < noElements(pgSz) ? pgSz[i].name.str() : _T("");}

PaperSz PrinterPgSz::getItem(int i)
                                {return 0 <= i && i < noElements(pgSz) ? pgSz[i].item   : NilPSz;}
const String& PrinterPgSz::getWidth( int i)
                                {return 0 <= i && i < noElements(pgSz) ? pgSz[i].width  : NilS;}
const String& PrinterPgSz::getLength(int i)
                                {return 0 <= i && i < noElements(pgSz) ? pgSz[i].length : NilS;}

