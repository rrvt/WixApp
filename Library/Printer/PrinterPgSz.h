// Printer Page Size -- array of parameter records


#pragma once


enum PaperSz  {NilPSz,
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
               UserDefinedPSz        = DMPAPER_USER
               };


class PrinterPgSz {

public:

  PrinterPgSz() { }
 ~PrinterPgSz() { }

  void set(CComboBox& cb, int initial);

        int     findItem(PaperSz pgSz);               // return index of entry
        int     find(TCchar* width, TCchar* length);  // ditto
        bool    isUserDefined(int i);
        TCchar* getName(      int i);
        PaperSz getItem(      int i);
  const String& getWidth(     int i);
  const String& getLength(    int i);
  };

