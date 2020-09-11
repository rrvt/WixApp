// Font Manager
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.


#include "stdafx.h"
#include "FontMgr.h"
#include "MessageBox.h"


FontMgr::FontMgr() : stkX(0) {clear();}

FontMgr::~FontMgr() {clear();}


void FontMgr::clear() {

  while (stkX > 0) pop();

  getAttr().clear();   stkX = 0;   //stk.clear();
  }


void FontMgr::initialize(TCchar* face, int fontSize, CDC* dc) {
CFont   font;
CFont*  original;
LOGFONT logFont;

  stkX = 0;   dc->SetMapMode(MM_TEXT);

  if (!font.CreatePointFont(fontSize, face, dc)) {createFailed(_T("initialize")); return;}

  original = dc->SelectObject(&font);    font.GetLogFont(&curLogFont);

  if (original) {
    original->GetLogFont(&logFont);    original->DeleteObject();

    FontAttr& attr = getAttr();
    attr.dc        = dc;
    attr.sz        = logFont.lfHeight;
    attr.bold      = logFont.lfWeight == FW_BOLD;
    attr.italic    = logFont.lfItalic;
    attr.underline = logFont.lfUnderline;
    attr.strikeout = logFont.lfStrikeOut;
    attr.face      = logFont.lfFaceName;
    }

  stkX++;

  FontAttr& attr = getAttr();
  attr.dc   = dc;
  attr.sz   = fontSize;
  attr.face = face;
  }


void FontMgr::setSize(int fontSize)
                          {FontAttr& attr = push();   attr.sz = fontSize;   update(attr, _T("setSize"));}



void FontMgr::setBold()   {FontAttr& attr = push();   attr.bold = true;    update(attr, _T("setBold"));}
void FontMgr::setItalic() {FontAttr& attr = push();   attr.italic = true;  update(attr, _T("setItalic"));}
void FontMgr::setUnderLine()
                    {FontAttr& attr = push();  attr.underline = true; update(attr, _T("setUnderLine"));}
void FontMgr::setStrikeOut()
                    {FontAttr& attr = push();  attr.strikeout = true; update(attr, _T("setStrikeOut"));}


FontAttr& FontMgr::push() {
FontAttr& prev = getAttr();     stkX++;
FontAttr& cur  = getAttr();     cur = prev;  return cur;}


void FontMgr::pop() {

  if (stkX <= 0) return;

  FontAttr& cur  = getAttr();     cur.clear();   --stkX;
  FontAttr& attr = getAttr();     update(attr, _T("pop"));
  }



void FontMgr::update(FontAttr& attr, TCchar* fn) {
CFont   font;
LOGFONT logFont;
CFont   f;

  try {if (!font.CreatePointFont(attr.sz, attr.face, attr.dc)) {createFailed(fn); return;}}
  catch (...) {createFailed(fn); return;}

  font.GetLogFont(&logFont); font.DeleteObject();

                      logFont.lfHeight    = attr.sz;
  if (attr.bold)      logFont.lfWeight    = FW_BOLD;
  if (attr.italic)    logFont.lfItalic    = -1;
  if (attr.underline) logFont.lfUnderline = true;
  if (attr.strikeout) logFont.lfStrikeOut = true;

  if (!f.CreatePointFontIndirect(&logFont, attr.dc)) {createFailed(_T("update")); return; }

  install(attr, f);
  }


void FontMgr::install(FontAttr& attr, CFont& font) {
CFont* x;

  font.GetLogFont(&curLogFont);

  x = attr.dc->SelectObject(&font);   if (x) x->DeleteObject();
  }


void FontMgr::createFailed(TCchar* fn)
                            {String s;   s.format(_T("Create Font failed in %s"), fn);   messageBox(s);}

