// RequiredOSs.cpp : implementation file
//

#include "stdafx.h"
#include "WixApp.h"
#include "RequiredOSs.h"
#include "afxdialogex.h"


// RequiredOSs dialog

IMPLEMENT_DYNAMIC(RequiredOSs, CDialogEx)


RequiredOSs::RequiredOSs(CWnd* pParent) : CDialogEx(RequiredOSs::IDD, pParent), nBase(0) {
  base[0] = {_T("Win10"), _T("603")}; nBase++;
  base[1] = {_T("Win7"),  _T("601")}; nBase++;
  base[2] = {_T("WinXP"), _T("501")}; nBase++;
  base[3] = {_T("Win2K"), _T("500")}; nBase++;
  }


RequiredOSs::~RequiredOSs() {
  }




BOOL RequiredOSs::OnInitDialog() {
int i;

  CDialogEx::OnInitDialog();

  for (i = 0; i < nBase; i++) {reqOSListCtrl.AddString(base[i].os);}

  return TRUE;  // return TRUE unless you set the focus to a control
  }


void RequiredOSs::DoDataExchange(CDataExchange* pDX) {

  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_LIST1, reqOSListCtrl);
  }


BEGIN_MESSAGE_MAP(RequiredOSs, CDialogEx)
  ON_BN_CLICKED(IDOK, &RequiredOSs::OnBnClickedOk)
  ON_BN_CLICKED(IDCANCEL, &RequiredOSs::OnBnClickedCancel)
END_MESSAGE_MAP()


// RequiredOSs message handlers


void RequiredOSs::OnBnClickedOk() {
int      nTgts;
int      tgts[5];

  CDialogEx::OnOK();   nTgts = reqOSListCtrl.GetSelItems(noElements(tgts), tgts);

  line = _T("<![CDATA[Installed OR ");

  TgtValue& firstTgt = base[tgts[0]];
  TgtValue& lastTgt  = base[tgts[nTgts-1]];

  if (!nTgts) {line.clear(); return;}

  line += _T("VersionNT >= ") + lastTgt.val;

  if (tgts[0] != 0) line += _T(" AND VersionNT < ") + base[tgts[0]-1].val;

  line += _T("]]>");
  }


// <![CDATA[Installed OR VersionNT >= 601]]>


void RequiredOSs::OnBnClickedCancel() {
  // TODO: Add your control notification handler code here
  CDialogEx::OnCancel();
}
