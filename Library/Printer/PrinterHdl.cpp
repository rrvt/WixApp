// Use to Create a printer DC


#include "pch.h"
#include "PrinterHdl.h"
#include "PrinterAttr.h"
#include <winspool.h>


PrinterHdl::~PrinterHdl() {
  if (m_pd.hDevMode)  GlobalFree(m_pd.hDevMode);
  if (m_pd.hDevNames) GlobalFree(m_pd.hDevNames);
  }


HDC PrinterHdl::operator() () {

  printerName = prtrDevAttr.name;

  if (!setDevMode()) return 0;

  setDevNames();

  return CreatePrinterDC();
  }


bool PrinterHdl::setDevMode() {
HANDLE   prtrHdl = 0;
HGLOBAL  modeHdl = 0;
ulong    needed;
DEVMODE* mode;

  if (!findPrinterNames()) return false;

  if (!OpenPrinter(printerName, &prtrHdl, NULL)) return false;

    needed = DocumentProperties(0, prtrHdl, printerName, 0, 0, 0);    // Get no bytes

    modeHdl  = (HGLOBAL) GlobalAlloc(GHND, needed);                   // Allocate global node
    if (!modeHdl) {ClosePrinter(prtrHdl);   return false;}            // lock to get pointer
    mode = (DEVMODE*) GlobalLock(modeHdl);   if (!mode) return false; // Initialize with the
                                                                      // printer's current settings
      if (DocumentProperties(0, prtrHdl, printerName, mode, 0, DM_OUT_BUFFER) != IDOK)
           {GlobalUnlock(modeHdl);   GlobalFree(modeHdl);   ClosePrinter(prtrHdl);   return false;}
                                                                      // Modify DEVMODE settings
        setAttr(*mode);                                               // and Merge changes to
                                                                      // update private sections
      DocumentProperties(0, prtrHdl, printerName, mode, mode, DM_IN_BUFFER|DM_OUT_BUFFER);

    GlobalUnlock(modeHdl);   m_pd.hDevMode = modeHdl;

  ClosePrinter(prtrHdl);   return true;               // 7. Clean up
  }


void PrinterHdl::setAttr(DEVMODE& devMode) {
ulong& fields = devMode.dmFields;
uint   pos;

  setName(prtrDevAttr.name, &devMode);

  fields |= DM_ORIENTATION; devMode.dmOrientation = short(prtrDevAttr.orient);
  fields |= DM_PAPERSIZE;   devMode.dmPaperSize   = short(prtrDevAttr.paperSz);
  fields |= DM_PAPERWIDTH;  devMode.dmPaperWidth  = short(prtrDevAttr.width.stod(pos)  * 254);
  fields |= DM_PAPERLENGTH; devMode.dmPaperLength = short(prtrDevAttr.length.stod(pos) * 254);
  fields |= DM_COPIES;      devMode.dmCopies      = short(prtrDevAttr.copies);
  fields |= DM_COLLATE;     devMode.dmCollate     = short(prtrDevAttr.collate);
  fields |= DM_DUPLEX;      devMode.dmDuplex      = short(prtrDevAttr.plex);
  }


static const ulong DrvrFlags = PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS;

bool PrinterHdl::findPrinterNames() {
NewArray(Byte);
ulong           need = 0;
ulong           n    = 0;
Byte*           p;
PRINTER_INFO_2* prtrList;
ulong           i;

  // First call to determine required p size
  EnumPrinters(DrvrFlags, 0, 2, 0, 0, &need, &n);                         // get no. bytes

  p = AllocArray(need);   if (!p) return false;                           // Allocate buffer

  if (!EnumPrinters(DrvrFlags, 0, 2, p, need, &need, &n)) return _T("");  // Get data

  prtrList = (PRINTER_INFO_2*) p;

  for (i = 0; i < n; i++) {        // Compare the printer name (case-insensitive if needed)

    PRINTER_INFO_2& item = prtrList[i];

    if (printerName == item.pPrinterName)
      {driverName= item.pDriverName;   portName   = item.pPortName;   FreeArray(p);   return true;}
    }

  FreeArray(p);   return false;
  }


// Allocate and construct the DEVNAMES structure

