// Printer Manager -- Interface to Windows


#include "pch.h"
#include "DisplayMgr.h"
#include "DisplayAttr.h"


TCchar* ArialFc      = _T("Arial");
TCchar* TimesFc      = _T("Times New Roman");
TCchar* CourierFc    = _T("Courier New");
TCchar* ComicFc      = _T("Comic Sans MS");
TCchar* BlackadderFc = _T("Blackadder ITC");
TCchar* BauhausFc    = _T("Bauhaus 93");


DisplayMgr::~DisplayMgr() {hdrCtx.~DisplayCtx();   bodyCtx.~DisplayCtx();   ftrCtx.~DisplayCtx();}


void DisplayMgr::prepare(CDC* cdc, CRect& r) {
DriverInfo info;
uint       p;

  cdcEx = cdc;   cdcEx.setMapMode(MM_TEXT);                   //cdc->SetMapMode(MM_TEXT);

  info.width = r.Width();   info.length = r.Height();   displayAttr.load(cdc, info);
  lineHeight = charWidth = maxX = maxY = 0;

  width       = xActualPixels(displayAttr.width.stod(p));
  length      = xActualPixels(displayAttr.length.stod(p));
  topMgn      = xActualPixels(displayAttr.topMgn.stod(p));
  leftMgn     = xActualPixels(displayAttr.leftMgn.stod(p));
  rightMgn    = xActualPixels(displayAttr.rightMgn.stod(p));
  bottomMgn   = xActualPixels(displayAttr.bottomMgn.stod(p));

  setFonts(_T("Arial"), 120);
  }


// Face and tenths of point

void DisplayMgr::setFonts(TCchar* face, int tenths) {

  hdrCtx.setDefaultFont( face, tenths);

  bodyCtx.setDefaultFont(face, tenths);

  ftrCtx.setDefaultFont( face, tenths);
  }


bool DisplayMgr::startDoc(TCchar* docName) {

  hdrCtx.x    = bodyCtx.x    = ftrCtx.x    = leftMgn;
  hdrCtx.xEnd = bodyCtx.xEnd = ftrCtx.xEnd = width - rightMgn;

  ctx = &bodyCtx;   ctx->initializeFont();   ctx->y = topMgn;

  return true;
  }


void DisplayMgr::setHdr(TCchar* face, int tenthsPt) {
DeviceCtx* pc = ctx;

  ctx = &hdrCtx;   ctx->setDefaultFont(face, tenthsPt);   setCurCtx(*pc);
  }


void DisplayMgr::startHeader() {
  setCurCtx(hdrCtx);   ctx->initializeFont();

  ctx->x = leftMgn;   ctx->y = bodyCtx.y;
  }


void DisplayMgr::endHeader() {
  finLine();
  ctx->clrFonts();
  rcvPrvCtx();
  bodyCtx.y = hdrCtx.y;
  }


bool DisplayMgr::startPage() {

  ctx = &bodyCtx;   updateMetrics();

  ctx->x = leftMgn;   ctx->xEnd = width  - rightMgn;

  ctx->y += hdrCtx.deltaY * 125 / 1000;   return true;
  }


bool DisplayMgr::endPage() {finLine();   return true;}


void DisplayMgr::setFtr(TCchar* face, int tenthsPt) {
DeviceCtx* pc = ctx;

  ctx = &ftrCtx;   ctx->setDefaultFont(face, tenthsPt);    ctx->initializeFont();

  ftrOffset = ctx->deltaY * 125 / 1000;

  ctx->clrFonts();   setCurCtx(*pc);
  }


void DisplayMgr::startFooter() {

  setCurCtx(ftrCtx);   ctx->initializeFont();

  ctx->x = leftMgn;   ctx->y = bodyCtx.y + ftrOffset;
  }


void DisplayMgr::endFooter() {
  ctx->clrFonts();   finLine();   rcvPrvCtx();

  ctx->y = ftrCtx.y + ftrOffset;
  }


// Print the content of the buffer being alert to underlines, end of page, end of line, etc.

