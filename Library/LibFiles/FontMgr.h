// Font Manager
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.


#pragma once
#include "ExpandableP.h"

/*
typedef struct tagLOGFONTW {
  LONG  lfHeight;
  LONG  lfWidth;
  LONG  lfEscapement;
  LONG  lfOrientation;
  LONG  lfWeight;
  BYTE  lfItalic;
  BYTE  lfUnderline;
  BYTE  lfStrikeOut;
  BYTE  lfCharSet;
  BYTE  lfOutPrecision;
  BYTE  lfClipPrecision;
  BYTE  lfQuality;
  BYTE  lfPitchAndFamily;
  WCHAR lfFaceName[LF_FACESIZE];
  } LOGFONTW, *PLOGFONTW, *NPLOGFONTW, *LPLOGFONTW;

*/


struct FontAttr {
CDC*   dc;
int    sz;
bool   bold;
bool   italic;
bool   underline;
bool   strikeout;
String face;

  FontAttr() {clear();}
  FontAttr(FontAttr& fd) {copy(fd);}

  void clear() {dc = 0; sz = 0; bold = italic = underline = strikeout = false; face.clear();}

  FontAttr& operator= (FontAttr& fd) {copy(fd); return *this;}

private:

  void copy(FontAttr& src) {
    dc        = src.dc;           sz        = src.sz;          bold = src.bold;  italic = src.italic;
    underline = src.underline;    strikeout = src.strikeout;   face = src.face;
    }
  };




typedef RcdPtrT<FontAttr> FontAttrPB;


class FontAttrP : public FontAttrPB {

public:


  };


class FontMgr {
int                       stkX;
ExpandableP<FontAttr, FontAttrP, 4> stk;

public:
LOGFONT curLogFont;

  FontMgr();
 ~FontMgr();

  void clear();

  void initialize(TCchar* face, int fontSize, CDC* dc);
  void pop();
  void setSize(int fontSize);
  void setBold();
  void setItalic();
  void setUnderLine();
  void setStrikeOut();

  FontAttr& getAttr() {return stk.getData(stkX);}

  LOGFONT&  getLogFont() {return curLogFont;}

private:

  FontAttr& push();
  void      install(FontAttr& attr, CFont& font);
  void      update(FontAttr& attr, TCchar* fn);
  void      createFailed(TCchar* fn);
  };




#if 0
class FontAttrP {
public:
FontAttr* p;

  FontAttrP() {p = 0;}
  FontAttrP(FontAttrP& attr) {p = attr.p;}
 ~FontAttrP() {p = 0;}

  void       clear() {if (p) delete p; p = 0;}

  FontAttrP& operator= (FontAttrP& attr) {p = attr.p; return *this;}

  FontAttr* operator() ();
  };
#endif

