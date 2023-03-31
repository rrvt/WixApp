// font Manager II -- Second attempt at managing fonts in Windows!!!


#include "pch.h"
#include "FontMgr.h"
#include "DevCtx.h"


FontMgr::~FontMgr() {stkX = -1; cur = 0;}



void FontMgr::setBase(DevCtx& dvx, TCchar* face, double fontSize)
                        {FontAttr* attr;   stkX = -1;   attr = next();   attr->init(dvx, face, fontSize);}


FontAttr* FontMgr::next() {
FontAttr* next = datum(++stkX);

  if (!next) {NewAlloc(FontAttr);   next = AllocNode;   data += next;}

  if (stkX) *next = *cur;   cur = next;   return next;
  }


void FontMgr::pop() {

  stkX = stkX >= 0 ? stkX - 1 : -1;

  cur = stkX >= 0 ? datum(stkX) : datum(0);

  if (cur) cur->createFont();
  }


void FontAttr::clear() {
  face.clear();
  sz        = 0;
  bold      = false;
  italic    = false;
  underline = false;
  strikeout = false;
  }


void FontAttr::init(DevCtx& devCtx, TCchar* name, double siz) {
  dvx       = &devCtx;
  face      = name;
  sz        = siz;
  bold      = false;
  italic    = false;
  underline = false;
  strikeout = false;   createFont();
  }


void FontAttr::copy(FontAttr& d) {
  dvx       = d.dvx;
  face      = d.face;
  sz        = d.sz;
  bold      = d.bold;
  italic    = d.italic;
  underline = d.underline;
  strikeout = d.strikeout;
  }


bool FontAttr::createFont() {
LOGFONT logFont;
CFont   font;

  if (!dvx->getLogFont(face, sz, logFont)) return false;

  logFont.lfWeight    = bold ? FW_BOLD : FW_NORMAL;
  logFont.lfItalic    = italic ? -1 : 0;
  logFont.lfUnderline = underline;
  logFont.lfStrikeOut = strikeout;

  if (!dvx->createFont(logFont, font)) return false;

  return dvx->select(font);
  }


void FontMgr::copy(FontMgr& fm) {
int       i;
int       n = fm.stkX + 1;
FontAttr* attr;
FontAttr* oldAttr;

  stkX = fm.stkX;

  for (i = 0; i < n; i++) {
    oldAttr = fm.data[i].p;
    if (oldAttr)
      {attr = getAttr(i);   *attr = *oldAttr;}
    }

  cur = stkX >= 0 ? data[stkX].p : 0;
  }

FontAttr* FontMgr::getAttr(int i) {
FontAttr* attr = datum(i);

  if (!attr) {NewAlloc(FontAttr);   attr = AllocNode;   data[i].p = attr;}

  return attr;
  }

