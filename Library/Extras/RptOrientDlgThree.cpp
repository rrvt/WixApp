// RptOrietnDlg.cpp : implementation file
//

#include "pch.h"
#include "RptOrientDlgOne.h"


IMPLEMENT_DYNAMIC(RptOrietnDlg, CDialogEx)

RptOrietnDlg::RptOrietnDlg(CWnd* pParent) :
                                        CDialogEx(IDD_Orientation, pParent), ntpd(_T("")), rpt1(_T("")) { }

RptOrietnDlg::~RptOrietnDlg() { }


BOOL RptOrietnDlg::OnInitDialog() {
CWnd* label;

  CDialogEx::OnInitDialog();

  if (!lbl00.isEmpty()) {label = GetDlgItem(IDC_RptLabel0);   label->SetWindowText(lbl00);}
  label = GetDlgItem(IDC_RptLabel1);   label->SetWindowText(Lbl01);
  label = GetDlgItem(IDC_RptLabel2);   label->SetWindowText(Lbl02);

  initOrietn(ntpdCtl, ntpd);
  initOrietn(rpt1Ctl, rpt1);
  initOrietn(rpt2Ctl, rpt2);

  return TRUE;
  }


void RptOrietnDlg::initOrietn(CComboBox& ctl, Cstring& orietn) {
  ctl.AddString(LandscapeKey);
  ctl.AddString(PortraitKey);
  ctl.SelectString(-1, orietn);
  }


void RptOrietnDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Control( pDX, IDC_NtpdOrietn, ntpdCtl);
  DDX_CBString(pDX, IDC_NtpdOrietn, ntpd);
  DDX_Control( pDX, IDC_RptOrietn1,  rpt1Ctl);
  DDX_CBString(pDX, IDC_RptOrietn1,  rpt1);
  DDX_Control( pDX, IDC_RptOrietn2,  rpt2Ctl);
  DDX_CBString(pDX, IDC_RptOrietn2,  rpt2);
  }


BEGIN_MESSAGE_MAP(RptOrietnDlg, CDialogEx)
END_MESSAGE_MAP()


