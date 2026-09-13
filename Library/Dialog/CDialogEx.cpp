// Extension of CDialogEx


#include "pch.h"
#include "DialogEx.h"
#include "DialogApp.h"
#include "MessageBox.h"
#include "PrinterPageAttrDlg.h"
#include "PrtrDevAttr.h"
#include "prtrMgr.h"
#include "StatusBar.h"


static void dbg(TCchar* fmt, void* v);


static const ulong PrtrFlags = PD_ALLPAGES | PD_USEDEVMODECOPIES | PD_NOPAGENUMS |
                                                               PD_HIDEPRINTTOFILE | PD_NOSELECTION;
static const ulong PageFlags = PSD_MARGINS | PSD_INWININIINTLMEASURE;


BOOL DialogEx::OnInitDialog() {

  CDialogEx::OnInitDialog();

  if (!statusBar.create(this, IDC_StatusBar)) return false;

  return true;
  }



void DialogEx::onFilePrint() {
CPrintDialog dlg(false, PrtrFlags, this);
PRINTDLG&    pd  = dlg.m_pd;
HDC&         hdc = pd.hDC;
String       name;

  setDefaultPrinter();

  pd.hDevMode  = theDlgApp->getDevMode();
  pd.hDevNames = theDlgApp->getDevNames();

  name = dlg.GetDeviceName();   statusBar.setText(name);

  if (dlg.DoModal() != IDOK) return;

  name = dlg.GetDeviceName();

  if (name != prtrDevAttr.name) prtrDevAttr.load(name);

  setPrinterPageDefaults(pd.hDevMode);

  if (hdc) {::DeleteDC(hdc);   hdc = 0;   hdc = dlg.CreatePrinterDC();}

  statusBar.setText(name);

  doPrint(dlg);

  theDlgApp->setDevMode(pd.hDevMode);
  theDlgApp->setDevNames(pd.hDevNames);
  }


#if 0
void CMyDialog::OnPrint()
{
    // 1. Create and display the Print Dialog
    CPrintDialog printDlg(FALSE);
    if (printDlg.DoModal() == IDOK)
    {
        // 2. Get the Printer DC handle
        HDC hdc = printDlg.GetPrinterDC();
        if (hdc != NULL)
        {
            // 3. Wrap it in a CDC object
            CDC dc;
            dc.Attach(hdc);

            // Critical: Set m_bPrinting so IsPrinting() returns TRUE
            dc.m_bPrinting = TRUE;

            // 4. Standard Printing Protocol
            DOCINFO di;
            memset(&di, 0, sizeof(DOCINFO));
            di.cbSize = sizeof(DOCINFO);
            di.lpszDocName = _T("My Document");

            if (dc.StartDoc(&di) > 0)
            {
                dc.StartPage();

                // Your drawing code here (GDI calls)
                dc.TextOut(100, 100, _T("Hello Printer!"));

                dc.EndPage();
                dc.EndDoc();
            }

            // Clean up: CDC::Detach prevents the destructor from
            // deleting the HDC prematurely if you want to manage it elsewhere,
            // but usually dc.Detach() isn't needed if dc owns the HDC here.
            dc.Detach();
            ::DeleteDC(hdc); // Important: Manually delete the HDC from GetPrinterDC()
        }
    }
}
#endif

#if 0
struct _DOCINFOA {
  int    cbSize;              // The size, in bytes, of the structure.
  LPCSTR lpszDocName;         // Pointer to a null-terminated string that specifies the name of the
                              // document.
  LPCSTR lpszOutput;          // Pointer to a null-terminated string that specifies the name of an
                              // output file. If this pointer is NULL, the output will be sent to
                              // the device identified by the device context handle that was passed
                              // to the StartDoc function.
  LPCSTR lpszDatatype;        // Pointer to a null-terminated string that specifies the type of
                              // data used to record the print job. The legal values for this
                              // member can be found by calling EnumPrintProcessorDatatypes and can
                              // include such values as raw, emf, or XPS_PASS. This member can be
                              // NULL. Note that the requested data type might be ignored.
  DWORD  fwType;              // Specifies additional information about the print job. This member
                              // must be zero or one of the following values:
                              // * DI_APPBANDING Applications that use banding should set this flag
                              //   for optimal performance during printing.
                              // * DI_ROPS_READ_DESTINATION The application will use raster
                              //   operations that involve reading from the destination surface.
  }
#endif


void DialogEx::doPrint(CPrintDialog& dlg) {
HDC     hdc = dlg.GetPrinterDC();   if (!hdc) return;
PrtrMgr prtr(hdc);

#if 1

  prtr.SetMapMode(MM_TEXT);

  prtr.setPrinter(dlg.GetDeviceName());
  prtr.width     = toHMM(prtrDevAttr.width);
  prtr.length    = toHMM(prtrDevAttr.length);
  prtr.topMgn    = toHMM(prtrDevAttr.topMgn);
  prtr.leftMgn   = toHMM(prtrDevAttr.leftMgn);
  prtr.rightMgn  = toHMM(prtrDevAttr.rightMgn);
  prtr.bottomMgn = toHMM(prtrDevAttr.bottomMgn);

  onPrint(prtr);

#else

//CDC     dc;
//DOCINFO di;


    di.lpszDocName = _T("My Document");

    if (prtr.StartDoc(&di) > 0)  {
      prtr.StartPage();

      // Your drawing code here (GDI calls)
      prtr.TextOut(100, 100, _T("Hello Printer!"));

      prtr.EndPage();
      prtr.EndDoc();
      }

    prtr.Detach();   ::DeleteDC(hdc);     // Clean up: CDC::Detach prevents the destructor from
                                          // deleting the HDC prematurely if you want to manage it
                                          // elsewhere, but usually dc.Detach() isn't needed if dc
#endif                                    // owns the HDC here.  Important: Manually delete the HDC
  }