bool DisplayMgr::output() {
DeviceBfr& buf = ctx->buf;
int        pos;
int        lastPos;
int        w;

  while(!isBufEmpty()) {

    for (lastPos = -1, pos = buf.findNextBrk(0); pos >= 0;
                                                     lastPos = pos, pos = buf.findNextBrk(pos+1)) {

      if (wrap && ctx->x + cdcEx.getExtent(buf(), pos).cx > ctx->xEnd) break;
      }

    if (lastPos < 0) {
      if (ctx->x > leftMgn) {ctx->crlf();   buf.trimLeft();   continue;}

      lastPos = buf.length();
      }

    w = buf.getExtent(lastPos);

    if      (ctx->center) {ctx->x = (ctx->xEnd + leftMgn - w) / 2;   ctx->center = false;}
    else if (ctx->right)  {ctx->x =  ctx->xEnd - w;                  ctx->right  = false;}

    ctx->textOut(lastPos);   return true;
    }

  return true;
  }




/////////------------------

#if 0
bool DisplayMgr::getTextMetrics() {
bool rslt = GetOutputTextMetrics(&metrics);

  return rslt;
  }
#endif

#if 0
CSize PixelsToHundredthsOfMm(CDC* pDC, CSize sizePixels)
{
    // 100 hundredths of a mm = 1 mm
    const int HUNDREDTHS_PER_MM = 100;

    // Get physical width and height in pixels
    int nPixelsX = pDC->GetDeviceCaps(HORZRES);
    int nPixelsY = pDC->GetDeviceCaps(VERTRES);

    // Get physical width and height in millimeters
    int nMmX = pDC->GetDeviceCaps(HORZSIZE);
    int nMmY = pDC->GetDeviceCaps(VERTSIZE);

    // Prevent division by zero
    if (nMmX == 0 || nMmY == 0) return CSize(0, 0);

    // Calculate hundredths of a mm per pixel (using MulDiv to avoid overflow)
    int nHundredthsPerPixelX = ::MulDiv(nMmX * HUNDREDTHS_PER_MM, 1, nPixelsX);
    int nHundredthsPerPixelY = ::MulDiv(nMmY * HUNDREDTHS_PER_MM, 1, nPixelsY);

    // Convert pixels to hundredths of a mm
    return CSize(sizePixels.cx * nHundredthsPerPixelX,
                 sizePixels.cy * nHundredthsPerPixelY);
}


void DisplayMgr::hmmToTxtExt(CSize& sz) {
String s;
// 100 hundredths of a mm = 1 mm
const int HUNDREDTHS_PER_MM = 100;

// Get physical width and height in pixels
int nPixelsX = GetDeviceCaps(HORZRES);
int nPixelsY = GetDeviceCaps(VERTRES);

// Get physical width and height in millimeters
int nMmX = GetDeviceCaps(HORZSIZE);          s.format(_T("nMmX = %i"), nMmX);   messageBox(s);
int nMmY = GetDeviceCaps(VERTSIZE);

// Prevent division by zero
  if (nMmX == 0 || nMmY == 0) {return;}

// Calculate hundredths of a mm per pixel (using MulDiv to avoid overflow)
int nHundredthsPerPixelX = ::MulDiv(nMmX * HUNDREDTHS_PER_MM, 1, nPixelsX);
int nHundredthsPerPixelY = ::MulDiv(nMmY * HUNDREDTHS_PER_MM, 1, nPixelsY);

  sz.cx *= nHundredthsPerPixelX;  sz.cy *= nHundredthsPerPixelY;
  }
#endif


#if 0
// 1. Get the screen's Device Context (or your view's DC)
CClientDC dc(this);

// 2. Measure the text in logical pixels
CString myText = _T("Hello World");
CSize pixelSize = dc->GetTextExtent(myText);

// 3. Get the screen's DPI (Dots Per Inch)
int dpiX = dc->GetDeviceCaps(LOGPIXELSX);
int dpiY = dc->GetDeviceCaps(LOGPIXELSY);

// 4. Calculate millimeters
// Using 25.4f since 1 inch is exactly 25.4 mm
float widthMM = (pixelSize.cx * 25.4f) / dpiX;
float heightMM = (pixelSize.cy * 25.4f) / dpiY;

// Optional: round to 2 decimal places if needed
-----------
// Assuming 'pDC' is your valid pointer to CDC
CSize GetTextSizeInMM(CDC* pDC, const CString& strText)
{
    // 1. Get the bounding box in pixels
    CSize pixelSize = pDC->GetTextExtent(strText);

    // 2. Get the device DPI (Dots Per Inch)
    int dpiX = pDC->GetDeviceCaps(LOGPIXELSX);
    int dpiY = pDC->GetDeviceCaps(LOGPIXELSY);

    // 3. Convert pixels to millimeters: mm = (pixels * 25.4) / DPI
    int widthMM = MulDiv(pixelSize.cx, 25.4, dpiX);
    int heightMM = MulDiv(pixelSize.cy, 25.4, dpiY);

    return CSize(widthMM, heightMM);
}



