// About Dialog


#include "stdafx.h"
#include "About.h"
#include "ExtraResource.h"
#include "Resources.h"


CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD) { }


void CAboutDlg::DoDataExchange(CDataExchange* pDX) {CDialogEx::DoDataExchange(pDX);}


BOOL CAboutDlg::OnInitDialog() {
ResourceData res;
String       copyright;
String       companyName;
String       name;
String       aboutName;

  CDialogEx::OnInitDialog();

  if (!res.getProductName(aboutName)) aboutName = _T("Product");    aboutName = _T("About ") + aboutName;
  aboutName.trim();

  if (!res.getCompanyName(name)) name = _T("Bob -- K6RWY");   name.trim();
  companyName = _T("Author:  ") + name;

  // Copyright (c) Bob -- K6RWY.  All rights reserved.
  copyright = _T("Copyright (c) ") + name + _T(".  All rights reserved.");

  SetWindowText( aboutName);
  SetDlgItemText(IDC_NameVer,     res.getAboutNameVer());
  SetDlgItemText(IDC_Copyright,   copyright);
  SetDlgItemText(IDC_CompanyName, companyName);

  return TRUE;
  }

