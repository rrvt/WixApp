// Font Manager
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.


#include "stdafx.h"
#include "FontMgr.h"


FontMgr::~FontMgr() {}


void FontMgr::initialize(TCchar* face, int fontSize, CDC* dc) {
CFont          font;
FontAttributes attr;

  if (stkX) uninstall();

  attr.dc   = dc;
  attr.sz   = fontSize;
  attr.face = face;

  stk[++stkX] = attr;

  if (font.CreatePointFont(attr.sz, attr.face, attr.dc)) {
    original = dc->SelectObject(&font);
    }
  }


void FontMgr::setSize(int fontSize) {
FontAttributes  attr = stk[stkX];
CFont           font;

  if (!stkX) return;

  attr.sz = fontSize;

  if (font.CreatePointFont(attr.sz, attr.face, attr.dc))
    {CFont* x = attr.dc->SelectObject(&font); if (x) x->DeleteObject(); stk[++stkX] = attr;}
  }



void FontMgr::setBold() {
FontAttributes  attr = stk[stkX];
CFont           font;
LOGFONT         logFont;

  if (!stkX) return;

  if (font.CreatePointFont(attr.sz, attr.face, attr.dc)) {

    font.GetLogFont(&logFont); font.DeleteObject();

    attr.bold = true;

    if (setFont(attr, logFont)) stk[++stkX] = attr;
    }
  }



void FontMgr::setItalic() {
FontAttributes  attr = stk[stkX];
CFont           font;
LOGFONT         logFont;

  if (!stkX) return;

  if (font.CreatePointFont(attr.sz, attr.face, attr.dc)) {

    font.GetLogFont(&logFont); font.DeleteObject();

    attr.italic = true;

    if (setFont(attr, logFont)) stk[++stkX] = attr;
    }
  }



void FontMgr::setUnderLine() {
FontAttributes  attr = stk[stkX];
CFont           font;
LOGFONT         logFont;

  if (!stkX) return;

  if (font.CreatePointFont(attr.sz, attr.face, attr.dc)) {

    font.GetLogFont(&logFont); font.DeleteObject();

    attr.underline = true;

    if (setFont(attr, logFont)) stk[++stkX] = attr;
    }
  }



void FontMgr::setStrikeOut() {
FontAttributes  attr = stk[stkX];
CFont           font;
LOGFONT         logFont;

  if (!stkX) return;

  if (font.CreatePointFont(attr.sz, attr.face, attr.dc)) {

    font.GetLogFont(&logFont); font.DeleteObject();

    attr.strikeout = true;

    if (setFont(attr, logFont)) stk[++stkX] = attr;
    }
  }





int FontMgr::prevFont() {
FontAttributes attr;
CFont          font;
LOGFONT        logFont;

  if (stkX < 2) return 0;

  attr = stk[--stkX];

  if (font.CreatePointFont(attr.sz, attr.face, attr.dc)) {

    font.GetLogFont(&logFont); font.DeleteObject();

    setFont(attr, logFont);
    }

  return attr.sz;
  }


bool FontMgr::setFont(FontAttributes& attr, LOGFONT& logFont) {
CFont f;

  logFont.lfHeight = attr.sz;
  if (attr.bold)      logFont.lfWeight = FW_BOLD;
  if (attr.italic)    logFont.lfItalic = -1;
  if (attr.underline) logFont.lfUnderline = true;
  if (attr.strikeout) logFont.lfStrikeOut = true;

  if (f.CreatePointFontIndirect(&logFont, attr.dc))
                         {CFont* x = attr.dc->SelectObject(&f); if (x) x->DeleteObject(); return true;}

  return false;
  }




void FontMgr::uninstall() {
FontAttributes attr;

  if (stkX && original) {

    attr = stk[stkX];

    CFont* f = attr.dc->SelectObject(original); if (f) f->DeleteObject();

    original = 0; stkX = 0;
    }
  }

