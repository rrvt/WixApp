// Printer Device Attributes


#pragma once
#include "PrinterPgSz.h"


extern TCchar* PortraitKey;
extern TCchar* LandscapeKey;


enum PrtrOrnt {NilOrnt, PortOrnt = DMORIENT_PORTRAIT, LandOrnt = DMORIENT_LANDSCAPE};

enum Plex     {NilPlx, SimPlx = DMDUP_SIMPLEX, DuPlx = DMDUP_VERTICAL, HorizPlx =DMDUP_HORIZONTAL};


// Structure to pass to app for initial attribute values

struct PrinterInfo {
String   docName;
int      orient{PortOrnt};
int      copies{1};
int      collate{1};
int      color{DMCOLOR_MONOCHROME};
int      plex{DMDUP_SIMPLEX};
PaperSz  paperSz{LetterPSz};
double   width{8.5};
double   length{11};
double   topMgn{0.125};
double   leftMgn{0.125};
double   rightMgn{0.125};
double   bottomMgn{0.125};
};



class PrinterAttr {       //: public PrtrBase {
public:

String   name;                        // Printer Name & DevMode+ Parameters
PrtrOrnt orient{NilOrnt};
int      copies{0};
int      collate{-1};
int      color{0};
int      plex{NilPlx};
PaperSz  paperSz{NilPSz};
String   width;                       // Dimensions in inches
String   length;
String   topMgn;                      // Margins needed when printing text
String   leftMgn;
String   rightMgn;
String   bottomMgn;

  PrinterAttr() { }
 ~PrinterAttr() { }

  void clear() {name.clear();}

  void initialize(PrinterInfo& info);

  void load(PrinterInfo& info);
  void load(TCchar* prtrName, PrinterInfo& info);
  void loadDefaults(PrinterInfo& info);
  void save();

  void    set(PrtrOrnt o)  {orient  =  o;}         // Set attr from app not printer dialog
  void    set(PaperSz  sz) {paperSz = sz;}
  void    set(double width, double length);        // width and length are in inches, sets paperSz
  void    setCopies(   int      v) {copies    = v;}
  void    setMargins(  double mgn) {topMgn = leftMgn = rightMgn = bottomMgn = get(mgn);}
  void    setTopMgn(   double mgn) {topMgn    = get(mgn);} // In inches
  void    setLeftMgn(  double mgn) {leftMgn   = get(mgn);}
  void    setRightMgn( double mgn) {rightMgn  = get(mgn);}
  void    setBottomMgn(double mgn) {bottomMgn = get(mgn);}

  String& getDevName(HANDLE hdl);
  String& getDevName(DEVMODE& devMode);
  void    setDevName(DEVMODE* devMode);

private:

  String getCurrentName();
  String get(double dbl) {return dblToString(dbl, 0, 3);}
  void   getDevMode();
  bool   setDevMode();
  void   validatePaperSz();

  bool   readItem(TCchar* key, int& val, int dflt);
  };


extern PrinterAttr prtrDevAttr;




//////------------

//#include "PrtrBase.h"
#if 0
  void get(HANDLE hdl);

  void set(String& name, HANDLE devMode);
  void set(HANDLE hdl);
  void set(DEVMODE& devMode);
#endif