void PrinterHdl::setDevNames() {
int       driverLng = driverName.length() + 1;
int       deviceLng = printerName.length() + 1;
int       portLng   = portName.length()   + 1;
size_t    nSize;
HGLOBAL   namesHdl;
DEVNAMES* names;
Tchar*    pDest;

  nSize = sizeof(DEVNAMES) + (driverLng + deviceLng + portLng) * sizeof(TCHAR);

  namesHdl = GlobalAlloc(GHND, nSize);
  names = (DEVNAMES*)::GlobalLock(namesHdl);

    names->wDriverOffset = sizeof(DEVNAMES) / sizeof(TCHAR);
    names->wDeviceOffset = names->wDriverOffset + driverLng;
    names->wOutputOffset = names->wDeviceOffset + deviceLng;
    names->wDefault = DN_DEFAULTPRN;

    pDest = (Tchar*) names;                              // Copy strings into the memory block

    _tcscpy_s(pDest + names->wDriverOffset, driverLng, driverName);
    _tcscpy_s(pDest + names->wDeviceOffset, deviceLng, printerName);
    _tcscpy_s(pDest + names->wOutputOffset, portLng,   portName);

  GlobalUnlock(namesHdl);   m_pd.hDevNames = namesHdl;
  }





void PrinterHdl::setName(TCchar* name, DEVMODE* devMode) {
int   i;
int   n = CCHDEVICENAME - 1;
Tchar ch;

  for (i = 0, ch = name[0]; ch && i < n; ch = name[++i]) devMode->dmDeviceName[i] = ch;

  if (i < CCHDEVICENAME) devMode->dmDeviceName[i] = 0;
  }





#if 0
void CDialogApp::saveDevMode(HGLOBAL hndl) {
void* pSource;
void* pTarget;

  if (!hndl) return;

  if (m_hDevMode) GlobalFree(m_hDevMode);           // Copy the DEVMODE structure

  m_hDevMode = (HGLOBAL) GlobalAlloc(GHND, GlobalSize(hndl));

  if (m_hDevMode) {
    pSource = GlobalLock(hndl);
    pTarget = GlobalLock(m_hDevMode);                     // WinAppEx::m_hDevMode

      memcpy(pTarget, pSource, GlobalSize(hndl));

    GlobalUnlock(m_hDevMode);
    GlobalUnlock(hndl);
    }
  }


  if (OpenPrinter((LPTSTR) prtrDevAttr.name, &prtrHdl, NULL)) {
    needed = DocumentProperties(m_pdex.hwndOwner, prtrHdl, (LPTSTR) prtrDevAttr.name, 0, 0, 0);

    devModeHdl = GlobalAlloc(GHND, needed);
    devMode    = (DEVMODE*) GlobalLock(devModeHdl);

      DocumentProperties(m_pdex.hwndOwner, prtrHdl, (LPTSTR) prtrDevAttr.name,
                                                                        devMode, 0, DM_OUT_BUFFER);
      setName(prtrDevAttr.name, devMode);

    GlobalUnlock(devModeHdl);

    dwNamesSize = sizeof(DEVNAMES) + (prtrDevAttr.name.length() + 1) * sizeof(TCHAR);
    hDevNames   = GlobalAlloc(GHND, dwNamesSize);
      pDevNames = (DEVNAMES*) GlobalLock(hDevNames);
      pDevNames->wDriverOffset = sizeof(DEVNAMES) / sizeof(TCHAR);
      pDevNames->wDeviceOffset = sizeof(DEVNAMES) / sizeof(TCHAR) + 0; // Adjust for driver name
      pDevNames->wOutputOffset = 0;                 // Port name offset (can be 0 if not needed)
      pDevNames->wDefault      = 0;

      lpszDest = (LPTSTR) pDevNames + pDevNames->wDeviceOffset;
      _tcscpy_s(lpszDest, prtrDevAttr.name.length() + 1, prtrDevAttr.name);

    GlobalUnlock(hDevNames);

    m_pdex.hDevMode  = devModeHdl;   devModeHdl = 0;
    m_pdex.hDevNames = hDevNames;    hDevNames  = 0;
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


//  if (!GetDefaults()) return 0;
    String s;   s.format(_T("Driver, port: (%s, %s)"), driverName.str(), portName.str());
    messageBox(s);
    }
#endif

//  m_pd.Flags |= PD_RETURNDC;
    //  p = AllocArray(needed);   mode = (DEVMODE*) p;

#if 0
      DEVMODE* dftMd = (DEVMODE*) GlobalLock(m_pd.hDevMode);

        compare(dftMd, mode);

      GlobalUnlock(m_pd.hDevMode);   GlobalFree(m_pd.hDevMode);

