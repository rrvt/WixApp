// Global Printer


#pragma once
#include "wingdi.h"


enum PrtrOrient {NilOrient, PortOrient = DMORIENT_PORTRAIT, LandOrient = DMORIENT_LANDSCAPE};
enum PagePlex   {NilPlex, SimPlex = DMDUP_SIMPLEX, DuPlex = DMDUP_VERTICAL, HorizPlex =DMDUP_HORIZONTAL};
enum PaperSize  {NilPprSz, LetterPprSz = DMPAPER_LETTER,     LetterSmallPprSz = DMPAPER_LETTERSMALL,
                 TabloidPprSz = DMPAPER_TABLOID,             LedgerPprSz = DMPAPER_LEDGER,
                 LegalPprSz = DMPAPER_LEGAL,                 StatementPprSz = DMPAPER_STATEMENT,
                 ExecutivePprSz = DMPAPER_EXECUTIVE,         TenX14PprSz = DMPAPER_10X14,
                 ElevenX17PprSz = DMPAPER_11X17,             NotePprSz = DMPAPER_NOTE,
                 Env_9PprSz = DMPAPER_ENV_9,                 Env_10PprSz = DMPAPER_ENV_10,
                 Env_11PprSz = DMPAPER_ENV_11,               Evn_12PprSz = DMPAPER_ENV_12,
                 Env_14PprSz = DMPAPER_ENV_14,               CSheetPprSz = DMPAPER_CSHEET,
                 DSheetPprSz = DMPAPER_DSHEET,               ESheetPprSz = DMPAPER_ESHEET,
                 Env_MonarchPprSz = DMPAPER_ENV_MONARCH,     Env_PersonalPprSz = DMPAPER_ENV_PERSONAL,
                 FanFold_USPprSz = DMPAPER_FANFOLD_US,
                 FanFold_Std_GermanPprSz = DMPAPER_FANFOLD_STD_GERMAN,
                 FanFold_Lgl_GermanPprSz = DMPAPER_FANFOLD_LGL_GERMAN,
                 NineX11PprSz = DMPAPER_9X11,                TenX11PprSz = DMPAPER_10X11,
                 FifteenX11PprSz = DMPAPER_15X11,            Letter_ExtraPprSz = DMPAPER_LETTER_EXTRA,
                 Legal_ExtraPprSz = DMPAPER_LEGAL_EXTRA,     Tabloid_ExtraPprSz = DMPAPER_TABLOID_EXTRA,
                 A4_ExtraPprSz = DMPAPER_A4_EXTRA,
                 Letter_TransversePprSz = DMPAPER_LETTER_TRANSVERSE};

extern TCchar* PortraitKey;
extern TCchar* LandscapeKey;



struct Printer {

String     name;
PrtrOrient orient;
PaperSize  paperSize;
PagePlex   pagePlex;
int        copies;
int        collate;
double     topMargin;
double     botMargin;
double     leftOdd;
double     rightOdd;
double     leftEven;
double     rightEven;
double     scale;

  Printer() : orient(NilOrient), paperSize(LetterPprSz), pagePlex(NilPlex), copies(1), collate(false),
              topMargin(0.0), botMargin(0.0), leftOdd(0.0), rightOdd(0.0), leftEven(0.0), rightEven(0.0),
              scale(0.0) { }
 ~Printer() { }

  void       load(TCchar* printer);
  void       store();

  PrtrOrient toOrient(Cstring& cs);
  TCchar*    toStg(PrtrOrient orient);

  void       set(PrtrOrient o) {orient = o;}
  };

extern Printer printer;




  //  void operator() (CScrView* vw);
//  void setOrient(PrtrOrient v) {orient = v; store();}

