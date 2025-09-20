// font Manager II -- Second attempt at managing fonts in Windows!!!


#include "pch.h"
#include "FontMgr.h"

//#include "MessageBox.h"           /// Debugging



FontMgr::FontMgr() : mumble(this), dc(0), cur(0), scale(0) { }


FontMgr::~FontMgr() {clear();}


void FontMgr::clear() {
  dc = 0;
  data.deallocate(cur);
  data.clear();
  scale = 0;
  }


void FontMgr::setBase(CDC* cdc, TCchar* face, double fontSize)
                                                 {push();   cur->init(dc, face, fontSize * scale);}


void FontAttr::init(CDC* cdc, TCchar* name, double scsz) {
  dc         = cdc;
  face       = name;
  scaledSize = scsz;
  bold       = false;
  italic     = false;
  underline  = false;
  strikeout  = false;
  createFont();
  }


void FontMgr::push()
              {FontAttr* p = data.allocate();   if (cur) {*p = *cur;   data.push(cur);}   cur = p;}


void FontMgr::pop() {data.deallocate(cur);   cur = data.pop();   restore();}


void FontAttr::clear() {
  dc         = 0;
  face.clear();
  scaledSize = 0;
  bold       = false;
  italic     = false;
  underline  = false;
  strikeout  = false;
  }


void FontAttr::copy(FontAttr& d) {
  dc         = d.dc;
  face       = d.face;
  scaledSize = d.scaledSize;
  bold       = d.bold;
  italic     = d.italic;
  underline  = d.underline;
  strikeout  = d.strikeout;
  }


bool FontAttr::createFont() {
LOGFONT logFont;
CFont   font;

  if (!getLogFont(logFont)) return false;

  logFont.lfWeight    = bold ? FW_BOLD : FW_NORMAL;
  logFont.lfItalic    = italic ? -1 : 0;
  logFont.lfUnderline = underline;
  logFont.lfStrikeOut = strikeout;

  if (!createFont(logFont, font)) return false;

  return select(font);
  }


bool FontAttr::getLogFont(LOGFONT& logFont) {
double ppiX = dc->GetDeviceCaps(LOGPIXELSX);
double t    = scaledSize / ppiX;
int    sze  = int(t + 0.5);
CFont  font;

  try {if (!font.CreatePointFont(sze, face, dc)) return false;} catch (...) {return false;}

  font.GetLogFont(&logFont);   return font.DeleteObject();
  }


bool FontAttr::createFont(LOGFONT& logFont, CFont& font)
                                               {return font.CreatePointFontIndirect(&logFont, dc);}


bool FontAttr::select(CFont& font)
           {CFont* f = dc ? dc->SelectObject(&font) : 0;    return f ?  f->DeleteObject() : false;}


void FontMgr::copy(FontMgr& fm) {
  dc    = fm.dc;
  push();
  *cur = *fm.cur;
  data  = fm.data;
  scale = fm.scale;
  }






////////////----------------------

#if 1
#else
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

FontAttr* FontMgr::getAttr(int i) {
FontAttr* attr = datum(i);

#if 1
  attr = &data.getData(i);
#else
  if (!attr) {NewAlloc(FontAttr);   attr = AllocNode;   data[i] = attr;}
#endif

  return attr;
  }

#endif
#if 0
FontAttr* FontMgr::next() {
FontAttr* next = datum(++stkX);

  if (!next) {
#if 1
    next = data.allocate();
#else
    NewAlloc(FontAttr);   next = AllocNode;
#endif
    data += next;
    }

  if (stkX) *next = *cur;   cur = next;   return next;
  }


void FontMgr::pop() {

  stkX = stkX >= 0 ? stkX - 1 : -1;

  cur = stkX >= 0 ? datum(stkX) : datum(0);

  if (cur) cur->createFont();
  }
#endif

