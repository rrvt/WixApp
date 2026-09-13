// Printer Device Attributes


#pragma once
//#include "PrinterPgSz.h"


enum PrtrOrnt {NilOrnt, PortOrnt = DMORIENT_PORTRAIT, LandOrnt = DMORIENT_LANDSCAPE};

enum Plex     {NilPlx, SimPlx = DMDUP_SIMPLEX, DuPlx = DMDUP_VERTICAL, HorizPlx =DMDUP_HORIZONTAL};


// Structure to pass to app for initial attribute values

struct DriverInfo {
double   width{8.5};
double   length{11};
double   topMgn{0.125};
double   leftMgn{0.125};
double   rightMgn{0.125};
double   bottomMgn{0.125};
};



class DisplayAttr {       //: public PrtrBase {
public:

String   width;                       // Dimensions in inches
String   length;
String   topMgn;                      // Margins needed when printing text
String   leftMgn;
String   rightMgn;
String   bottomMgn;

  DisplayAttr() { }
 ~DisplayAttr() { }

  void    load(CDC* dc, DriverInfo& info);
  void    loadDefaults(DriverInfo& info);
  void    save();

  void    setMargins(  double mgn) {topMgn = leftMgn = rightMgn = bottomMgn = get(mgn);}
  void    setTopMgn(   double mgn) {topMgn    = get(mgn);} // In inches
  void    setLeftMgn(  double mgn) {leftMgn   = get(mgn);}
  void    setRightMgn( double mgn) {rightMgn  = get(mgn);}
  void    setBottomMgn(double mgn) {bottomMgn = get(mgn);}

private:


  String toInchesX(CDC* dc, int pixels);
  String toInchesY(CDC* dc, int pixels);

  String get(double dbl) {return dblToString(dbl, 0, 3);}

  bool   readItem(TCchar* key, int& val, int dflt);
  };


extern DisplayAttr displayAttr;




//////------------

//#include "PrtrBase.h"
#if 0
  void get(HANDLE hdl);

  void set(String& name, HANDLE devMode);
  void set(HANDLE hdl);
  void set(DEVMODE& devMode);
#endif

