// RptOrietnDlg.cpp : implementation file
//

#include "pch.h"
#include "RptOrientDlgOne.h"


IMPLEMENT_DYNAMIC(RptOrietnDlg, CDialogEx)

RptOrietnDlg::RptOrietnDlg(CWnd* pParent) : CDialogEx(IDD_Orientation, pParent) { }

RptOrietnDlg::~RptOrietnDlg() { }


BOOL RptOrietnDlg::OnInitDialog() {
CWnd* label;

  CDialogEx::OnInitDialog();

  if (!lbl00.isEmpty()) {label = GetDlgItem(IDC_RptLabel0);   label->SetWindowText(lbl00);}

  initOrietn(ntpdCtl, ntpd);   return TRUE;
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
  }


BEGIN_MESSAGE_MAP(RptOrietnDlg, CDialogEx)
END_MESSAGE_MAP()


