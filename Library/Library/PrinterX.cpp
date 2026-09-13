// Global Printer


#include "pch.h"
#include "PrinterX.h"
#include "IniFileEx.h"
//#include "MessageBox.h"
#include "winspool.h"


       TCchar* PortraitKey    = _T("Portrait");
       TCchar* LandscapeKey   = _T("Landscape");

static TCchar* GlobalSection  = _T("Global");
static TCchar* CurPrinterKey  = _T("CurrentPrinter");
static TCchar* PaperOrientKey = _T("Orientation");
static TCchar* PaperSizeKey   = _T("PaperSize");
static TCchar* PaperWidthKey  = _T("Width");
static TCchar* PaperLengthKey = _T("Length");
static TCchar* PlexKey        = _T("Plex");
static TCchar* CopiesKey      = _T("Copies");
static TCchar* CollateKey     = _T("Collate");
static TCchar* TopMarginKey   = _T("TopMargin");
static TCchar* BotMarginKey   = _T("BotMargin");
static TCchar* LeftOddKey     = _T("LeftOdd");
static TCchar* RightOddKey    = _T("RightOdd");
static TCchar* LeftEvenKey    = _T("LeftEven");
static TCchar* RightEvenKey   = _T("RightEven");
static TCchar* ScaleKey       = _T("Scale");

Printer printer;


bool PrinterX::readDevAttr(TCchar* name) {
bool   dirty = false;

  if (!name || !*name) return false;

  this->name = name;

  dirty |= readItem(PaperOrientKey, (int&)orient,    PortOrient);
  dirty |= readItem(PaperSizeKey,   (int&)paperSize, LetterPSz);
  dirty |= readItem(PaperWidthKey,        width,     0);
  dirty |= readItem(PaperLengthKey,       length,    0);
  dirty |= readItem(CopiesKey,            copies,    1);
  dirty |= readItem(CollateKey,           collate,   1);
  dirty |= readItem(PlexKey,        (int&)pagePlex,  SimPlex);

  return dirty;
  }


bool PrinterX::readItem(TCchar* key, int& val, int dflt) {
int v = iniFile.readInt(name, key, dflt);

  if (v != val) {val = v; return true;}

  return false;
  }


void PrinterX::readPageAttr(TCchar* name) {

  topMargin = iniFile.read(name, TopMarginKey,   3.0);
  botMargin = iniFile.read(name, BotMarginKey,   3.0);
  leftOdd   = iniFile.read(name, LeftOddKey,     3.0);
  rightOdd  = iniFile.read(name, RightOddKey,    3.0);
  leftEven  = iniFile.read(name, LeftEvenKey,    3.0);
  rightEven = iniFile.read(name, RightEvenKey,   3.0);
  scale     = iniFile.read(name, ScaleKey,    7175.0);
  }


void PrinterX::getDevMode(HANDLE hdl) {

  devMode = (DEVMODE*) GlobalLock(hdl);           // Protect memory handle with ::GlobalLock

    getDevMode();

  GlobalUnlock(devMode);
  }


void PrinterX::getDevMode() {
  getDevName();
  if (devMode->dmFields & DM_ORIENTATION)  orient    = (PrtrOrient) devMode->dmOrientation;
  if (devMode->dmFields & DM_PAPERSIZE)    paperSize = (PaperSize)  devMode->dmPaperSize;
  if (paperSize == UserDefinedPSz) {       width     =              devMode->dmPaperWidth;
                                           length    =              devMode->dmPaperLength;}
  if (devMode->dmFields & DM_COPIES)       copies    =              devMode->dmCopies;
  if (devMode->dmFields & DM_COLLATE)      collate   =              devMode->dmCollate;
  if (devMode->dmFields & DM_DUPLEX)       pagePlex  = (PagePlex)   devMode->dmDuplex;
  }


bool PrinterX::setDevMode(HANDLE hdl) {
bool     rslt = false;

  devMode = (DEVMODE*) GlobalLock(hdl);           // Protect memory handle with ::GlobalLock

    rslt = setDevMode();

  GlobalUnlock(devMode);   return rslt;
  }


bool PrinterX::setDevMode() {
ulong& fields = devMode->dmFields;
bool   rslt   = devMode->dmOrientation != orient    ||
                devMode->dmPaperSize   != paperSize ||
                devMode->dmPaperWidth  != width     ||
                devMode->dmPaperLength != length    ||
                devMode->dmCopies      != copies    ||
                devMode->dmCollate     != collate   ||
                devMode->dmDuplex      != pagePlex;

  setDevName();
  fields |= DM_ORIENTATION; devMode->dmOrientation = orient;
  setPaperSize();
  fields |= DM_COPIES;      devMode->dmCopies      = copies;
  fields |= DM_COLLATE;     devMode->dmCollate     = collate;
  fields |= DM_DUPLEX;      devMode->dmDuplex      = pagePlex;
  return rslt;
  }


