// NamePswdDlg.cpp : implementation file
//

#include "pch.h"
#include "NamePswdDlg.h"
#include "ResourceExtra.h"
#include "Random.h"


// NamePswdDlg dialog

IMPLEMENT_DYNAMIC(NamePswdDlg, CDialogEx)

NamePswdDlg::NamePswdDlg(CWnd* pParent) : CDialogEx(IDD_NamePswd, pParent), pswd(_T("")) { }


NamePswdDlg::~NamePswdDlg() { }


BOOL NamePswdDlg::OnInitDialog() {

  CDialogEx::OnInitDialog();
                                              //   IDD_NamePswd
  return TRUE;                                //
  }                                           //   IDC_NPName
                                              //   IDC_NPPswd

void NamePswdDlg::DoDataExchange(CDataExchange* pDX) {
  CDialogEx::DoDataExchange(pDX);
  DDX_Text(   pDX, IDC_NPName, name);
  DDX_Control(pDX, IDC_NPPswd, pswdCtl);
  DDX_Text(   pDX, IDC_NPPswd, pswd);
  DDX_Control(pDX, IDC_NPName, nameCtl);
  }


BEGIN_MESSAGE_MAP(NamePswdDlg, CDialogEx)
  ON_EN_KILLFOCUS(IDC_NPName, &NamePswdDlg::OnKillfocusName)
  ON_EN_KILLFOCUS(IDC_NPPswd, &NamePswdDlg::OnKillfocusPswd)
END_MESSAGE_MAP()


void NamePswdDlg::OnOK() {
  if (!nameCtl.LineLength()) return;

  if (pswdCtl.LineLength() < 8) return;

  CDialogEx::OnOK();
  }


void NamePswdDlg::OnKillfocusName() {if (!nameCtl.LineLength()) nameCtl.SetFocus();}


void NamePswdDlg::OnKillfocusPswd() {if (pswdCtl.LineLength() < 8) pswdCtl.SetFocus();}

#if 0
void NamePswdDlg::expunge() {
int    n = pswd.length();
int    i;
Random rnd;

  for (i = 0; i < n; i++) {pswd.Insert(i, (Tchar) (256 * rnd.next()));}
  }
#endif
