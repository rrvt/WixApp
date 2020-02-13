// SolutionPath.cpp : implementation file
//

#include "stdafx.h"
#include "WixApp.h"
#include "Solution.h"
#include "SolutionPath.h"
#include "afxdialogex.h"


// SolutionPath dialog

IMPLEMENT_DYNAMIC(SolutionPath, CDialogEx)


SolutionPath::SolutionPath(CWnd* pParent) : CDialogEx(SolutionPath::IDD, pParent),
explanation(_T("")), Expanation2(_T("")), solutionPath(_T("")) {}


SolutionPath::~SolutionPath() { }


void SolutionPath::DoDataExchange(CDataExchange* pDX) {

  CDialogEx::DoDataExchange(pDX);

  DDX_Text(pDX, IDC_EDIT1, explanation);
  DDX_Text(pDX, IDC_EDIT3, Expanation2);
  DDX_Text(pDX, IDC_EDIT2, solutionPath);
}


BEGIN_MESSAGE_MAP(SolutionPath, CDialogEx)
  ON_BN_CLICKED(IDC_BUTTON1, &SolutionPath::OnBnClickedButton1)
END_MESSAGE_MAP()


// SolutionPath message handlers


void SolutionPath::OnBnClickedButton1() {
  EndDialog(IDCANCEL); solution.clear();
  }
