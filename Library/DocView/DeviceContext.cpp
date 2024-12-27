// Device Context with all the details


#include "pch.h"
#include "DeviceContext.h"
#include "MessageBox.h"


DeviceContext::DeviceContext() : dc(0), suppress(false) { }


DeviceContext::~DeviceContext() { }


void DeviceContext::clear() {
  dc       = 0;
  suppress = 0;
  fontMgr.clear();
  horz.clear();
  vert.clear();
  }


void DeviceContext::copy(DeviceContext& dvc) {
  dc       = dvc.dc;
  suppress = dvc.suppress;
  fontMgr  = dvc.fontMgr;
  horz     = dvc.horz;
  vert     = dvc.vert;
  }


void DeviceContext::wdwDim() {
RECT  winSz;

  dc->GetWindow()->GetClientRect(&winSz);

  horz.setPgWidth(winSz.right);  vert.setPgHeight(winSz.bottom);
  }


int DeviceContext::width(TCchar* tc) {
CString cs = tc;
CSize   sz = dc->GetOutputTextExtent(cs);

  return fontMgr.isFontItalic() ? sz.cx + 2 : sz.cx;
  }


void DeviceContext::txtOut(TCchar* tc, int hzPos, int vertPos) {
Cstring cs = tc;

  if (suppress) return;

  try {if (!dc->TextOut(hzPos, vertPos, cs)) {outError(cs); return;}}
  catch (...)                                {outError(cs); return;}
  }


void DeviceContext::outError(TCchar* stg)
           {String err = _T("Unable to output: '"); err += stg; err += _T("'");   messageBox(err);}


void DeviceContext::line(POINT* points, int& n) {if (!suppress) dc->Polyline(points, n);   n = 0;}


void DeviceContext::setAvgLgChWidth() {
TEXTMETRIC metric;
int        buf[26];
int        n;
int        i;
double     sum;
double     avgFlChWidth;
double     avgLgChWidth;

  if (dc->GetTextMetrics(&metric)) {

    dc->GetCharWidth(_T('A'), _T('Z'), buf);

    for (i = 0, n = noElements(buf), sum = 0; i < n; i++) sum += buf[i];

    avgLgChWidth = sum / n;

    avgFlChWidth = metric.tmAveCharWidth;

    if (avgFlChWidth > avgLgChWidth) avgLgChWidth = avgFlChWidth;

    if (!horz.charWidth) horz.charWidth = (avgLgChWidth + avgFlChWidth) / 2;
    }
  }


void DeviceContext::setHeight() {
TEXTMETRIC metric;

  if (dc->GetTextMetrics(&metric)) {

    vert.charHeight = metric.tmHeight + metric.tmExternalLeading + 2;

    vert.uLineDelta = vert.charHeight - metric.tmInternalLeading - metric.tmExternalLeading;
    }
  }



void DeviceContext::examineCurFont(TCchar* tc) {
LOGFONT logFont;
String  s;

  if (!dc->m_bPrinting) return;

  if (getCurFont(logFont)) {s = tc;   s += _T(":  ");   s += logFont.lfFaceName;   messageBox(s);}
  }


bool DeviceContext::getCurFont(LOGFONT& logFont) {
CFont* f = dc->GetCurrentFont();

  return f ? f->GetLogFont(&logFont) : false;
  }



///--------------------

//DevCtxBkp::~DevCtxBkp() { }
#if 0
void DeviceContext::saveDvx(DevCtxBkp& bkp) {

  bkp.suppress     = suppress;
  bkp.fontMgr         = fontMgr;
  bkp.scale        = scale;

  bkp.pgWidth      = pgWidth;
  bkp.rightMgn     = rightMgn;
  bkp.leftMgn      = leftMgn;

  bkp.pgHeight     = pgHeight;
  bkp.topMgn       = topMgn;
  bkp.botMgn       = botMgn;

//bkp.avgFlChWidth = avgFlChWidth;
//bkp.avgLgChWidth = avgLgChWidth;
  bkp.charWidth  = charWidth;

  bkp.chHeight     = chHeight;
  bkp.uLineDelta   = uLineDelta;
  }


void DeviceContext::restoreDvx(DevCtxBkp& bkp) {
  suppress     = bkp.suppress;
  fontMgr         = bkp.fontMgr;
  scale        = bkp.scale;

  pgWidth      = bkp.pgWidth;
  rightMgn     = bkp.rightMgn;
  leftMgn      = bkp.leftMgn;

  pgHeight     = bkp.pgHeight;
  topMgn       = bkp.topMgn;
  botMgn       = bkp.botMgn;

//avgFlChWidth = bkp.avgFlChWidth;
//avgLgChWidth = bkp.avgLgChWidth;
  charWidth    = bkp.charWidth;

  chHeight     = bkp.chHeight;
  uLineDelta   = bkp.uLineDelta;
  }
#endif
#if 0
bool DeviceContext::getLogFont(TCchar* face, double sz, LOGFONT& logFont) {
double ppiX = dc->GetDeviceCaps(LOGPIXELSX);
double t    = sz * scale / ppiX;
int    sze  = int(t + 0.5);
CFont  fontMgr;

  try {if (!fontMgr.CreatePointFont(sze, face, dc)) return false;} catch (...) {return false;}

  fontMgr.GetLogFont(&logFont);   return fontMgr.DeleteObject();
  }


bool DeviceContext::createFont(LOGFONT& logFont, CFont& fontMgr)
                                               {return fontMgr.CreatePointFontIndirect(&logFont, dc);}


bool DeviceContext::select(CFont& fontMgr) {
CFont* f = dc ? dc->SelectObject(&fontMgr) : 0;

  return f ?  f->DeleteObject() : false;
  }
#endif
//pgWidth(0),     //leftMgn(0),        rightMgn(0),
//                                 pgHeight(0),    topMgn(0),         botMgn(0),
//                                 chHeight(0),       uLineDelta(0)
//avgFlChWidth(1), avgLgChWidth(1),