int DialogEx::toHMM(String& s) {uint x;   return int(s.stod(x) * 254.0 + 0.5);}






/*
BOOL CScrView::OnPreparePrinting(CPrintInfo* info) {
CPrintDialog* dlg      = info->m_pPD;
PRINTDLG&     pd       = dlg->m_pd;
HGLOBAL       hDevMode = pd.hDevMode;
Cstring       name;
String s;

  if (info->m_bPreview) return onPreparePreview(info);

  if (!theCApp->GetPrinterDeviceDefaults(&pd))
                               {messageBox(_T("GetPrinterDeviceDefaults failed"));   return false;}

  onPreparePrinting(info);   printer.updateDevMode(pd.hDevMode);

  if (dlg->DoModal() != IDOK) return false;

  name = dlg->GetDeviceName();

  if (name != printer.name) {

    printer.updateAttr(name);   onPreparePrinting(info);

    if (printer.updateDevMode(pd.hDevMode)) {

      HDC& hdc = pd.hDC;   if (hdc) {::DeleteDC(hdc);   hdc = 0;

      hdc = dlg->CreatePrinterDC();}
      }
    }

  printer.save();   theCApp->clearDevMode();   return true;
  }
*/


void DialogEx::onSetupPrinter() {
CPrintDialog  dlg(true, PrtrFlags, this);
PRINTDLG&     pd = dlg.m_pd;
Cstring       name;

  setDefaultPrinter();

  pd.hDevMode  = theDlgApp->getDevMode();
  pd.hDevNames = theDlgApp->getDevNames();

  name = dlg.GetDeviceName();   statusBar.setText(name);

  if (dlg.DoModal() != IDOK) return;

  name = dlg.GetDeviceName();   statusBar.setText(name);

  setPrinterPageDefaults(pd.hDevMode);

  theDlgApp->setDevMode(pd.hDevMode);
  theDlgApp->setDevNames(pd.hDevNames);
  }


void DialogEx::setPrinterPageDefaults(HANDLE hDevMode) {
DEVMODE* devMode;

  devMode = (DEVMODE*) GlobalLock(hDevMode);

    if (devMode) onPreparePrinting(*devMode);

  GlobalUnlock(hDevMode);
  }


void DialogEx::onPrintPageSetup() {
#if 1
PrinterPageAttrDlg dlg;

  setDefaultPrinter();

  setPrinterPageDlg(theDlgApp->getDevMode(), dlg);

  dlg.DoModal();

  getPrinterPageDlg(dlg);   prtrDevAttr.save();


#else
CPageSetupDialog dlg(PageFlags, this);
PAGESETUPDLG&    psd = dlg.m_psd;
Cstring          name;

  setDefaultPrinter();

  psd.hDevMode  = theDlgApp->getDevMode();
  psd.hDevNames = theDlgApp->getDevNames();

  name = dlg.GetDeviceName();   statusBar.setText(name);

  if (dlg.DoModal() != IDOK) return;

  theDlgApp->setDevMode(psd.hDevMode);
  theDlgApp->setDevNames(psd.hDevNames);
#endif
  }


void DialogEx::setPrinterPageDlg(HANDLE hDevMode, PrinterPageAttrDlg& dlg) {
DEVMODE* devMode;

  if (!hDevMode) return;

  devMode = (DEVMODE*) GlobalLock(hDevMode);    if (!devMode) return;

    prtrDevAttr.getDevName(*devMode);

    prtrDevAttr.load(prtrDevAttr.name);

    dlg.name         = prtrDevAttr.name;
    dlg.orientation  = prtrDevAttr.orient;
    dlg.paperSize    = prtrDevAttr.paperSz;
    dlg.pageWidth    = prtrDevAttr.width;
    dlg.pageLength   = prtrDevAttr.length;
    dlg.topMargin    = prtrDevAttr.topMgn;
    dlg.leftMargin   = prtrDevAttr.leftMgn;
    dlg.rightMargin  = prtrDevAttr.rightMgn;
    dlg.bottomMargin = prtrDevAttr.bottomMgn;

  GlobalUnlock(hDevMode);
  }


void DialogEx::getPrinterPageDlg(PrinterPageAttrDlg& dlg) {
  prtrDevAttr.orient    = dlg.orientation;
  prtrDevAttr.paperSz   = dlg.paperSize;
  prtrDevAttr.width     = dlg.pageWidth;
  prtrDevAttr.length    = dlg.pageLength;
  prtrDevAttr.topMgn    = dlg.topMargin;
  prtrDevAttr.leftMgn   = dlg.leftMargin;
  prtrDevAttr.rightMgn  = dlg.rightMargin;
  prtrDevAttr.bottomMgn = dlg.bottomMargin;
  }


void DialogEx::onDisplayPageSetup() { }


void DialogEx::setDefaultPrinter() {
CPrintDialog dlg(true, PrtrFlags, this);
PRINTDLG&    pd = dlg.m_pd;
Cstring      name;

  if (theDlgApp->isDfltPrtr()) return;

  dlg.GetDefaults();

  theDlgApp->setDevMode(pd.hDevMode);
  theDlgApp->setDevNames(pd.hDevNames);
  }


void dbg(TCchar* fmt, void* v) {
  String f = fmt;   f += _T(": %08x");
  String s;   s.format(f, v);   messageBox(s);
  }

