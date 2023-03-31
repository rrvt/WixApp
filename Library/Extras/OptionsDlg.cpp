// OptionsDlg.cpp : implementation file
//

#include "pch.h"
#include "OptionsDlg.h"
#include "Display.h"
#include "Printer.h"
#include <Winspool.h>


static const ulong EnumFlags = PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS;
static const ulong DscType   = 4;



// OptionsDlg dialog

IMPLEMENT_DYNAMIC(OptionsDlg, CDialogEx)


BEGIN_MESSAGE_MAP(OptionsDlg, CDialogEx)
  ON_CBN_SELCHANGE(IDC_PrinterName, &onSelectPrinter)
  ON_EN_CHANGE(    IDC_TopMargin,   &OnChangeTop)
  ON_EN_CHANGE(    IDC_BotMargin,   &OnChangeBot)
  ON_EN_CHANGE(    IDC_LeftOdd,     &OnChangeLeftOdd)
  ON_EN_CHANGE(    IDC_RightOdd,    &OnChangeRightOdd)
  ON_EN_CHANGE(    IDC_LeftEven,    &OnChangeLeftEven)
  ON_EN_CHANGE(    IDC_RightEven,   &OnChangeRightEven)
  ON_EN_CHANGE(    IDC_DspScale,    &OnChngDspScl)
  ON_EN_CHANGE(    IDC_PrtScale,    &OnChngPrtScl)
END_MESSAGE_MAP()


OptionsDlg::OptionsDlg(CWnd* pParent) : CDialogEx(IDD_Options, pParent),
     topMargin(_T("")), leftOdd(_T("")), rightOdd(_T("")), botMargin(_T("")),
     dspScale(_T("")), prtScale(_T("")), printerName(_T("")) { }


OptionsDlg::~OptionsDlg() { }


BOOL OptionsDlg::OnInitDialog() {

  dspScale    = toStg(display.scale);

  printerName = printer.name;
  topMargin   = printer.topMargin;
  botMargin   = printer.botMargin;
  leftOdd     = printer.leftOdd;
  rightOdd    = printer.rightOdd;
  leftEven    = printer.leftEven;
  rightEven   = printer.rightEven;

  prtScale    = toStg(printer.scale);

  CDialogEx::OnInitDialog();

  printers();

//  orientCtrl.AddString(PortraitKey);
//  orientCtrl.AddString(LandscapeKey);

  return TRUE;
  }


void OptionsDlg::printers() {
ulong  nBuf      = 0;
ulong  nPrinters = 0;
Byte*  buf;
String name;

  if (!EnumPrinters(EnumFlags, NULL, DscType, NULL, 0, &nBuf, &nPrinters) &&
                                                      GetLastError() != ERROR_INSUFFICIENT_BUFFER) return;
  buf = new Byte[nBuf];   if (buf == NULL) return;

    // Fill the buf
    // Again, this depends on the O/S

    if (!EnumPrinters(EnumFlags, NULL, DscType, buf, nBuf, &nBuf, &nPrinters)) {delete [] buf; return;}

    if (nPrinters == 0) {delete [] buf; return;}

    PRINTER_INFO_4* pInfo = (PRINTER_INFO_4*) buf;

    for (DWORD i = 0; i < nPrinters; i++, pInfo++) {
      if (pInfo->Attributes & PRINTER_ATTRIBUTE_FAX) continue;

      printerNameCtrl.AddString(pInfo->pPrinterName);
      }

    printerNameCtrl.SelectString(-1, printerName);

  delete [] buf;
  }


void OptionsDlg::onSelectPrinter() {
Cstring newPrntr;
String  s;

  printerNameCtrl.GetLBText(printerNameCtrl.GetCurSel(), newPrntr);

  if (!printerName.isEmpty() && printerName != newPrntr) printer.store();

  printer.load(newPrntr);
//  orient      = printer.toStg(printer.orient);
//  orientCtrl.SelectString(-1, orient);
  topMargin   = printer.topMargin;              topCtrl.SetWindowText(topMargin);
  botMargin   = printer.botMargin;              botCtrl.SetWindowText(botMargin);
  leftOdd     = printer.leftOdd;                leftOddCtrl.SetWindowText(leftOdd);
  rightOdd    = printer.rightOdd;               rightOddCtrl.SetWindowText(rightOdd);
  leftEven    = printer.leftEven;               leftEvenCtrl.SetWindowText(leftEven);
  rightEven   = printer.rightEven;              rightEvenCtrl.SetWindowText(rightEven);

  prtScale    = toStg(printer.scale);           prtSclCtrl.SetWindowText(prtScale);
  }


