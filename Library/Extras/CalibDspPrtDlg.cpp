// CalibDspPrtDlg.cpp : implementation file
//

#include "pch.h"
#include "CalibDspPrtDlg.h"
#include "CApp.h"
#include "ResourceExtra.h"
#include "IniFile.h"
#include "Resource.h"
#include "CScrView.h"
#include "Utilities.h"



static const TCchar* FontSection = _T("FontSection");
static const TCchar* DspLeftKey  = _T("DspLeft");
static const TCchar* DspRightKey = _T("DspRight");
static const TCchar* PrtLeftKey  = _T("PrtLeft");
static const TCchar* PrtRightKey = _T("PrtRight");

static const TCchar* expl0 = _T("There is a constant that govens the size of the fonts.  ")
            _T("It is different for the display and printer.  ")
            _T("The value reported in the edit box is used to set the scale factor.  ")
            _T("The 12.0 point Courier Font string output of 10 characters should be 1 inch in length.  ")
            _T("Use a ruler to measure the distance between the vertical bars.  ")
            _T("There are two versions of the character string.  ")
            _T("One is 80 characters and one is 100 characters.  ")
            _T("They should be 8 and 10 inches respectfully.")
            _T("Unfortunately the physical length of the string may not be precisely 8 or 10 inches.")
            _T("Try to get close and make the display and printer similar in length.  ")
            _T("Once the scale facter is chosen, the the setFontSize function and the stream modifiers, ")
            _T("nFSize and dFSize, will adjust the font size during execution of the program.");


// CalibDspPrtDlg dialog

IMPLEMENT_DYNAMIC(CalibDspPrtDlg, CDialogEx)


BEGIN_MESSAGE_MAP(CalibDspPrtDlg, CDialogEx)
  ON_BN_CLICKED(IDC_Display,    &OnSelectDisplay)
  ON_BN_CLICKED(IDC_Printer,    &OnSelectPrinter)
  ON_BN_CLICKED(IDC_LeftLeft,   &OnSelectLeftleft)
  ON_BN_CLICKED(IDC_LeftMid,    &OnSelectLeftmid)
  ON_BN_CLICKED(IDC_RightMid,   &OnBnClickedRightmid)
  ON_BN_CLICKED(IDC_RightRight, &OnSelectRightright)
  ON_EN_CHANGE( IDC_Scale,      &OnChangeScale)
  ON_BN_CLICKED(IDOK, &CalibDspPrtDlg::OnOk)
END_MESSAGE_MAP()


CalibDspPrtDlg::CalibDspPrtDlg(CWnd* pParent) : CDialogEx(IDD_Calibrate, pParent),
                                                leftVal(0.0), value(0.0), rightVal(0.0),
                                                calib(0), scaleValue(_T("")),  procdr(0),
                                                calibInstr(expl0) { }

CalibDspPrtDlg::~CalibDspPrtDlg() { }


BOOL CalibDspPrtDlg::OnInitDialog() {CDialogEx::OnInitDialog();   setValues();  return TRUE;}


void CalibDspPrtDlg::DoDataExchange(CDataExchange* pDX) {
  CDialogEx::DoDataExchange(pDX);
  DDX_Radio(pDX, IDC_Display, calib);
  DDX_Control(pDX, IDC_LeftValue, leftValueCtrl);
  DDX_Text(pDX, IDC_Scale, scaleValue);
  DDX_Control(pDX, IDC_Scale, scaleValueCtrl);
  DDX_Control(pDX, IDC_RightValue, rightValueCtrl);

  DDX_Radio(pDX, IDC_LeftLeft, procdr);
  DDX_Text(pDX, IDC_CalibInstr, calibInstr);
  }


// CalibDspPrtDlg message handlers


void CalibDspPrtDlg::OnSelectDisplay() {

  value    = cView()->getFontScale(false);
  leftVal  = iniFile.readInt(FontSection, DspLeftKey,  0);
  rightVal = iniFile.readInt(FontSection, DspRightKey, 0);

  if (leftVal == 0 && rightVal == 0) {leftVal  = value / 2.0;   rightVal = value * 2.0;}

  setValues();
  }



void CalibDspPrtDlg::OnSelectPrinter() {

  value    = cView()->getFontScale(true);
  leftVal  = iniFile.readInt(FontSection, PrtLeftKey,  0);
  rightVal = iniFile.readInt(FontSection, PrtRightKey, 0);

  if (leftVal == 0 && rightVal == 0) {leftVal  = value / 2.0;   rightVal = value * 2.0;}

  setValues();
  }


void CalibDspPrtDlg::OnChangeScale() {floatingOnly(scaleValueCtrl);}


void CalibDspPrtDlg::OnSelectLeftleft() {

  rightVal = value;
  value    = leftVal;
  leftVal  = value   / 1.3;
  setValues();
  }


void CalibDspPrtDlg::OnSelectLeftmid() {

  rightVal = value;
  value    = (leftVal + value) / 2.0;
  setValues();
  }


void CalibDspPrtDlg::OnBnClickedRightmid() {

  leftVal   = value;
  value     = (value + rightVal) / 2.0;
  setValues();
  }


void CalibDspPrtDlg::OnSelectRightright() {

  leftVal  = value;
  value    = rightVal;
  rightVal = value * 1.3;
  setValues();
  }


void CalibDspPrtDlg::setValues() {
String  lft = leftVal;
String  val = value;
String  rgt = rightVal;

  leftValueCtrl.SetWindowText(lft);
  scaleValueCtrl.SetWindowText(val);
  rightValueCtrl.SetWindowText(rgt);
  }




void CalibDspPrtDlg::OnOk() {
String s;
uint   x;

  CDialogEx::OnOK();

  s = scaleValue;   value = s.stod(x);

  if (calib == 0) {
    iniFile.writeInt(FontSection, DspLeftKey,  int(leftVal));
    iniFile.writeInt(FontSection, DspRightKey, int(rightVal));
    }
  else {
    iniFile.writeInt(FontSection, PrtLeftKey,  int(leftVal));
    iniFile.writeInt(FontSection, PrtRightKey, int(rightVal));
    }
  }
