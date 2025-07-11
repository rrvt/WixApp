// Global Printer


#include "pch.h"
#include "Printer.h"
#include "IniFileEx.h"
#include "MessageBox.h"
#include "winspool.h"


       TCchar* PortraitKey    = _T("Portrait");
       TCchar* LandscapeKey   = _T("Landscape");

static TCchar* GlobalSection  = _T("Global");
static TCchar* CurPrinterKey  = _T("CurrentPrinter");
static TCchar* PaperSizeKey   = _T("PaperSize");
static TCchar* PagePlexKey    = _T("PagePlex");
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



void Printer::initAttr(HANDLE hdl) {
DEVMODE* devMode;

  printer.load(0);   if (!hdl) return;

  devMode = (DEVMODE*) GlobalLock(hdl);           // Protect memory handle with ::GlobalLock

    if (devMode->dmFields & DM_ORIENTATION) devMode->dmOrientation = orient;
    if (devMode->dmFields & DM_PAPERSIZE)   devMode->dmPaperSize   = paperSize;
    if (devMode->dmFields & DM_COPIES)      devMode->dmCopies      = copies;
    if (devMode->dmFields & DM_COLLATE)     devMode->dmCollate     = collate;
    if (devMode->dmFields & DM_DUPLEX)      devMode->dmDuplex      = pagePlex;

  GlobalUnlock(hdl);
  }


void Printer::saveAttr(HANDLE hdl) {
DEVMODE* devMode;

  if (hdl) {

    devMode = (DEVMODE*) GlobalLock(hdl);               // Protect memory handle with ::GlobalLock

      if (devMode->dmFields & DM_ORIENTATION) orient    = (PrtrOrient) devMode->dmOrientation;
      if (devMode->dmFields & DM_PAPERSIZE)   paperSize = (PaperSize)  devMode->dmPaperSize;
      if (devMode->dmFields & DM_COPIES)      copies    =              devMode->dmCopies;
      if (devMode->dmFields & DM_COLLATE)     collate   =              devMode->dmCollate;
      if (devMode->dmFields & DM_DUPLEX)      pagePlex  = (PagePlex)   devMode->dmDuplex;

    GlobalUnlock(hdl);
    }

  printer.store();
  }



String& Printer::getName(HANDLE hdl) {
DEVMODE* devMode;

  if (!hdl) return name;

  devMode = (DEVMODE*) GlobalLock(hdl);                 // Protect memory handle with ::GlobalLock

    name = devMode->dmDeviceName;

  GlobalUnlock(hdl);

  return name;
  }


void Printer::load(TCchar* printer) {
String s;
uint   x;

  if (printer) name = printer;

  if (name.isEmpty()) {
    iniFile.read(GlobalSection, CurPrinterKey, name);

    if (name.isEmpty()) {
      Tchar buf[1024];
      DWORD nBuf = noElements(buf);

      if (!GetDefaultPrinter((LPWSTR) buf, &nBuf)) return;

      name = buf;
      }
    }

  paperSize = (PaperSize)  iniFile.readInt(name, PaperSizeKey,   LetterPprSz);
  pagePlex  = (PagePlex)   iniFile.readInt(name, PagePlexKey,    SimPlex);
  iniFile.read(name, CopiesKey,      copies,        1);
  iniFile.read(name, CollateKey,     collate,       1);
  iniFile.read(name, TopMarginKey,   s,     _T("3.0"));     topMargin = s.stod(x);
  iniFile.read(name, BotMarginKey,   s,     _T("3.0"));     botMargin = s.stod(x);
  iniFile.read(name, LeftOddKey,     s,     _T("3.0"));     leftOdd   = s.stod(x);
  iniFile.read(name, RightOddKey,    s,     _T("3.0"));     rightOdd  = s.stod(x);
  iniFile.read(name, LeftEvenKey,    s,     _T("3.0"));     leftEven  = s.stod(x);
  iniFile.read(name, RightEvenKey,   s,     _T("3.0"));     rightEven = s.stod(x);
  iniFile.read(name, ScaleKey,       s,     _T("7175.0"));  scale     = s.stod(x);

  // And set defaults

  orient    = PortOrient;
  paperSize = LetterPprSz;
  copies    = 1;
  collate   = 1;
  pagePlex  = SimPlex;
  }


void Printer::store() {
String s;

  if (name.isEmpty()) return;

  iniFile.write(name, PaperSizeKey,   paperSize);
  iniFile.write(name, PagePlexKey,    pagePlex);
  iniFile.write(name, CopiesKey,      copies);
  iniFile.write(name, CollateKey,     collate);
  iniFile.write(name, TopMarginKey,   topMargin);
  iniFile.write(name, LeftOddKey,     leftOdd);
  iniFile.write(name, RightOddKey,    rightOdd);
  iniFile.write(name, LeftEvenKey,    leftEven);
  iniFile.write(name, RightEvenKey,   rightEven);
  iniFile.write(name, BotMarginKey,   botMargin);

  s = scale;   iniFile.write(name,    ScaleKey, s);

  iniFile.write(GlobalSection, CurPrinterKey, name);
  }


PrtrOrient Printer::toOrient(Cstring& cs)
                             {String s = cs;   return s == LandscapeKey ? LandOrient : PortOrient;}


TCchar* Printer::toStg(PrtrOrient orient)
                                        {return orient == PortOrient ? PortraitKey : LandscapeKey;}