#endif
#if 0
void PrinterHdl::setDevMode() {
int      sz = sizeof(DEVMODE);
HGLOBAL  h  = (HGLOBAL) GlobalAlloc(GHND, sz);
DEVMODE* mode;

  mode = (DEVMODE*) GlobalLock(h);
    mode->dmSpecVersion = DM_SPECVERSION;
    mode->dmSize        = sz;
    mode->dmScale       = 100;            mode->dmFields |= DM_SCALE;
    mode->dmNup         = DMNUP_SYSTEM;   mode->dmFields |= DM_NUP;

    getPrinterDriverVersion(prtrDevAttr.name, mode);

    setAttr(*mode);
#if 1
DEVMODE* dftMd = (DEVMODE*) GlobalLock(m_pd.hDevMode);

  compare(dftMd, mode);

  GlobalUnlock(m_pd.hDevMode);
#else
  m_pd.hDevMode = h;
#endif
  GlobalUnlock(h);
  }
#endif
#if 0

void PrinterHdl::compare(DEVMODE* d1, DEVMODE* d2) {
//int    i;
//String s;

  compare(_T("dmDeviceName"), d1->dmDeviceName, d2->dmDeviceName, CCHDEVICENAME);
//  for (i = 0; i < CCHDEVICENAME; i++) {
//    if (d1->dmDeviceName[i] != d2->dmDeviceName[i]) messageBox(_T("dmDeviceName"));
//    }
  compare(_T("dmSpecVersion"),      d1->dmSpecVersion ,     d2->dmSpecVersion);
  compare(_T("dmDriverVersion"),    d1->dmDriverVersion,    d2->dmDriverVersion);
  compare(_T("dmSize"),             d1->dmSize,             d2->dmSize);
  compare(_T("dmDriverExtra"),      d1->dmDriverExtra,      d2->dmDriverExtra);
  identBits(                        d1->dmFields,           d2->dmFields);
  compare(_T("dmOrientation  "),    d1->dmOrientation  ,    d2->dmOrientation  );
  compare(_T("dmPaperSize    "),    d1->dmPaperSize    ,    d2->dmPaperSize    );
  compare(_T("dmPaperLength  "),    d1->dmPaperLength  ,    d2->dmPaperLength  );
  compare(_T("dmPaperWidth   "),    d1->dmPaperWidth   ,    d2->dmPaperWidth   );
  compare(_T("dmScale        "),    d1->dmScale        ,    d2->dmScale        );
  compare(_T("dmCopies       "),    d1->dmCopies       ,    d2->dmCopies       );
  compare(_T("dmDefaultSource"),    d1->dmDefaultSource,    d2->dmDefaultSource);
  compare(_T("dmPrintQuality "),    d1->dmPrintQuality ,    d2->dmPrintQuality );

  compare(_T("dmColor      "),      d1->dmColor          ,  d2->dmColor                );
  compare(_T("dmDuplex     "),      d1->dmDuplex         ,  d2->dmDuplex               );
  compare(_T("dmYResolution"),      d1->dmYResolution    ,  d2->dmYResolution          );
  compare(_T("dmTTOption   "),      d1->dmTTOption       ,  d2->dmTTOption             );
  compare(_T("dmCollate    "),      d1->dmCollate        ,  d2->dmCollate              );
  compare(_T("dmFormName   "),      d1->dmFormName       ,  d2->dmFormName, CCHFORMNAME);
  compare(_T("dmLogPixels  "),      d1->dmLogPixels      ,  d2->dmLogPixels            );
  compare(_T("dmBitsPerPel "),      d1->dmBitsPerPel     ,  d2->dmBitsPerPel           );
  compare(_T("dmPelsWidth  "),      d1->dmPelsWidth      ,  d2->dmPelsWidth            );
  compare(_T("dmPelsHeight "),      d1->dmPelsHeight     ,  d2->dmPelsHeight           );

  compare(_T("dmNup             "), d1->dmNup             , d2->dmNup             );
  compare(_T("dmDisplayFrequency"), d1->dmDisplayFrequency, d2->dmDisplayFrequency);
  compare(_T("dmICMMethod       "), d1->dmICMMethod       , d2->dmICMMethod       );
  compare(_T("dmICMIntent       "), d1->dmICMIntent       , d2->dmICMIntent       );
  compare(_T("dmMediaType       "), d1->dmMediaType       , d2->dmMediaType       );
  compare(_T("dmDitherType      "), d1->dmDitherType      , d2->dmDitherType      );
  compare(_T("dmReserved1       "), d1->dmReserved1       , d2->dmReserved1       );
  compare(_T("dmReserved2       "), d1->dmReserved2       , d2->dmReserved2       );
  compare(_T("dmPanningWidth    "), d1->dmPanningWidth    , d2->dmPanningWidth    );
  compare(_T("dmPanningHeight   "), d1->dmPanningHeight   , d2->dmPanningHeight   );
  }



