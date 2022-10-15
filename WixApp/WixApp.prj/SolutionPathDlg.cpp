// SolutionPathDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SolutionPathDlg.h"
#include "GetPathDlg.h"
#include "Solution.h"


static TCchar* Explanation =  _T("WixApp will produce a wix file \"Product.wxs\" when the Create Button ")
                              _T("is selected.  The original Product.wxs file is renamed with a ")
                              _T("date/time appendage.  The new Product.wxs may then be compiled.\n\r")
                              _T("\n\r\n\r")
                              _T("The first step is to supply the path to the root of the project and ")
                              _T("to supply the path to the solution (\".sol\") file for the project.");


// SolutionPathDlg dialog

IMPLEMENT_DYNAMIC(SolutionPathDlg, CDialogEx)


BEGIN_MESSAGE_MAP(SolutionPathDlg, CDialogEx)
  ON_BN_CLICKED(IDC_RootBrowse,     &onRootBrowse)
  ON_BN_CLICKED(IDC_SolutionBrowse, &onSolutionBrowse)
  ON_BN_CLICKED(IDC_AbsolutePaths,  &onAbsolutePaths)
END_MESSAGE_MAP()


SolutionPathDlg::SolutionPathDlg(CWnd* pParent) : CDialogEx(SolutionPathDlg::IDD, pParent),
                                            rootPath(_T("")), solutionPath(_T("")), explanation(_T("")) {}


SolutionPathDlg::~SolutionPathDlg() { }


BOOL SolutionPathDlg::OnInitDialog()
                                {explanation = Explanation;    CDialogEx::OnInitDialog();   return TRUE;}


void SolutionPathDlg::DoDataExchange(CDataExchange* pDX) {
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(   pDX, IDC_Explanation,  explanation);
    DDX_Text(   pDX, IDC_RootPath,     rootPath);
    DDX_Text(   pDX, IDC_SolutionPath, solutionPath);
    DDX_Control(pDX, IDC_RootPath,     rootPathCtl);
    DDX_Control(pDX, IDC_SolutionPath, solutionPathCtl);
}



void SolutionPathDlg::onRootBrowse() {
String s;

  if (solution.getRootPath(s)) {rootPath = s; rootPathCtl.SetWindowText(rootPath);}
  }


void SolutionPathDlg::onSolutionBrowse() {
String s = rootPath + _T('\\');

  if (solution.getSolutionPath(s)) {solutionPath = s;  solutionPathCtl.SetWindowText(solutionPath);}
  }


void SolutionPathDlg::onAbsolutePaths() {EndDialog(IDC_AbsolutePaths);}