void OptionsDlg::DoDataExchange(CDataExchange* pDX) {
  CDialogEx::DoDataExchange(pDX);

  DDX_Text(    pDX, IDC_DspScale,    dspScale);

  DDX_Control( pDX, IDC_PrinterName, printerNameCtrl);
  DDX_CBString(pDX, IDC_PrinterName, printerName);

//  DDX_Control( pDX, IDC_Orientation, orientCtrl);
//  DDX_CBString(pDX, IDC_Orientation, orient);

  DDX_Text(    pDX, IDC_TopMargin,   topMargin);
  DDX_Text(    pDX, IDC_BotMargin,   botMargin);

  DDX_Text(    pDX, IDC_LeftOdd,     leftOdd);
  DDX_Text(    pDX, IDC_RightOdd,    rightOdd);

  DDX_Text(    pDX, IDC_LeftEven,    leftEven);
  DDX_Text(    pDX, IDC_RightEven,   rightEven);

  DDX_Text(    pDX, IDC_PrtScale,    prtScale);

  DDX_Control( pDX, IDC_DspScale,    dspSclCtrl);
  DDX_Control( pDX, IDC_TopMargin,   topCtrl);
  DDX_Control( pDX, IDC_BotMargin,   botCtrl);

  DDX_Control( pDX, IDC_LeftOdd,     leftOddCtrl);
  DDX_Control( pDX, IDC_RightOdd,    rightOddCtrl);

  DDX_Control( pDX, IDC_LeftEven,    leftEvenCtrl);
  DDX_Control( pDX, IDC_RightEven,   rightEvenCtrl);

  DDX_Control( pDX, IDC_PrtScale,    prtSclCtrl);
  }


void OptionsDlg::OnChangeTop()       {floatingOnly(topCtrl);}
void OptionsDlg::OnChangeBot()       {floatingOnly(botCtrl);}
void OptionsDlg::OnChangeLeftOdd()   {floatingOnly(leftOddCtrl);}
void OptionsDlg::OnChangeRightOdd()  {floatingOnly(rightOddCtrl);}
void OptionsDlg::OnChangeLeftEven()  {floatingOnly(leftEvenCtrl);}
void OptionsDlg::OnChangeRightEven() {floatingOnly(rightEvenCtrl);}
void OptionsDlg::OnChngDspScl()      {floatingOnly(dspSclCtrl);}
void OptionsDlg::OnChngPrtScl()      {floatingOnly(prtSclCtrl);}


void OptionsDlg::floatingOnly(CEdit& ctrl) {
CString cs;
String  s;
int     n;
int     i;
bool    periodSeen = false;

  ctrl.GetWindowText(cs);   s = cs;

  for (i = 0, n = s.length(); i < n; i++) {
    Tchar ch = s[i];

    if (_T('0') <= ch && ch <= _T('9')) continue;

    if (ch == _T('.') && !periodSeen) {periodSeen = true; continue;}

    Beep(1500, 120);   ctrl.SetSel(i, i+1);   ctrl.ReplaceSel(_T(""));
    }
  }


void OptionsDlg::OnOK() {

  CDialogEx::OnOK();

  display.scale     = toDbl(dspScale);    display.store();

  printer.name      = printerName;
//  printer.orient    = printer.toOrient(orient);
  printer.topMargin = toDbl(topMargin);
  printer.botMargin = toDbl(botMargin);
  printer.leftOdd   = toDbl(leftOdd);
  printer.rightOdd  = toDbl(rightOdd);
  printer.leftEven  = toDbl(leftEven);
  printer.rightEven = toDbl(rightEven);
  printer.scale     = toDbl(prtScale);   printer.store();
  }


double OptionsDlg::toDbl(Cstring& cs) {
String s = cs;
uint   x = 0;
double v = s.stod(x);

  return v;
  }


String OptionsDlg::toStg(double v) {String s = v;   return s;}


