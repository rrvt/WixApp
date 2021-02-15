// OptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OptionsDlg.h"



TCchar* PortraitKey  = _T("Portrait");
TCchar* LandscapeKey = _T("Landscape");



// OptionsDlg dialog

IMPLEMENT_DYNAMIC(OptionsDlg, CDialogEx)


BEGIN_MESSAGE_MAP(OptionsDlg, CDialogEx)
  ON_EN_CHANGE(IDC_TopMargin,   &OptionsDlg::OnChangeTop)
  ON_EN_CHANGE(IDC_LeftMargin,  &OptionsDlg::OnChangeLeft)
  ON_EN_CHANGE(IDC_RightMargin, &OptionsDlg::OnChangeRight)
  ON_EN_CHANGE(IDC_BotMargin,   &OptionsDlg::OnChangeBot)
END_MESSAGE_MAP()


OptionsDlg::OptionsDlg(CWnd* pParent) : CDialogEx(IDD_Options, pParent),
     topMargin(_T("")), leftMargin(_T("")), rightMargin(_T("")), botMargin(_T("")), orient(_T("")),
     dspScale(_T("")), prtScale(_T("")) { }


OptionsDlg::~OptionsDlg() { }


BOOL OptionsDlg::OnInitDialog() {
  CDialogEx::OnInitDialog();

  orientCtrl.AddString(PortraitKey);
  orientCtrl.AddString(LandscapeKey);

  return TRUE;
  }


void OptionsDlg::DoDataExchange(CDataExchange* pDX) {
  CDialogEx::DoDataExchange(pDX);
  DDX_Text(    pDX, IDC_DspScale,    dspScale);
  DDX_Control( pDX, IDC_Orientation, orientCtrl);
  DDX_CBString(pDX, IDC_Orientation, orient);
  DDX_Text(    pDX, IDC_TopMargin,   topMargin);
  DDX_Text(    pDX, IDC_LeftMargin,  leftMargin);
  DDX_Text(    pDX, IDC_RightMargin, rightMargin);
  DDX_Text(    pDX, IDC_BotMargin,   botMargin);
  DDX_Text(    pDX, IDC_PrtScale,    prtScale);
  DDX_Control( pDX, IDC_DspScale,    dspSclCtrl);
  DDX_Control( pDX, IDC_TopMargin,   topCtrl);
  DDX_Control( pDX, IDC_LeftMargin,  leftCtrl);
  DDX_Control( pDX, IDC_RightMargin, rightCtrl);
  DDX_Control( pDX, IDC_BotMargin,   botCtrl);
  DDX_Control( pDX, IDC_PrtScale,    prtSclCtrl);
  }


void OptionsDlg::OnChangeTop()   {floatingOnly(topCtrl);}
void OptionsDlg::OnChangeLeft()  {floatingOnly(leftCtrl);}
void OptionsDlg::OnChangeRight() {floatingOnly(rightCtrl);}
void OptionsDlg::OnChangeBot()   {floatingOnly(botCtrl);}
void OptionsDlg::OnChngDspScl()  {floatingOnly(dspSclCtrl);}
void OptionsDlg::OnChngPrtScl()  {floatingOnly(prtSclCtrl);}


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

