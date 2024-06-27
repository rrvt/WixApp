// WixOptsDlg.cpp : implementation file
//

#include "pch.h"
#include "WixOptsDlg.h"
#include "DefaultPath.h"
#include "WixApp.h"


TCchar* DefLicensePathKey = _T("License");


static KeyedPathDsc browseDsc =
                              {DefLicensePathKey, _T("License RTF file"), _T(""), _T("*"), _T("")};

// WixOptsDlg dialog

IMPLEMENT_DYNAMIC(WixOptsDlg, CDialogEx)

WixOptsDlg::WixOptsDlg(CWnd* pParent) : CDialogEx(WixOptsDlg::IDD, pParent),
                                                              allowSameVer(FALSE), addLic(false) {}

WixOptsDlg::~WixOptsDlg() { }


void WixOptsDlg::DoDataExchange(CDataExchange* pDX) {
  CDialogEx::DoDataExchange(pDX);
  DDX_Check(  pDX, IDC_AllowSameVer, allowSameVer);
  DDX_Control(pDX, IDC_AddLicense,   addLicenseCH);
  DDX_Control(pDX, IDC_License,      licensePathEB);
  }


BEGIN_MESSAGE_MAP(WixOptsDlg, CDialogEx)
  ON_BN_CLICKED(IDC_AddLicense, &WixOptsDlg::OnAddLicense)
END_MESSAGE_MAP()




BOOL WixOptsDlg::OnInitDialog() {
  CDialogEx::OnInitDialog();

  addLicenseCH.SetCheck(addLic);

  licensePathEB.SetWindowText(String(licPath));

  return TRUE;
  }


// WixOptsDlg message handlers


void WixOptsDlg::OnAddLicense() {

  addLic = addLicenseCH.GetCheck()  != 0;

  if (addLic) {

    licPath.getPath(browseDsc);

    licensePathEB.SetWindowText(String(licPath));
    }

  else {licPath.clear();  licensePathEB.SetWindowText(_T(""));}
  }



