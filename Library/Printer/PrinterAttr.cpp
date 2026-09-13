// Printer Device Attributes


#include "pch.h"
#include "PrinterAttr.h"
#include "IniFileEx.h"
#include "Utilities.h"


PrinterAttr prtrDevAttr;

       TCchar* PortraitKey    = _T("Portrait");
       TCchar* LandscapeKey   = _T("Landscape");

static TCchar* GlobalSection   = _T("Global");
static TCchar* CurPrinterKey   = _T("CurrentPrinter");
static TCchar* PaperOrientKey  = _T("Orientation");
static TCchar* PaperSizeKey    = _T("PaperSize");
static TCchar* PaperWidthKey   = _T("Width");
static TCchar* PaperLengthKey  = _T("Length");
static TCchar* PlexKey         = _T("Plex");
static TCchar* CopiesKey       = _T("Copies");
static TCchar* CollateKey      = _T("Collate");
static TCchar* ColorKey        = _T("Color");
static TCchar* TopMarginKey    = _T("TopMargin");
static TCchar* LeftMarginKey   = _T("LeftMargin");
static TCchar* RightMarginKey  = _T("RightMargin");
static TCchar* BottomMarginKey = _T("BottomMargin");
static TCchar* BotMarginKey    = _T("BotMargin");
static TCchar* LeftOddKey      = _T("LeftOdd");
static TCchar* RightOddKey     = _T("RightOdd");
static TCchar* LeftEvenKey     = _T("LeftEven");
static TCchar* RightEvenKey    = _T("RightEven");
static TCchar* ScaleKey        = _T("Scale");



void PrinterAttr::load(PrinterInfo& info) {
String name = getCurrentName();

  return load(name, info);
  }


String PrinterAttr::getCurrentName() {
String name;

  iniFile.read(GlobalSection, CurPrinterKey, name);

  return name;
  }


void PrinterAttr::load(TCchar* prtrName, PrinterInfo& info) {
String name = prtrName;
String dfltWidth;
String dfltLength;
String dfltTopMgn;
String dfltLeftMgn;
String dfltRightMgn;
String dfltBottomMgn;

  if (name.isEmpty()) {loadDefaults(info);   return;}

  if (this->name == name) return;

  dfltWidth     = info.width;
  dfltLength    = info.length;
  dfltTopMgn    = info.topMgn;
  dfltLeftMgn   = info.leftMgn;
  dfltRightMgn  = info.rightMgn;
  dfltBottomMgn = info.bottomMgn;

  this->name = name;

  orient    = (PrtrOrnt) iniFile.read(name, PaperOrientKey,    (int) info.orient);
  paperSz   = (PaperSz)  iniFile.read(name, PaperSizeKey,      (int) info.paperSz);
  copies    =            iniFile.read(name, CopiesKey,         (int) info.copies);
  collate   =            iniFile.read(name, CollateKey,        (int) info.collate);
  plex      =            iniFile.read(name, PlexKey,           (int) info.plex);
  color     =            iniFile.read(name, ColorKey,          (int) info.color);
  iniFile.read(name, PaperWidthKey,   width,     dfltWidth);
  iniFile.read(name, PaperLengthKey,  length,    dfltLength);
  iniFile.read(name, TopMarginKey,    topMgn,    dfltTopMgn);
  iniFile.read(name, LeftMarginKey,   leftMgn,   dfltLeftMgn);
  iniFile.read(name, RightMarginKey,  rightMgn,  dfltRightMgn);
  iniFile.read(name, BottomMarginKey, bottomMgn, dfltBottomMgn);
  validatePaperSz();
  }


void PrinterAttr::loadDefaults(PrinterInfo& info) {
  orient    = (PrtrOrnt) info.orient;
  copies    = info.copies;
  collate   = info.collate;
  color     = info.color;
  plex      = info.plex;
  paperSz   = info.paperSz;
  width     = info.width;
  length    = info.length;
  topMgn    = info.topMgn;
  leftMgn   = info.leftMgn;
  rightMgn  = info.rightMgn;
  bottomMgn = info.bottomMgn;
  validatePaperSz();
  }


void PrinterAttr::validatePaperSz() {
PrinterPgSz tbl;
int         i    = tbl.find(width, length);
PaperSz     item = tbl.getItem(i);

  if (item != paperSz) paperSz = item;
  }





void PrinterAttr::save() {
String s;

  if (name.isEmpty()) return;

  iniFile.write(name, PaperOrientKey,  orient);
  iniFile.write(name, CopiesKey,       copies);
  iniFile.write(name, CollateKey,      collate);
  iniFile.write(name, ColorKey,        color);
  iniFile.write(name, PlexKey,         plex);
  iniFile.write(name, PaperSizeKey,    paperSz);
  iniFile.write(name, PaperWidthKey,   width);
  iniFile.write(name, PaperLengthKey,  length);
  iniFile.write(name, TopMarginKey,    topMgn);
  iniFile.write(name, LeftMarginKey,   leftMgn);
  iniFile.write(name, RightMarginKey,  rightMgn);
  iniFile.write(name, BottomMarginKey, bottomMgn);

  iniFile.write(GlobalSection, CurPrinterKey, name);
  }