void PrinterX::setDevName() {
int    i;
int    n = CCHDEVICENAME - 1;
Tchar  ch;

  for (i = 0, ch = name[i]; ch && i < n; ch = name[++i]) devMode->dmDeviceName[i] = ch;

  devMode->dmDeviceName[i] = 0;
  }


void PrinterX::getDevName() {
Tchar* data = (Tchar*)&devMode->dmDeviceName;
int    n    = CCHDEVICENAME;
int    i;
Tchar  ch;

  name.clear();

  for (i = 0, ch = data[i]; ch && i < n; ch = data[++i]) name += ch;
  }


void PrinterX::setPaperSize() {

  if (!devMode) return;

  devMode->dmFields |= DM_PAPERSIZE; devMode->dmPaperSize   = paperSize;

  if (paperSize == UserDefinedPSz) {
    devMode->dmFields |= DM_PAPERWIDTH | DM_PAPERLENGTH;
    devMode->dmPaperWidth  = width;   devMode->dmPaperLength = length;
    }
  }


void PrinterX::set(int width, int length) {
  this->width = short(width * 254);   this->length = short(length * 254);
  paperSize = UserDefinedPSz;
  }


PrtrOrient PrinterX::toOrient(Cstring& cs)
                             {String s = cs;   return s == LandscapeKey ? LandOrient : PortOrient;}


TCchar* PrinterX::toStg(PrtrOrient orient)
                                        {return orient == PortOrient ? PortraitKey : LandscapeKey;}


void PrinterX::load(TCchar* PrinterX) {
//String s;
//    uint   x;

  if (loaded) return;

  if (PrinterX) name = PrinterX;

  if (name.isEmpty()) {
    iniFile.read(GlobalSection, CurPrinterKey, name);

    if (name.isEmpty()) {
      Tchar buf[1024];
      DWORD nBuf = noElements(buf);

      if (!GetDefaultPrinter((LPWSTR) buf, &nBuf)) return;

      name = buf;
      }
    }

  orient    = (PrtrOrient) iniFile.read(name, PaperOrientKey, PortOrient);
  paperSize = (PaperSize)  iniFile.read(name, PaperSizeKey,   LetterPSz);
  width     = (short)      iniFile.read(name, PaperWidthKey,  0);
  length    =              iniFile.read(name, PaperLengthKey, 0);
  copies    =              iniFile.read(name, CopiesKey,      1);
  collate   =              iniFile.read(name, CollateKey,     1);
  pagePlex  = (PagePlex)   iniFile.read(name, PlexKey,        SimPlex);
  topMargin =              iniFile.read(name, TopMarginKey,   3.0);
  botMargin =              iniFile.read(name, BotMarginKey,   3.0);
  leftOdd   =              iniFile.read(name, LeftOddKey,     3.0);
  rightOdd  =              iniFile.read(name, RightOddKey,    3.0);
  leftEven  =              iniFile.read(name, LeftEvenKey,    3.0);
  rightEven =              iniFile.read(name, RightEvenKey,   3.0);
  scale     =              iniFile.read(name, ScaleKey,    7175.0);

  loaded = true;
  }


void PrinterX::save() {
String s;

  if (name.isEmpty()) return;

  iniFile.write(name, PaperOrientKey, orient);
  iniFile.write(name, PaperSizeKey,   paperSize);
  iniFile.write(name, PaperWidthKey,  width);
  iniFile.write(name, PaperLengthKey, length);
  iniFile.write(name, PlexKey,        pagePlex);
  iniFile.write(name, CopiesKey,      copies);
  iniFile.write(name, CollateKey,     collate);
  iniFile.write(name, TopMarginKey,   topMargin);
  iniFile.write(name, BotMarginKey,   botMargin);
  iniFile.write(name, LeftOddKey,     leftOdd);
  iniFile.write(name, RightOddKey,    rightOdd);
  iniFile.write(name, LeftEvenKey,    leftEven);
  iniFile.write(name, RightEvenKey,   rightEven);
  iniFile.write(name, ScaleKey,       scale);

  iniFile.write(GlobalSection, CurPrinterKey, name);
  }


#if 0

HANDLE PrinterX::getHandle(HWND hWnd) {
DWORD    dwNeeded;
DEVMODE* devMode;

  // 1. Open PrinterX
  OpenPrinter(name, &hPrinter, NULL);

  // 2. Get size needed
  dwNeeded = DocumentProperties(hWnd, hPrinter, name, NULL, NULL, 0);

  // 3. Allocate handle (HDEVMODE)
  hDevMode = GlobalAlloc(GHND, dwNeeded);

  devMode = (LPDEVMODE) GlobalLock(hDevMode);

  // 4. Initialize DevMode
  DocumentProperties(hWnd, hPrinter, name, devMode, NULL, DM_OUT_BUFFER);

  setDevMode(devMode);

  // 5. Unlock after use
  GlobalUnlock(hDevMode);   return hDevMode;
  }


