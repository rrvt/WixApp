// Stream to display/printer device


#pragma once
#include "Devices.h"
#include "ManipT.h"


class DevStream;
typedef ManipT<   DevStream> DvStManip;
typedef ManipIntT<DevStream> DvStManipInt;
typedef ManipDblT<DevStream> DvStManipDbl;
typedef ManipStgT<DevStream> DvStManipStg;


class DevStream {

Device&  device;
NoteNmbr nmbr;

public:                                                //IntNmbrTyp, UIntNmbrTyp, DblNmbTyp

  DevStream(DeviceID id);
 ~DevStream() { }

  void       init(Device& dev) {device = dev;}

  void       setFontScale( double scale) {device.setFontScale(scale);}
  void       setWrapEnable(bool   enbld) {device.setWrapEnable(enbld);}
  void       setFooter(    bool   on)    {device.setFooter(on);}
  void       setBottom()                 {device.setBottom();}
  bool       doEndPageChk()       {return device.doEndPageChk();}

  DevStream& operator<< (String&     s)  {device.append(s); return *this;}
  DevStream& operator<< (TCchar*     s)  {device.append(s); return *this;}

  DevStream& operator<< (int         v)
    {nmbr.longVal  = v;   nmbr.typ = IntNmbrTyp;   device.append(nmbr);   nmbr.clear();   return *this;}
  DevStream& operator<< (ulong       v)
    {nmbr.uLongVal = v;   nmbr.typ = UIntNmbrTyp;  device.append(nmbr);   nmbr.clear();   return *this;}
  DevStream& operator<< (double      v)
    {nmbr.dblVal   = v;   nmbr.typ = DblNmbTyp;    device.append(nmbr);   nmbr.clear();   return *this;}

  DevStream& operator<< (DvStManip& m) {return m.func(*this);}

  DevStream& operator<< (DvStManipInt& m)
                      {m.func(*this, m.v);   NewAlloc(DvStManipInt);   FreeNode(&m); return *this;}

  DevStream& operator<< (DvStManipDbl& m)
                      {m.func(*this, m.v);   NewAlloc(DvStManipDbl);   FreeNode(&m); return *this;}

  DevStream& operator<< (DvStManipStg& m)
                      {m.func(*this, m.v);   NewAlloc(DvStManipStg);   FreeNode(&m); return *this;}
private:

  static DevStream& doSetLMargin(DevStream& n, double v) {n.device.setLeftMargin(v);    return n;}
  static DevStream& doSetRMargin(DevStream& n, double v) {n.device.setRightMargin(v);   return n;}

  static DevStream& doFFace(    DevStream& n, String& s) {n.device.setFontFaceII(s);  return n;}
  static DevStream& doFSize(    DevStream& n, double  v) {n.device.setFontSizeII(v);  return n;}
  static DevStream& doBold(     DevStream& n)            {n.device.setBold();         return n;}
  static DevStream& doItalic(   DevStream& n)            {n.device.setItalic();       return n;}
  static DevStream& doUnderLine(DevStream& n)            {n.device.setUnderLine();    return n;}
  static DevStream& doStrikeOut(DevStream& n)            {n.device.setStrikeOut();    return n;}
  static DevStream& doPrev(     DevStream& n)            {n.device.popFontAttr();     return n;}

  static DevStream& doClrTabs(  DevStream& n)            {n.device.clrTabs();         return n;}
  static DevStream& doSetTab(   DevStream& n, int val)   {NoteTab nt(val); n.device.setTab(nt); return n;}
  static DevStream& doSetRTab(  DevStream& n, int val)
                                            {NoteTab nt(val, true); n.device.setTab(nt); return n;}

  static DevStream& doTab(      DevStream& n)            {n.device.setTabSeen();      return n;}
  static DevStream& doCenter(   DevStream& n)            {n.device.setCenter();       return n;}
  static DevStream& doRight(    DevStream& n)            {n.device.setRight();        return n;}

  static DevStream& doSetWidth( DevStream& n, int val)   {n.device.nmbr.width = val;  return n;}
  static DevStream& doSetPrec(  DevStream& n, int val)   {n.device.nmbr.prec  = val;  return n;}
  static DevStream& doSetHex(   DevStream& n)            {n.device.nmbr.hex   = true; return n;}

  static DevStream& doCrlf(     DevStream& n)            {n.device.crlf();            return n;}
  static DevStream& doCR(       DevStream& n)            {n.device.cr();              return n;}
  static DevStream& doFlushFtr( DevStream& n)            {n.device.flushFtr();        return n;}

  friend DvStManipDbl& dSetLMargin(double val);
  friend DvStManipDbl& dSetRMargin(double val);

  friend DvStManipStg& dFFace( TCchar* val);
  friend DvStManipDbl& dFSize(  double val);

  friend DvStManipInt& dSetTab(    int val);
  friend DvStManipInt& dSetRTab(   int val);

  friend DvStManipInt& dSetWidth(  int val);
  friend DvStManipInt& dSetPrec(   int prec);

  friend class DeviceDisplay;
  friend class DevicePrinter;
  friend class NtPdToDisplay;
  friend class NtPdToPrinter;
  };


DvStManipDbl&    dSetLMargin(double val);
DvStManipDbl&    dSetRMargin(double val);

DvStManipStg&    dFFace(TCchar*     val);
DvStManipDbl&    dFSize(double      val);

extern DvStManip dBold;
extern DvStManip dItalic;
extern DvStManip dUnderLine;
extern DvStManip dStrikeOut;
extern DvStManip dFont;

extern DvStManip dClrTabs;
DvStManipInt&    dSetTab(    int val);
DvStManipInt&    dSetRTab(   int val);

extern DvStManip dTab;                    // add to stream to tab to next tab position:
                                          // dsp << dvsTab << "xyz";
extern DvStManip dCenter;                 // center the string following up to the nCrlf
extern DvStManip dRight;                  // right align the string following up to the nCrlf

// Specify immediately before an integer or double to be effective.  Only effective for one value
DvStManipInt&    dSetWidth(  int val);    // Set width for next integer or double, negativ
                                          // indicates left adj
DvStManipInt&    dSetPrec(   int prec);   // Set precision (no of digits)
extern DvStManip dSetHex;                 // Set hex modifier for next integer value (unsigned or
                                          // signed)
extern DvStManip dBeginLine;              // begin line here
extern DvStManip dEndLine;                // end line here -- Does not affect current position

extern DvStManip dCrlf;                   // add to stream to terminate a line on display:
                                          // dsp << "xyz" << dvsCrlf;
extern DvStManip dCR;                     // add to stream to perform a carriage return (only)
extern DvStManip dEndPage;                // add to stream to terminate a page when printing or
                                          // do nothing
extern DvStManip dFlushFtr;               // add to stream to terminate a footer when printing