void PrinterHdl::compare(TCchar* prefix, Tchar* w1, Tchar* w2, int n) {
int    i;
Tchar  b1;
Tchar  b2;
String s;

  for (i = 0; i < n; i++) {
    b1 = w1[i];   b2 = w2[i];

    if (b1 != b2) {s.format(_T("%s: %s != %s"), prefix, w1, w2);   messageBox(s);  break;}
    }
  }


void PrinterHdl::compare(TCchar* prefix, short w1, short w2) {
String s;

  if (w1 != w2)
    {s.format(_T("%s: %hi != %hi"), prefix, w1, w2);   messageBox(s);}
  }

void PrinterHdl::compare(TCchar* prefix, ushort w1, ushort w2) {
String s;

  if (w1 != w2)
    {s.format(_T("%s: %hu != %hu"), prefix, w1, w2);   messageBox(s);}
  }


void PrinterHdl::compare(TCchar* prefix, ulong w1, ulong w2) {
String s;

  if (w1 != w2)
    {s.format(_T("%s: %lu != %lu"), prefix, w1, w2);   messageBox(s);}
  }


void PrinterHdl::compareX(TCchar* prefix, ulong  w1, ulong  w2) {
String s;

  if (w1 != w2)
    {s.format(_T("%s: 0x%0lx != 0x%0lx"), prefix, w1, w2);   messageBox(s);}
  }

struct Bit {
ulong  bit;
String name;
};

static Bit bits[] = {{DM_ORIENTATION       , _T("DM_ORIENTATION        "}),
                    {DM_PAPERSIZE          , _T("DM_PAPERSIZE          "}),
                    {DM_PAPERLENGTH        , _T("DM_PAPERLENGTH        "}),
                    {DM_PAPERWIDTH         , _T("DM_PAPERWIDTH         "}),
                    {DM_SCALE              , _T("DM_SCALE              "}),
                    {DM_POSITION           , _T("DM_POSITION           "}),
                    {DM_NUP                , _T("DM_NUP                "}),     //DMNUP_SYSTEM=1
                    {DM_DISPLAYORIENTATION , _T("DM_DISPLAYORIENTATION "}),
                    {DM_COPIES             , _T("DM_COPIES             "}),
                    {DM_DEFAULTSOURCE      , _T("DM_DEFAULTSOURCE      "}),
                    {DM_PRINTQUALITY       , _T("DM_PRINTQUALITY       "}),
                    {DM_COLOR              , _T("DM_COLOR              "}),
                    {DM_DUPLEX             , _T("DM_DUPLEX             "}),
                    {DM_YRESOLUTION        , _T("DM_YRESOLUTION        "}),
                    {DM_TTOPTION           , _T("DM_TTOPTION           "}),
                    {DM_COLLATE            , _T("DM_COLLATE            "}),
                    {DM_FORMNAME           , _T("DM_FORMNAME           "}),
                    {DM_LOGPIXELS          , _T("DM_LOGPIXELS          "}),
                    {DM_BITSPERPEL         , _T("DM_BITSPERPEL         "}),
                    {DM_PELSWIDTH          , _T("DM_PELSWIDTH          "}),
                    {DM_PELSHEIGHT         , _T("DM_PELSHEIGHT         "}),
                    {DM_DISPLAYFLAGS       , _T("DM_DISPLAYFLAGS       "}),
                    {DM_DISPLAYFREQUENCY   , _T("DM_DISPLAYFREQUENCY   "}),
                    {DM_ICMMETHOD          , _T("DM_ICMMETHOD          "}),
                    {DM_ICMINTENT          , _T("DM_ICMINTENT          "}),
                    {DM_MEDIATYPE          , _T("DM_MEDIATYPE          "}),
                    {DM_DITHERTYPE         , _T("DM_DITHERTYPE         "}),
                    {DM_PANNINGWIDTH       , _T("DM_PANNINGWIDTH       "}),
                    {DM_PANNINGHEIGHT      , _T("DM_PANNINGHEIGHT      "}),
                    {DM_DISPLAYFIXEDOUTPUT , _T("DM_DISPLAYFIXEDOUTPUT "}),
                    };

void PrinterHdl::identBits(ulong  w1, ulong  w2) {
int    i;
ulong  b1;
ulong  b2;
String s;

  for (i = 0; i < noElements(bits); i++) {
    Bit& itm = bits[i];

    b1 = itm.bit & w1;
    b2 = itm.bit & w2;
    if (b1 != b2) {s.format(_T("%s: 0x%08x != 0x%08x"), itm.name.str(), b1, b2);   messageBox(s);}
    }
  }


