// Printer Manager -- Interface to Windows


#include "pch.h"
#include "PrinterMgr.h"
#include "PrinterAttr.h"


static TCchar* CourierNew = _T("Courier New");


bool PrinterMgr::prepare(HDC dc) {
uint p;

  if (!dc) return false;

  cdcEx.attach(dc);   cdcEx.setMapMode(MM_TEXT);

  if (prtrDevAttr.orient == PortOrnt) {
    width       = xActualPixels(prtrDevAttr.width.stod(p));
    length      = yActualPixels(prtrDevAttr.length.stod(p));
    topMgn      = yActualPixels(prtrDevAttr.topMgn.stod(p));
    leftMgn     = xActualPixels(prtrDevAttr.leftMgn.stod(p));
    rightMgn    = xActualPixels(prtrDevAttr.rightMgn.stod(p));
    bottomMgn   = yActualPixels(prtrDevAttr.bottomMgn.stod(p));
    }
  else {
    width       = yActualPixels(prtrDevAttr.length.stod(p));
    length      = xActualPixels(prtrDevAttr.width.stod(p));
    topMgn      = xActualPixels(prtrDevAttr.topMgn.stod(p));
    leftMgn     = yActualPixels(prtrDevAttr.leftMgn.stod(p));
    rightMgn    = yActualPixels(prtrDevAttr.rightMgn.stod(p));
    bottomMgn   = xActualPixels(prtrDevAttr.bottomMgn.stod(p));
    }

  setFonts(_T("Arial"), 120);   return true;
  }


// Face and tenths of point

void PrinterMgr::setFonts(TCchar* face, int tenths) {

  hdrCtx.setDefaultFont( face, tenths);

  bodyCtx.setDefaultFont(face, tenths);

  ftrCtx.setDefaultFont( face, tenths);
  }


bool PrinterMgr::startDoc(TCchar* docName)  {
String s;

  hdrCtx.x    = bodyCtx.x    = ftrCtx.x    = leftMgn;
  hdrCtx.xEnd = bodyCtx.xEnd = ftrCtx.xEnd = width - rightMgn;

  ctx = &bodyCtx;   ctx->initializeFont();

  if (suppress || printerDC.startDoc(docName)) return true;

  getError(::GetLastError(), s);   messageBox(s);   return false;
  }


void PrinterMgr::setHdr(TCchar* face, int tenthsPt) {
DeviceCtx* pc = ctx;

  ctx = &hdrCtx;   ctx->setDefaultFont(face, tenthsPt);   setCurCtx(*pc);
  }


void PrinterMgr::startHeader() {setCurCtx(hdrCtx);   ctx->initializeFont();}


void PrinterMgr::endHeader() {
  ctx->clrFonts();   finLine();   rcvPrvCtx();   ctx->y = hdrCtx.y + hdrCtx.deltaY * 125 / 1000;
  }


bool PrinterMgr::startPage() {
String s;

  ctx = &bodyCtx;   updateMetrics();

  ctx->x   = leftMgn;           ctx->xEnd = width  - rightMgn;

  hdrCtx.y = ctx->y = topMgn;   ctx->yEnd = length - bottomMgn - ftrOffset;

  if (suppress || printerDC.startPage()) return true;

  getError(::GetLastError(), s);   messageBox(s);   printerDC.abortDoc();   return false;
  }


bool PrinterMgr::endPage() {return suppress || cdcEx.endPage();}


void PrinterMgr::setFtr(TCchar* face, int tenthsPt) {
DeviceCtx* pc = ctx;

  ctx = &ftrCtx;;   ctx->setDefaultFont(face, tenthsPt);    ctx->initializeFont();

  ctx->yEnd  = length;

    ftrOffset = ctx->deltaY * 1125 / 1000;
    bodyCtx.yEnd      = length - bottomMgn - ftrOffset;

  ctx->clrFonts();   setCurCtx(*pc);
  }


void PrinterMgr::startFooter() {

  setCurCtx(ftrCtx);  ctx->initializeFont();

  ctx->y = length - (bottomMgn + ctx->metrics.tmHeight);
  }


void PrinterMgr::endFooter() {ctx->clrFonts();   rcvPrvCtx();}


void PrinterMgr::updateCoordinates() {ctx->x = leftMgn;   ctx->y = topMgn + hdrOffset;}


// Print the content of the buffer being alert to underlines, end of page, end of line, etc.

