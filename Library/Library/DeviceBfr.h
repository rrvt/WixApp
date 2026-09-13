// Some operations cannot take place at the invocation when the buffer has content that "precedes"
// the operation, i.e. left over from the last np node due to an end of line or end of page.


#pragma once
#include "NoteTab.h"

class  CDCex;
class  DeviceCtx;


class DeviceBfr {

CDCex& cdcEx;

String buf;
int    w{0};

public:
              DeviceBfr(CDCex& cdcEX) : cdcEx(cdcEX) { }
             ~DeviceBfr() { }

  bool        isEmpty() {return buf.isEmpty();}

  DeviceBfr&  operator += (TCchar* tc) {buf += tc;   return *this;}
  TCchar*     operator() () {return buf;}

  int         getExtent(int pos);
  void        textOut(DeviceCtx* ctx, int pos);

  int         findNextBrk(int pos);
  int         length()   {return buf.length();}
  String&     trimLeft() {return buf.trimLeft();}

private:

  DeviceBfr() : cdcEx(*(CDCex*)0) { }
  };






/////////////-----------------
//  void        setNewPageUL() {set(NwPgDfr);}

//  bool        doBegUnderLine();
//  bool        doEndUnderLine();
//DeviceCtx& ctx;
#if 0
enum DeferItem {NilDfr =0,
                LMgnDfr=0x80000000,
                RMgnDfr=0x40000000,
                FaceDfr=0x20000000,
                FSzeDfr=0x10000000,
                BoldDfr=0x08000000,
                ItlcDfr=0x04000000,
                FUnLDfr=0x02000000,
                StkODfr=0x01000000,
                PopFDfr=0x00800000,
                ClTbDfr=0x00400000,
                StTbDfr=0x00200000,
                MvTbDfr=0x00100000,
                CntrDfr=0x00080000,
                RghtDfr=0x00040000,
                BgULDfr=0x00020000,
                TextDfr=0x00010000,
                EnULDfr=0x00008000,
                NwPgDfr=0x00004000
                };

const uint PriorAttr = 0xfffe0000;
#endif
#if 0
  bool        doLeftMargin(double v);
  bool        doRightMargin(double v);

  bool        doSetFace(TCchar* face);
  bool        doSetFSize(int tenths);
  bool        doBold();
  bool        doItalic();
  bool        doFUnderline();
  bool        doStrikeout();
  bool        doPopFont();

  bool        doClrTabs();
  bool        dosetTab(NoteTab& tab);
  bool        doMoveNextTab();

  bool        doCenter();
  bool        doRight();

  void        doText(TCchar* txt);

  void        mergeDfrd();
#endif
#if 0
uint   defer{NilDfr};                   // One bit for each operation in an np node

double  dfrLMgnV{0};
double  dfrRMgnV{0};
String  dfrFaceV;
int     dfrFSize{0};                     // in 10ths of a point (i.e. point x 10.0)
NoteTab dfrTab;
String  dfrText;
#endif
#if 0
  void        set(DeferItem dfr) {defer |=  dfr;}
  void        clr(DeferItem dfr) {defer ^=  dfr;}
  bool        isDeferred(DeferItem dfr) {return (defer & dfr) != 0;}
#endif


