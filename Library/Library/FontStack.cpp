// DisplayFonts


#include "pch.h"
#include "FontStack.h"
#include "CDCex.h"


void FontStack::setDefault(TCchar* face, int tenthsPt) {
  if (face)     defaultFont.face      = face;
  if (tenthsPt) defaultFont.tenthsPt  = tenthsPt;
  }


void FontStack::initialize() {popAll();   top = data.allocate();   *top = defaultFont;   create();}


void FontStack::push()
                {FontItem* p = data.allocate();   p->getItem(top);   data.push(top);   top = p;}


void FontStack::popAll() {while (top) pop();}


void FontStack::pop() {

  if (top->oldFont)
                 try {top->oldFont = cdcEx.selectObj(top->oldFont);} catch(...) {top->oldFont = 0;}

  data.deallocate(top);   top = data.pop();
  }


void FontItem::set(TCchar* face, int tenthsPt) {this->face = face; this->tenthsPt = tenthsPt;}


bool FontItem::create(CDCex& cdcEx) {
int  weight = bold ? FW_BOLD : FW_NORMAL;
bool rslt = false;

  if (!cdcEx() || face.isEmpty() || tenthsPt < 20) return false;

  try {
    rslt = CreateFont(cdcEx.scaleYY(tenthsPt),          // height
                      0,                                // width
                      0,                                // escapement
                      0,                                // orientation
                      weight,                           // weight
                      italic,                           // italic
                      underline,                        // underline
                      strikeout,                        // strikeout
                      DEFAULT_CHARSET,                  // char set
                      OUT_CHARACTER_PRECIS,             // output precision
                      CLIP_CHARACTER_PRECIS,            // clip precision
                      DEFAULT_QUALITY,                  // quality
                      DEFAULT_PITCH | FF_DONTCARE,      // pitch and family
                      face);                            // Face
    } catch (...) {
      messageBox(_T("CreateFont Crashed"));
      }

  if (oldFont) messageBox(_T("Not Good"));

  if (rslt) oldFont = cdcEx.selectObj(this);   return rslt;
  }


FontItem::~FontItem()
              {try {if (oldFont) oldFont->DeleteObject();} catch (...) {oldFont = 0;} oldFont = 0;}



void FontItem::FontItem::copy(FontItem& f) {oldFont = f.oldFont;   getItem(&f);}


void FontItem::getItem(FontItem* f) {
  if (!f) return;
  face      = f->face;
  tenthsPt  = f->tenthsPt;
  bold      = f->bold;
  italic    = f->italic;
  underline = f->underline;
  strikeout = f->strikeout;
  }




////////----------------

#if 0
void FontStack::msg(TCchar* prefix) {
CDC*    dc = curDC();            if (!dc) return;
CFont*  f  = dc->GetCurrentFont();   if (!f)  return;
LOGFONT lf;
String  s;

  f->GetLogFont(&lf);   s.format(_T("%s: %s"), prefix, lf.lfFaceName);   messageBox(s);
  }
#endif

#if 0
void FontStack::msg(TCchar* prefix, FontItem* item) {
String s;

  if (item) s.format(_T("%s: %s, %i"), prefix, item->face.str(), item->tenthsPt);
  else      s.format(_T("%s: zero item"), prefix);

  messageBox(s);
  }
#endif
#if 0
int  logPxlPrIn;
int  nFontHeight;
bool rslt;

  cdc         = dc;    if (!cdc || !face || !*face || tenthsPt < 20) return false;
  logPxlPrIn  = cdc->GetDeviceCaps(LOGPIXELSY);
  nFontHeight = -MulDiv(tenthsPt, logPxlPrIn, 720);   // point size scaled up from 72 px/in

  rslt = CreateFont(nFontHeight,                      // height
                    0,                                // width
                    0,                                // escapement
                    0,                                // orientation
                    bold ? FW_BOLD : FW_NORMAL,       // weight
                    italic,                           // italic
                    underline,                        // underline
                    strikeout,                        // strikeout
                    DEFAULT_CHARSET,                  // char set
                    OUT_CHARACTER_PRECIS,             // output precision
                    CLIP_CHARACTER_PRECIS,            // clip precision
                    DEFAULT_QUALITY,                  // quality
                    DEFAULT_PITCH | FF_DONTCARE,      // pitch and family
                    face);                            // Face

  if (rslt) oldFont = cdc->SelectObject(this);   return rslt;
#endif
#if 0
void CMyView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
    int nPointSize = 12; // Desired size in points

    // 1. Get printer vertical resolution
    int nDpiY = pDC->GetDeviceCaps(LOGPIXELSY);

    // 2. Calculate logical height for the font
    // MulDiv is a safe way to handle the PointSize * DPI / 72 math
    int nFontHeight = -MulDiv(nPointSize, nDpiY, 72);

    // 3. Create the font
    CFont printerFont;
    printerFont.CreateFont(
        nFontHeight, 0, 0, 0, FW_NORMAL,
        FALSE, FALSE, 0, DEFAULT_CHARSET,
        OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
        _T("Arial")
    );

    // 4. Select into DC and print
    CFont* pOldFont = pDC->SelectObject(&printerFont);
    pDC->TextOut(100, 100, _T("This is scaled for the printer."));

    // Clean up
    pDC->SelectObject(pOldFont);
    printerFont.DeleteObject();
}
#endif
//  int  logPxlPrIn;
//  logPxlPrIn  = curDC()->GetDeviceCaps(LOGPIXELSY);
  //int  nFontHeight;
//  nFontHeight = MulDiv(tenthsPt, logPxlPrIn, 720);   // point size scaled up from 72 px/in
// x = curDC.scaleYY(tenthsPt);

//  if (nFontHeight != x) messageBox(_T("mumble"));


//bool FontStack::create(TCchar* face, int tenthsPt) {push();  return top->create(face, tenthsPt);}