#endif

//void DisplayMgr::txtExtToXHmm(CSize& ext) {ext.cx = MulDiv(ext.cx, 254, GetDeviceCaps(LOGPIXELSX));}





//////////----------------

#if 0
void DisplayMgr::SetAdobePDFOutputName(String& sOutputFilePath) {
HKEY    hKey;
LPCTSTR pszSubKey = _T("Software\\Adobe\\Acrobat Distiller\\PrinterJobControl");
TCHAR   szExePath[MAX_PATH];
TCchar* oName = sOutputFilePath;
uint    lng   = (sOutputFilePath.length() + 1) * sizeof(TCchar);

  if (RegCreateKeyEx(HKEY_CURRENT_USER, pszSubKey, 0, NULL,
               REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, NULL) != ERROR_SUCCESS) return;

  GetModuleFileName(NULL, szExePath, MAX_PATH);         // Get current app path

  // Set the value: [AppPath] = [TargetPDFPath]        (const BYTE*) (LPCTSTR)
  RegSetValueEx(hKey, szExePath, 0, REG_SZ,  (Byte*) oName, lng);

  RegCloseKey(hKey);
  }
#endif
#if 0
void DisplayMgr::hmmToTxtExt(CSize& hmm) {
  hmm.cx = xHmmToTxtExt(hmm.cx);
  hmm.cy = yHmmToTxtExt(hmm.cy);
  }
#endif

#if 0

  ctx->polyULine = true;
  ctx->begULine.x = ctx->x;     ctx->begULine.y = ctx->y + ctx->deltaUL;
#endif
#if 0

  if (ctx->polyULine) ctx->doUnderLine = true;
#endif
#if 0
CPen  pen;
CPen* prevPen;
  pen.CreatePen(PS_SOLID, 5, RGB(0,0,0));

  prevPen = SelectObject(&pen);

    Polyline(pts, 2);

  SelectObject(prevPen);
#endif
#if 0
void DisplayMgr::dspState(TCchar* prefix) {
bool bf   = !ctx->buf.isEmpty();
bool epg  = ctx->isEndPage();
bool unl  = !ctx->underLine.isEmpty();

bool b = epg | unl;

if (!b) return;

String s;
  s.format(_T("%s: buf not empty: %i, endPage: %i, underLine: %i"), prefix, bf, epg, unl);
  messageBox(s);
  }
#endif
#if 0
  if (ctx == &bodyCtx && ctx->buf.newPageUL) {beginUnderLine();   ctx->buf.newPageUL = false;}

  TextOut(ctx->x, ctx->y, buf(), lastPos);   ctx->x += w;   buf = buf.substr(lastPos);

  if (ctx->deferUL && buf.isEmpty()) {finEndUnderLine();   ctx->deferUL = false;}
#endif
//if (this == &prtrMgr->bodyCtx) dspMetrics();

//void DisplayMgr::nextBegin() {ctx->underLine.nextBeg(ctx->x, ctx->yUnderLine());}

//  displayAttr.set(pd.hDevMode);   updateDC(dlg);
#if 0
void DisplayMgr::updateDC(CPrintDialogEx& dlg) {
HDC& hdc  = dlg.m_pdex.hDC;

  if (hdc) {::DeleteDC(hdc);   hdc = 0;   hdc = dlg.CreatePrinterDC();}
  }
#endif
#if 0
void DisplayMgr::attach(HDC hdc) {
String s;

  if (!hdc) return;

  memset(&docInfo, 0, sizeof(DOCINFO));   docInfo.cbSize = sizeof(DOCINFO);

  m_bPrinting = TRUE;

  if (!Attach(hdc)) {                              // 3. Wrap it in a CDC object
    getError(::GetLastError(), s);   messageBox(s);
    }

  initialized = true; // IsPrinting() returns m_bPrinting;
  }
#endif

//TCchar* ArialFont  = _T("Arial");
#if 0
int DisplayMgr::xInToTxtExt(double xIn)  {
double horzPixl = devDC()->GetDeviceCaps(HORZRES);
double horzInch = ((double)devDC()->GetDeviceCaps(HORZSIZE)) / 25.4;

  return int((xIn * horzPixl / horzInch) + 0.5);
  }


