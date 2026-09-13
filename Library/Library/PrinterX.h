// Global Printer


#pragma once


enum PrtrOrient {NilOrient, PortOrient = DMORIENT_PORTRAIT, LandOrient = DMORIENT_LANDSCAPE};

enum PagePlex   {NilPlex,   SimPlex    = DMDUP_SIMPLEX,     DuPlex = DMDUP_VERTICAL,
                                                                      HorizPlex =DMDUP_HORIZONTAL};

enum PaperSize {NilPSz,
                LetterPSz             = DMPAPER_LETTER,
                LetterSmallPSz        = DMPAPER_LETTERSMALL,
                TabloidPSz            = DMPAPER_TABLOID,
                LedgerPSz             = DMPAPER_LEDGER,
                LegalPSz              = DMPAPER_LEGAL,
                StatementPSz          = DMPAPER_STATEMENT,
                ExecutivePSz          = DMPAPER_EXECUTIVE,
                TenX14PSz             = DMPAPER_10X14,
                ElevenX17PSz          = DMPAPER_11X17,
                NotePSz               = DMPAPER_NOTE,
                Env_9PSz              = DMPAPER_ENV_9,
                Env_10PSz             = DMPAPER_ENV_10,
                Env_11PSz             = DMPAPER_ENV_11,
                Evn_12PSz             = DMPAPER_ENV_12,
                Env_14PSz             = DMPAPER_ENV_14,
                CSheetPSz             = DMPAPER_CSHEET,
                DSheetPSz             = DMPAPER_DSHEET,
                ESheetPSz             = DMPAPER_ESHEET,
                Env_MonarchPSz        = DMPAPER_ENV_MONARCH,
                Env_PersonalPSz       = DMPAPER_ENV_PERSONAL,
                FanFold_USPSz         = DMPAPER_FANFOLD_US,
                FanFold_Std_GermanPSz = DMPAPER_FANFOLD_STD_GERMAN,
                FanFold_Lgl_GermanPSz = DMPAPER_FANFOLD_LGL_GERMAN,
                NineX11PSz            = DMPAPER_9X11,
                TenX11PSz             = DMPAPER_10X11,
                FifteenX11PSz         = DMPAPER_15X11,
                Letter_ExtraPSz       = DMPAPER_LETTER_EXTRA,
                Legal_ExtraPSz        = DMPAPER_LEGAL_EXTRA,
                Tabloid_ExtraPSz      = DMPAPER_TABLOID_EXTRA,
                A4_ExtraPSz           = DMPAPER_A4_EXTRA,
                Letter_TransversePSz  = DMPAPER_LETTER_TRANSVERSE,
                UserDefinedPSz        = DMPAPER_USER};

extern TCchar* PortraitKey;
extern TCchar* LandscapeKey;



class PrinterX {

bool       loaded{false};
DEVMODE*   devMode{0};

public:

String     name;                        // DevMode Parameters
PrtrOrient orient{NilOrient};
PaperSize  paperSize{LetterPSz};
int        width{0};
int        length{0};
int        copies{1};
int        collate{false};
PagePlex   pagePlex{NilPlex};

double     topMargin{0.0};                   // Page Parameters
double     botMargin{0.0};
double     leftOdd{0.0};
double     rightOdd{0.0};
double     leftEven{0.0};
double     rightEven{0.0};
double     scale{0.0};

  Printer() { }
 ~Printer() { }

  bool readDevAttr(TCchar* name);
  void readPageAttr(TCchar* name);

  void getDevMode(HANDLE hdl);
  bool setDevMode(HANDLE hdl);

  void set(PrtrOrient o)  {orient = o;}
  void set(PaperSize  sz) {paperSize = sz;}
  void set(int width, int length);

  PrtrOrient toOrient(Cstring& cs);
  TCchar*    toStg(PrtrOrient orient);

  void       load(TCchar* printer);
  void       save();

private:

  void getDevMode();
  bool setDevMode();
  void getDevName();
  void setDevName();
  void setPaperSize();

  bool readItem(TCchar* key, int& val, int dflt);
  };


extern Printer printer;




////////////-----------
#if 0
  void        getAttr(HANDLE hdl);
  void        save();

  void        setDevMode(HANDLE hdl);
  void        saveAttr(HANDLE hdl);
  String&     getName( HANDLE hdl);

  void       load(TCchar* printer);

  HANDLE     getHandle(HWND hWnd);
  void       freeHandle();
//  HGLOBAL    createDevnames(TCchar* driver, TCchar* device, TCchar* output);


  void       set(PrtrOrient o)  {orient = o;}
  void       set(PaperSize  sz) {paperSize = sz;}
  void       set(int width, int length);


private:


//bool       loaded{false};
//HANDLE     hPrinter;
//HANDLE     hDevMode;

#endif
//#include "wingdi.h"

