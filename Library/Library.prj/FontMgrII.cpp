// font Manager II -- Second attempt at managing fonts in Windows!!!


#include "stdafx.h"
#include "FontMgrII.h"



void FontMgrII::set(TCchar* face, double fontSize, CDC* dc) {
FontAttr* attr;

  if (!dc) return;

  dc->SetMapMode(MM_TEXT);

  attr = next();   attr->face = face;   attr->sz = fontSize;   attr->dc = dc;   createFont(*attr);
  }


void FontMgrII::setFace(TCchar* face)
                               {FontAttr* attr = next();   attr->face = face;       createFont(*attr);}

void FontMgrII::setSize(double  size)
                               {FontAttr* attr = next();   attr->sz = size;         createFont(*attr);}

void FontMgrII::setBold()      {FontAttr* attr = next();   attr->bold = true;       createFont(*attr);}

void FontMgrII::setItalic()    {FontAttr* attr = next();   attr->italic = true;     createFont(*attr);}

void FontMgrII::setUnderLine() {FontAttr* attr = next();   attr->underline = true;  createFont(*attr);}

void FontMgrII::setStrikeOut() {FontAttr* attr = next();   attr->strikeout = true;  createFont(*attr);}



FontAttr* FontMgrII::next() {
          get(stkX+1);                              // touch next entry
FontAttr* cur  = top();                             // Get current top
FontAttr* next = datum(++stkX);                     // Bump Stack return ptr to top

  *next = *cur;   return next;                      // copy current attr to next
  }


void FontMgrII::pop()
  {stkX = stkX > 0 ? stkX - 1 : 0;    if (stkX > 0) createFont(*top());}



bool FontMgrII::createFont(FontAttr& attr) {
CDC*    dc = attr.dc;
CFont   font;
LOGFONT logFont;
CFont   f;

  if (!dc || !createPointFont(dc, attr.face, attr.sz, font)) return false;

  font.GetLogFont(&logFont);   font.DeleteObject();

  logFont.lfWeight    = attr.bold ? FW_BOLD : FW_NORMAL;
  logFont.lfItalic    = attr.italic ? -1 : 0;
  logFont.lfUnderline = attr.underline;
  logFont.lfStrikeOut = attr.strikeout;

  if (!f.CreatePointFontIndirect(&logFont, dc)) return false;

  return select(f, dc);
  }


bool FontMgrII::createPointFont(CDC* dc, TCchar* face, double size, CFont& font) {
double ppiX = dc->GetDeviceCaps(LOGPIXELSX);
double t    = size * scale / ppiX;
int    sz   = int(t + 0.5);

  try {if (!font.CreatePointFont(sz, face, dc)) return false;} catch (...) {return false;}

  return true;
  }




bool FontMgrII::select(CFont& font, CDC* dc) {
CFont* f;
BOOL   rslt = false;

  f = dc ? dc->SelectObject(&font) : 0;   if (f) rslt = f->DeleteObject();   return rslt;
  }


FontMgrII::~FontMgrII() {
int n = stk.end();
int i;

  for (i = 0; i < n; i++) {
    FontAttr* p = stk[i].p;

    if (p) {NewAlloc(FontAttr);  FreeNode(p);   p = 0;}
    }
  }



void FontAttr::copy(FontAttr& d) {
  dc        = d.dc;
  face      = d.face;
  sz        = d.sz;
  bold      = d.bold;
  italic    = d.italic;
  underline = d.underline;
  strikeout = d.strikeout;
  }


void FontAttr::clear() {
  dc        = 0;
  face.clear();
  sz        = 0;
  bold      = false;
  italic    = false;
  underline = false;
  strikeout = false;
  }


FontAttr* FontAttrP::get() {if (!p) {NewAlloc(FontAttr);   p = AllocNode;}   return p;}