void PrinterAttr::set(double width, double length)
                  {this->width = width;   this->length = length;   paperSz = UserDefinedPSz;}




String& PrinterAttr::getDevName(HANDLE hdl) {
DEVMODE* devMode;

  if (!hdl) {name.clear();   return name;}

  devMode = (DEVMODE*) GlobalLock(hdl);   getDevName(*devMode);   GlobalUnlock(devMode);

  return name;
  }


String& PrinterAttr::getDevName(DEVMODE& devMode) {
Tchar* data = (Tchar*)&devMode.dmDeviceName;
int    n    = CCHDEVICENAME;
int    i;
Tchar  ch;

  name.clear();

  for (i = 0, ch = data[i]; ch && i < n; ch = data[++i]) name += ch;

  return name;
  }


void PrinterAttr::setDevName(DEVMODE* devMode) {
int   i;
int   n = CCHDEVICENAME;
Tchar ch;

  for (i = 0, ch = name[i]; ch && i < n; ch = name[++i]) devMode->dmDeviceName[i] = ch;

  if (i < CCHDEVICENAME) devMode->dmDeviceName[i] = 0;
  }


/////////////------------
#if 0
void PrinterAttr::get(HANDLE hdl) {
DEVMODE* devMode;
double   w;
double   l;

  if (!hdl) return;

  devMode = (DEVMODE*) GlobalLock(hdl);   if (!devMode) return;
                                                    // Protect memory handle with ::GlobalLock
    getDevName(*devMode);
    if (devMode->dmFields & DM_ORIENTATION) orient   = (PrtrOrnt) devMode->dmOrientation;
    if (devMode->dmFields & DM_PAPERSIZE)   paperSz  = (PaperSz)  devMode->dmPaperSize;
    w = devMode->dmPaperWidth;   w /= 254;   width.format(_T("%.3f"),  w);   trimZ(width);
    l = devMode->dmPaperLength;  l /= 254;   length.format(_T("%.3f"), l);   trimZ(length);
    if (devMode->dmFields & DM_COPIES)      copies   =            devMode->dmCopies;
    if (devMode->dmFields & DM_COLLATE)     collate  =            devMode->dmCollate;
    if (devMode->dmFields & DM_DUPLEX)      plex  = (Plex)  devMode->dmDuplex;

  GlobalUnlock(devMode);
  }


void PrinterAttr::set(String& name, HANDLE devMode) {

  if (name.isEmpty() && !devMode) return;

  if (name.isEmpty()) {get(devMode);   save();   return;}

  if (!devMode)       {load(name);               return;}

  load(name);   if (loaded) set(devMode);
  }



void PrinterAttr::set(HANDLE hdl) {
DEVMODE* devMode;
HANDLE   hPrinter;

  if (!hdl) return;

  devMode = (DEVMODE*) GlobalLock(hdl);   if (!devMode) return;
                                                        // Protect memory handle with ::GlobalLock
    setDevName(devMode);   set(*devMode);
#if 0
    // 5. Update the dialog's internal DEVMODE with the changes
            CString deviceName = printDlg.GetDeviceName();
            if (::OpenPrinter((LPTSTR)(LPCTSTR)deviceName, &hPrinter, nullptr))
            {
                // Synchronize the driver with the modified DEVMODE settings
                ::DocumentProperties(nullptr, hPrinter, (LPTSTR)(LPCTSTR)deviceName,
                                    pDevMode, pDevMode, DM_IN_BUFFER | DM_OUT_BUFFER);
                ::ClosePrinter(hPrinter);
            }
#endif
  GlobalUnlock(devMode);
  }



void PrinterAttr::set(DEVMODE& devMode) {
ulong& fields = devMode.dmFields;
uint   pos;

  fields |= DM_ORIENTATION; devMode.dmOrientation = orient;
  fields |= DM_PAPERSIZE;   devMode.dmPaperSize   = paperSz;
  fields |= DM_PAPERWIDTH;  devMode.dmPaperWidth  = short(width.stod(pos)  * 254);
  fields |= DM_PAPERLENGTH; devMode.dmPaperLength = short(length.stod(pos) * 254);
  fields |= DM_COPIES;      devMode.dmCopies      = copies;
  fields |= DM_COLLATE;     devMode.dmCollate     = collate;
  fields |= DM_DUPLEX;      devMode.dmDuplex      = plex;
  }
#endif