int DisplayMgr::yInToTxtExt(double yIn)  {
double vertPixl = devDC()->GetDeviceCaps(VERTRES);
double vertInch = ((double)devDC()->GetDeviceCaps(VERTSIZE)) / 25.4;

  return int((yIn * vertPixl / vertInch) + 0.5);
  }
#endif
#if 0
//void DisplayMgr::endUnderLine() {if (ctx->buf.doEndUnderLine()) finEndUnderLine();}


void DisplayMgr::doUnderLine(Point* pts) {
uint  black = RGB(0,0,0);
#if 0
int   dy    = ctx->deltaY;
long& y0    = pts[0].y;
long& y1    = pts[1].y;
int   x     = y0 - y1;

  if (x && abs(x) < dy) {
    if (x > 0) y1 = y0;
    else       y0 = y1;
    }
#endif

  drawLine(pts, RGB(0,0,0));
  }

void DisplayMgr::drawLine(Point* pt, uint rgb) {
CPen  pen;
CPen* prevPen;

  pen.CreatePen(PS_SOLID, 5, rgb);

  prevPen = devDC.selectObj(&pen);

    devDC.polyLine(pt, 2);

  devDC.selectObj(prevPen);
  }
#endif
#if 0
void DisplayMgr::setFtrOffset() {
int offset;

  if (ctx != &ftrCtx) return;

  offset

  if (offset > bodyCtx.ftrOffset)  = offset;
  }
#endif
#if 0
void DisplayMgr::setCurCtx(DeviceCtx& pc)
                {prevCtx = ctx;   ctx = &pc;   ctx->x = leftMgn;  ctx->y = topMgn;}


void DisplayMgr::rcvPrvCtx() {ctx = prevCtx;   ctx->x = leftMgn;  ctx->y = topMgn;}
#endif
#if 0
bool DisplayMgr::examineFont() {
Point  line[2];                                            //  tmDescent;
Point& pt0    = line[0];                                      //  tmInternalLeading;
Point& pt1    = line[1];
int    x0     = ctx->x;                                      //  tmExternalLeading;
int    x1     = x0 + ctx->avgCharWidth;                //  tmHeight;
int    x2     = x1 + ctx->avgCharWidth;
int    x3     = x2 + ctx->avgCharWidth;
int    x4     = x3 + ctx->avgCharWidth;
int    x5     = x4 + ctx->avgCharWidth;
int    x6     = x5 + ctx->avgCharWidth;
int    y      = ctx->y;                                        //  tmAscent;
uint   blue   = RGB(  0,   0, 255);
uint   green  = RGB(  0, 255,   0);
uint   red    = RGB(255,   0,   0);
uint   magenta= RGB(255,   0, 255);
uint   yellow = RGB(255, 255,   0);
uint   black  = RGB(  0,   0,   0);
uint   cyan   = RGB(  0, 255, 255);

  pt0.x = x0;      pt1.x = x1;

  pt0.y = pt1.y = y;                                  //  drawLine(line, black);
                                                      //
  pt0.y = pt1.y = y + ctx->metrics.tmAscent;          //  drawLine(line, green);
  pt0.y = pt1.y = y + ctx->metrics.tmDescent;         //  drawLine(line, red);
  pt0.y = pt1.y = y + ctx->metrics.tmHeight;          //  drawLine(line, black);

  pt0.x = x1;   pt1.x = x2;

  pt0.y = pt1.y = y + ctx->metrics.tmAscent + ctx->metrics.tmDescent; // drawLine(line, black);
  pt0.y = pt1.y = y + ctx->metrics.tmInternalLeading;                 // drawLine(line, magenta);
  pt0.y = pt1.y = y + ctx->metrics.tmExternalLeading;                 // drawLine(line, yellow);

  ctx->x += x2;   return ctx->x > ctx->xEnd ? crlf() : true;
  }
#endif
#if 0
void DisplayMgr::debug() {
CFont*  cFont = displayDC.getCurrentFont();   if (!cFont) return;
LOGFONT logFont;
String  s;

  cFont->GetLogFont(&logFont);

  s.format(_T("Current Font: %s, %i"), logFont.lfFaceName, logFont.lfHeight);   messageBox(s);
  }
#endif
/*ctx->setDefaultFont(BauhausFc, 100);*/
#if 0
// Advances one line

bool DisplayMgr::crlf() {

  drawUnderline();

  ctx->x = leftMgn;   setMaxY(ctx->y += ctx->deltaY);

  if (isUnderlining()) startUnderlining();   return true;
  }
#endif