void PrinterHdl::examine(TCchar* prefix, ushort v)
                                 {String s;   s.format(_T("%s: %hu"), prefix, v);   messageBox(s);}
#endif
#if 0
void PrinterHdl::getPrinterDriverVersion(Tchar* printerName, DEVMODE* mode) {
NewArray(Byte);
HANDLE         hdl    = 0;
ulong          needed = 0;
Byte*          p;
DRIVER_INFO_3* pdi3;

//messageBox(printerName);

  if (!OpenPrinter(printerName, &hdl, NULL) || !hdl) return;

  GetPrinterDriver(hdl, 0, 3, 0, 0, &needed);               // Get the size of the buffer needed

  p = AllocArray(needed);   if (!p) return;

    if (GetPrinterDriver(hdl, 0, 3, p, needed, &needed)) {  // Get the actual driver info

      pdi3 = (DRIVER_INFO_3*) p;

      mode->dmDriverVersion = (ushort) pdi3->cVersion;
      }                                             // Windows Driver Model version (e.g., 3 or 4)

  FreeArray(p);

  ClosePrinter(hdl);
  }
#endif
#if 0
void PrinterHdl::setDevNames() {
uint      dwNamesSize;
HGLOBAL   hDevNames;
DEVNAMES* pDevNames;
Tchar*    lpszDest;

  dwNamesSize = sizeof(DEVNAMES) + (prtrDevAttr.name.length() + 1) * sizeof(TCHAR);

  hDevNames   = GlobalAlloc(GHND, dwNamesSize);

    pDevNames = (DEVNAMES*) GlobalLock(hDevNames);
    pDevNames->wDriverOffset = sizeof(DEVNAMES) / sizeof(TCHAR);
    pDevNames->wDeviceOffset = sizeof(DEVNAMES) / sizeof(TCHAR) + 0; // Adjust for driver name
    pDevNames->wOutputOffset = 0;                 // Port name offset (can be 0 if not needed)
    pDevNames->wDefault      = 0;

//    lpszDest = (Tchar*) pDevNames + pDevNames->wDriverOffset;   *lpszDest = 0;

    lpszDest = (Tchar*) pDevNames + pDevNames->wDeviceOffset;

    _tcscpy_s(lpszDest, prtrDevAttr.name.length() + 1, prtrDevAttr.name);

  GlobalUnlock(hDevNames);

  m_pd.hDevNames = hDevNames;
  }
#endif
#if 0
HGLOBAL PrinterHdl::createDevnames(TCchar* driver, TCchar* device, TCchar* output) {
size_t    driverLen  = (driver ? _tcsclen(driver) : 0) + 1; // 1. Calculate required size in bytes
size_t    deviceLen  = (device ? _tcsclen(device) : 0) + 1;
size_t    outputLen  = (output ? _tcsclen(output) : 0) + 1;
size_t    totalBytes = sizeof(DEVNAMES) + (driverLen + deviceLen + outputLen) * sizeof(TCchar);
HGLOBAL   hDevnames;
DEVNAMES* pDevnames;

    // 2. Allocate and initialize memory
    hDevnames = GlobalAlloc(GHND, totalBytes);   if (!hDevnames) return 0;

    // 3. Lock memory
    pDevnames = (DEVNAMES*) GlobalLock(hDevnames);
    if (!pDevnames) {GlobalFree(hDevnames); return 0;}

    // 4. Populate DEVNAMES structure
    pDevnames->wDriverOffset = sizeof(DEVNAMES) / sizeof(TCchar);
    pDevnames->wDeviceOffset = pDevnames->wDriverOffset + (WORD) driverLen;
    pDevnames->wOutputOffset = pDevnames->wDeviceOffset + (WORD) deviceLen;
    pDevnames->wDefault = 0;

    // 5. Copy strings to the memory block
    wchar_t* pStrings = (wchar_t*)(pDevnames + 1);

    _tcscpy_s(pStrings, driverLen, driver);
    _tcscpy_s(pStrings + driverLen, deviceLen, device);
    _tcscpy_s(pStrings + driverLen + deviceLen, outputLen, output);

    // 6. Unlock
    GlobalUnlock(hDevnames);
    return hDevnames;
}
#endif
#if 0
String& PrinterHdl::getName(DEVMODE* devMode) {
static String name;
Tchar*        data = (Tchar*) &devMode->dmDeviceName;
int           n    = CCHDEVICENAME;
int           i;
Tchar         ch;

  name.clear();

  for (i = 0, ch = data[i]; ch && i < n; ch = data[++i]) name += ch;

  return name;
  }
#endif

