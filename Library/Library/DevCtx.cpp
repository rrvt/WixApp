// Device Context with all the details


#include "pch.h"
#include "DevCtx.h"
#include "MessageBox.h"


DevCtx::DevCtx() : dc(0), suppress(false), scale(1000),     //  savedDC(0),
                   pgWidth(0), rightMgn(0), leftMgn(0),
                   pgHeight(0),  topMgn(0),  botMgn(0),
                   avgFlChWidth(1), avgLgChWidth(1), edgeChWidth(0),
                   chHeight(0), uLineDelta(0) { }


DevCtx::~DevCtx() { }


void DevCtx::wdwDim() {
RECT  winSz;

  dc->GetWindow()->GetClientRect(&winSz);   pgWidth = winSz.right;  pgHeight = winSz.bottom;
  }


int DevCtx::width(TCchar* tc) {
CString cs = tc;
CSize   sz = dc->GetOutputTextExtent(cs);

  return font.isFontItalic() ? sz.cx + 2 : sz.cx;
  }


bool DevCtx::getLogFont(TCchar* face, double sz, LOGFONT& logFont) {
double ppiX = dc->GetDeviceCaps(LOGPIXELSX);
double t    = sz * scale / ppiX;
int    sze  = int(t + 0.5);
CFont  font;

  try {if (!font.CreatePointFont(sze, face, dc)) return false;} catch (...) {return false;}

  font.GetLogFont(&logFont);   return font.DeleteObject();
  }


bool DevCtx::createFont(LOGFONT& logFont, CFont& font)
                                                     {return font.CreatePointFontIndirect(&logFont, dc);}


bool DevCtx::select(CFont& font) {
CFont* f = dc ? dc->SelectObject(&font) : 0;

  return f ?  f->DeleteObject() : false;
  }


void DevCtx::txtOut(TCchar* tc, int hzPos, int vertPos) {
Cstring cs = tc;

  if (suppress) return;

  try {if (!dc->TextOut(hzPos, vertPos, cs)) {outError(cs); return;}}
  catch (...)                                {outError(cs); return;}
  }


void DevCtx::outError(TCchar* stg)
                  {String err = _T("Unable to output: '"); err += stg; err += _T("'");   messageBox(err);}


void DevCtx::line(POINT* points, int& n) {if (!suppress) dc->Polyline(points, n);   n = 0;}


void DevCtx::setAvgLgChWidth() {
TEXTMETRIC metric;
int        buf[26];
int        n;
int        i;
double     sum;

  if (dc->GetTextMetrics(&metric)) {

    dc->GetCharWidth(_T('A'), _T('Z'), buf);

    for (i = 0, n = noElements(buf), sum = 0; i < n; i++) sum += buf[i];

    avgLgChWidth = sum / n;

    avgFlChWidth = metric.tmAveCharWidth;   if (avgFlChWidth > avgLgChWidth) avgLgChWidth = avgFlChWidth;

    if (!edgeChWidth) edgeChWidth = avgLgChWidth;
    }
  }


void DevCtx::setHeight() {
TEXTMETRIC metric;

  if (dc->GetTextMetrics(&metric)) {

    chHeight = metric.tmHeight + metric.tmExternalLeading + 2;

    uLineDelta = chHeight - metric.tmInternalLeading - metric.tmExternalLeading;
    }
  }


void DevCtx::saveDvx(DevCtxBkp& bkp) {

  bkp.suppress     = suppress;
  bkp.font         = font;
  bkp.scale        = scale;

  bkp.pgWidth      = pgWidth;
  bkp.rightMgn     = rightMgn;
  bkp.leftMgn      = leftMgn;

  bkp.pgHeight     = pgHeight;
  bkp.topMgn       = topMgn;
  bkp.botMgn       = botMgn;

  bkp.avgFlChWidth = avgFlChWidth;
  bkp.avgLgChWidth = avgLgChWidth;
  bkp.edgeChWidth  = edgeChWidth;

  bkp.chHeight     = chHeight;
  bkp.uLineDelta   = uLineDelta;
  }


void DevCtx::restoreDvx(DevCtxBkp& bkp) {
  suppress     = bkp.suppress;
  font         = bkp.font;
  scale        = bkp.scale;

  pgWidth      = bkp.pgWidth;
  rightMgn     = bkp.rightMgn;
  leftMgn      = bkp.leftMgn;

  pgHeight     = bkp.pgHeight;
  topMgn       = bkp.topMgn;
  botMgn       = bkp.botMgn;

  avgFlChWidth = bkp.avgFlChWidth;
  avgLgChWidth = bkp.avgLgChWidth;
  edgeChWidth  = bkp.edgeChWidth;

  chHeight     = bkp.chHeight;
  uLineDelta   = bkp.uLineDelta;
  }


void DevCtx::examineCurFont(TCchar* tc) {
LOGFONT logFont;
String  s;

  if (!dc->m_bPrinting) return;

  if (getCurFont(logFont)) {s = tc;   s += _T(":  ");   s += logFont.lfFaceName;   messageBox(s);}
  }


bool DevCtx::getCurFont(LOGFONT& logFont) {
CFont* f = dc->GetCurrentFont();

  return f ? f->GetLogFont(&logFont) : false;
  }


DevCtxBkp::~DevCtxBkp() { }

