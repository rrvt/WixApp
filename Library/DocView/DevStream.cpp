// Stream to display/printer device


#include "pch.h"
#include "DevStream.h"
#include "Devices.h"



static DvStManipInt& setupManipInt(DvStManipInt::Func func, int     val);
static DvStManipDbl& setupManipDbl(DvStManipDbl::Func func, double  val);
static DvStManipStg& setupManipStg(DvStManipStg::Func func, TCchar* val);


DevStream::DevStream(DeviceID id) : device(*devices.get(id)) {
  device.clear();

  dBold.n           = this; dBold.func           = &doBold;
  dItalic.n         = this; dItalic.func         = &doItalic;
  dUnderLine.n      = this; dUnderLine.func      = &doUnderLine;
  dStrikeOut.n      = this; dStrikeOut.func      = &doStrikeOut;
  dFont.n           = this; dFont.func           = &doPrev;

  dClrTabs.n        = this; dClrTabs.func        = &doClrTabs;
  dTab.n            = this; dTab.func            = &doTab;
  dCenter.n         = this; dCenter.func         = &doCenter;
  dRight.n          = this; dRight.func          = &doRight;

  dSetHex.n         = this; dSetHex.func         = &doSetHex;

  dCrlf.n           = this; dCrlf.func           = &doCrlf;
  dCR.n             = this; dCR.func             = &doCR;
  dFlushFtr.n       = this; dFlushFtr.func       = &doFlushFtr;
  };


DvStManipDbl&   dSetLMargin(double val) {return setupManipDbl(DevStream::doSetLMargin, val);}
DvStManipDbl&   dSetRMargin(double val) {return setupManipDbl(DevStream::doSetRMargin, val);}

DvStManipStg&   dFFace( TCchar* val) {return setupManipStg(DevStream::doFFace,      val);}
DvStManipDbl&   dFSize(  double val) {return setupManipDbl(DevStream::doFSize,      val);}
DvStManip       dBold;
DvStManip       dItalic;
DvStManip       dUnderLine;
DvStManip       dStrikeOut;
DvStManip       dFont;

DvStManip       dClrTabs;
DvStManipInt&   dSetTab( int val)   {return setupManipInt(DevStream::doSetTab,  val);}
DvStManipInt&   dSetRTab(int val)   {return setupManipInt(DevStream::doSetRTab, val);}

DvStManip       dTab;                     // add to stream to tab to next tab position:
                                            // dsp << dvsTab << "xyz";
DvStManip       dCenter;                  // center the string following up to the nCrlf
DvStManip       dRight;                   // right align the string following up to the nCrlf

// Specify immediately before an integer or double to be effective.  Only effective for one value
DvStManipInt&   dSetWidth(int val)  {return setupManipInt(DevStream::doSetWidth, val);}

DvStManipInt&   dSetPrec( int val)  {return setupManipInt(DevStream::doSetPrec,  val);}
DvStManip       dSetHex;                  // Specify that next integer (unsigned or signed) be
                                            // output as hex
DvStManip       dBeginLine;
DvStManip       dEndLine;

DvStManip       dCrlf;
DvStManip       dCR;                      // add to stream to perform a carriage return (only)
DvStManip       dEndPage;                 // add to stream to terminate a page when printing or
                                            // do nothing
DvStManip       dFlushFtr;                // add to stream to terminate a footer when printing





// Setup a Node with the data, then the << operator implements the function specified in the node

DvStManipInt& setupManipInt(DvStManipInt::Func fn, int val)
          {NewAlloc(DvStManipInt);   DvStManipInt* m = AllocNode;   m->set(fn, val);   return *m;}

DvStManipDbl& setupManipDbl(DvStManipDbl::Func fn, double val)
          {NewAlloc(DvStManipDbl);   DvStManipDbl* m = AllocNode;   m->set(fn, val);   return *m;}

DvStManipStg& setupManipStg(DvStManipStg::Func fn, TCchar* val)
          {NewAlloc(DvStManipStg);   DvStManipStg* m = AllocNode;   m->set(fn, val);   return *m;}


