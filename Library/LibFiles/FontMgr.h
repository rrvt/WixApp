// Font Manager
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.


#pragma once
#include "Expandable.h"


struct FontAttributes {
CDC*   dc;
int    sz;
bool   bold;
bool   italic;
bool   underline;
bool   strikeout;
String face;

  FontAttributes() : dc(0), sz(0), bold(false), italic(false), underline(false), strikeout(false) {}
  FontAttributes(FontAttributes& fd) {copy(fd, *this);}

  FontAttributes& operator= (FontAttributes& fd) {copy(fd, *this); return *this;}

private:

  void copy(FontAttributes& src, FontAttributes& dst) {
    dst.dc = src.dc;   dst.sz = src.sz;    dst.bold = src.bold;  dst.italic = src.italic;
    dst.underline = src.underline;   dst.strikeout = src.strikeout;   dst.face = src.face;
    }
  };


class FontMgr {
CFont*                         original;
int                            stkX;
Expandable <FontAttributes, 4> stk;

public:

  FontMgr() : stkX(0), original(0) {}
 ~FontMgr();

  void initialize(TCchar* face, int fontSize, CDC* dc);
  void setSize(   int fontSize);
  void setBold();
  void setItalic();
  void setUnderLine();
  void setStrikeOut();
  int  prevFont();

  FontAttributes* getAttr() {return &stk[stkX];}    // Returns a temporary pointer to the current attr

  void uninstall();

private:

;
  bool setFont(FontAttributes& attr, LOGFONT& logFont);
  };