bool PrinterMgr::output() {
DeviceBfr& buf = ctx->buf;
int        pos;
int        lastPos;
int        w;

  while (!isBufEmpty()) {
    for (lastPos = -1, pos = buf.findNextBrk(0); pos >= 0;
                                                     lastPos = pos, pos = buf.findNextBrk(pos+1)) {

      if (ctx->x + cdcEx.getExtent(buf(), pos).cx > ctx->xEnd) break;
      }

    if (lastPos < 0) {
      if (ctx->x > leftMgn) {if (!ctx->crlf()) return false;    buf.trimLeft();}
      else ctx->textOut(buf.length());
      continue;
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
bool PrinterMgr::getTextMetrics() {
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


void PrinterMgr::hmmToTxtExt(CSize& sz) {
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
CSize pixelSize = dc.GetTextExtent(myText);

// 3. Get the screen's DPI (Dots Per Inch)
int dpiX = dc.GetDeviceCaps(LOGPIXELSX);
int dpiY = dc.GetDeviceCaps(LOGPIXELSY);

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

//void PrinterMgr::txtExtToXHmm(CSize& ext) {ext.cx = MulDiv(ext.cx, 254, GetDeviceCaps(LOGPIXELSX));}





//////////----------------

#if 0
void PrinterMgr::SetAdobePDFOutputName(String& sOutputFilePath) {
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
void PrinterMgr::hmmToTxtExt(CSize& hmm) {
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
void PrinterMgr::dspState(TCchar* prefix) {
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

//void PrinterMgr::nextBegin() {ctx->underLine.nextBeg(ctx->x, ctx->yUnderLine());}

//  prtrDevAttr.set(pd.hDevMode);   updateDC(dlg);
#if 0
void PrinterMgr::updateDC(CPrintDialogEx& dlg) {
HDC& hdc  = dlg.m_pdex.hDC;

  if (hdc) {::DeleteDC(hdc);   hdc = 0;   hdc = dlg.CreatePrinterDC();}
  }
#endif
#if 0
void PrinterMgr::attach(HDC hdc) {
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
#if 0
//void PrinterMgr::endUnderLine() {if (ctx->buf.doEndUnderLine()) finEndUnderLine();}


/*  // 1. Create a logical pen (e.g., PS_SOLID with a width of 5 pixels)
    CPen pen(PS_SOLID, 5, RGB(255, 0, 0)); // Red, 5 pixels thick

    // 2. Select the pen into the device context and store the old pen
    CPen* pOldPen = pDC->SelectObject(&pen);

    // 3. Define the points for your polyline
    CPoint points[] = { CPoint(50, 50), CPoint(200, 50), CPoint(200, 200), CPoint(50, 200) };

    // 4. Draw the polyline
    pDC->Polyline(points, 4);

    // 5. Restore the original pen
    pDC->SelectObject(pOldPen);     */

void PrinterMgr::doUnderLine(Point* pts) {
uint  black = RGB(0,0,0);
int   dy    = ctx->deltaY;
long& y0    = pts[0].y;
long& y1    = pts[1].y;
int   x     = y0 - y1;

  if (x && abs(x) < dy) {
    if (x > 0) y1 = y0;
    else       y0 = y1;
    }

  drawLine(pts, RGB(0,0,0));
  }


void PrinterMgr::drawLine(Point* pt, uint rgb) {
CPen  pen;
CPen* prevPen;

  if (suppress) return;

  pen.CreatePen(PS_SOLID, 5, rgb);

  prevPen = devDC.selectObj(&pen);

    devDC.polyLine(pt, 2);

  devDC.selectObj(prevPen);
  }
#endif
#if 0
  if (isBodyCtx() && ctx->isEndPage())
                                   {if (isUnderLine()) ctx->buf.setNewPageUL();   return false;}
#endif


// Clean up: CDC::Detach prevents the destructor from  deleting the HDC prematurely if you want to
// manage it // elsewhere, but usually dc.Detach() isn't needed if dc owns the HDC here.
// Important: Manually delete the HDC

//PrinterMgr::~PrinterMgr() {devDC.~CDCex();}

//bool underlineNext = false;

//  if (isUnderLine()) {finEndUnderLine();  underlineNext = true;}


//TCchar* ArialFont  = _T("Arial");
//  printerName = prtrDevAttr.name;

//  prevCtx->cFont = devDC.selectObj(ctx->fonts.getTop());   updateMetrics();
#if 0
void PrinterMgr::setCurCtx(DeviceCtx& pc)
              {prevCtx = ctx;   ctx = &pc;   ctx->x = leftMgn;   ctx->y = topMgn;}


void PrinterMgr::rcvPrvCtx() {ctx = prevCtx;   ctx->x = leftMgn;  ctx->y = topMgn;}
#endif
#if 0
void PrinterMgr::setFtrOffset() {
int offset;

  if (ctx != &ftrCtx) return;

  offset = ctx->deltaY * 1125 / 1000;

  if (offset > bodyCtx.ftrOffset) bodyCtx.ftrOffset = offset;
  }

void PrinterMgr::debug() {
CFont*  cFont = printerDC.getCurrentFont();   if (!cFont) return;
LOGFONT logFont;
String  s;

  cFont->GetLogFont(&logFont);

  s.format(_T("Current Font: %s, %i"), logFont.lfFaceName, logFont.lfHeight);   messageBox(s);
  }
#endif
#if 0
// Advances one line and returns true if another line is allowed in page

bool PrinterMgr::crlf() {

  if (!ctx->flush()) return false;

  drawUnderline();

  ctx->x = leftMgn;   ctx->y += ctx->deltaY;

  if (isUnderlining()) startUnderlining();

  return !ctx->isEndPage();
  }
#endif