void PrinterX::freeHandle() {

// 6. Cleanup
GlobalFree(hDevMode);
ClosePrinter(hPrinter);
}



HGLOBAL createDevnames(TCchar* driver, TCchar* device, TCchar* output) {

size_t driverLen = wcslen(driver) + 1;      // 1. Calculate required size in bytes
size_t deviceLen = wcslen(device) + 1;
size_t outputLen = wcslen(output) + 1;
size_t totalBytes = sizeof(DEVNAMES) + (driverLen + deviceLen + outputLen) * sizeof(wchar_t);

    // 2. Allocate and initialize memory
    HGLOBAL hDevnames = GlobalAlloc(GHND, totalBytes);
    if (!hDevnames) return NULL;

    // 3. Lock memory
    DEVNAMES* pDevnames = (DEVNAMES*)GlobalLock(hDevnames);
    if (!pDevnames) {
        GlobalFree(hDevnames);
        return NULL;
    }

    // 4. Populate DEVNAMES structure
    pDevnames->wDriverOffset = sizeof(DEVNAMES) / sizeof(wchar_t);
    pDevnames->wDeviceOffset = pDevnames->wDriverOffset + (WORD)driverLen;
    pDevnames->wOutputOffset = pDevnames->wDeviceOffset + (WORD)deviceLen;
    pDevnames->wDefault = 0;

    // 5. Copy strings to the memory block
    wchar_t* pStrings = (wchar_t*)(pDevnames + 1);
    wcscpy_s(pStrings, driverLen, driver);
    wcscpy_s(pStrings + driverLen, deviceLen, device);
    wcscpy_s(pStrings + driverLen + deviceLen, outputLen, output);

    // 6. Unlock
    GlobalUnlock(hDevnames);
    return hDevnames;
}

// Example Usage
// HGLOBAL hMyDevnames = CreateDevnamesHandle(L"winspool", L"PrinterName", L"LPT1:");


void PrinterX::setDevMode(HANDLE hdl) {
DEVMODE* devMode;

  devMode = (DEVMODE*) GlobalLock(hdl);           // Protect memory handle with ::GlobalLock

    setDevMode(devMode);

  GlobalUnlock(hdl);
  }




void PrinterX::saveAttr(HANDLE hdl) {
DEVMODE* devMode;

  if (hdl) {

    devMode = (DEVMODE*) GlobalLock(hdl);               // Protect memory handle with ::GlobalLock

      if (devMode->dmFields & DM_ORIENTATION)  orient    = (PrtrOrient) devMode->dmOrientation;
      if (devMode->dmFields & DM_PAPERSIZE)    paperSize = (PaperSize)  devMode->dmPaperSize;
      if (paperSize == UserDefinedPSz) {       width     =              devMode->dmPaperWidth;
                                               length    =              devMode->dmPaperLength;}
      if (devMode->dmFields & DM_COPIES)       copies    =              devMode->dmCopies;
      if (devMode->dmFields & DM_COLLATE)      collate   =              devMode->dmCollate;
      if (devMode->dmFields & DM_DUPLEX)       pagePlex  = (PagePlex)   devMode->dmDuplex;

    GlobalUnlock(hdl);
    }

  PrinterX.save();
  }



String& PrinterX::getName(HANDLE hdl) {
DEVMODE* devMode;

  if (!hdl) return name;

  devMode = (DEVMODE*) GlobalLock(hdl);                 // Protect memory handle with ::GlobalLock

    name = devMode->dmDeviceName;

  GlobalUnlock(hdl);

  return name;
  }


#endif


///////--------------

//  paperSize = LetterPSz;
  //  v = iniFile.read(name, , );
  //  if (v != ) {orient = (PrtrOrient) v;      dirty |= true;}
//  v = iniFile.read(name, ,   );
//  if (v != paperSize) { = (PaperSize) v; dirty |= true;}
//  v = iniFile.read(name, ,  0);
//  if (v != width) { = (short) v;             dirty |= true;}
//  v = iniFile.read(name, , 0);
//  if (v != length) { = v;                   dirty |= true;}
//  v = iniFile.read(name, ,      1);
//  if (v != copies) {copies = v;                   dirty |= true;}
//  v = iniFile.read(name, ,     1);
//  if (v != collate) { = v;                 dirty |= true;}
//  v = iniFile.read(name, ,    );
//  if (v != pagePlex) { = (PagePlex) v;    dirty |= true;}

