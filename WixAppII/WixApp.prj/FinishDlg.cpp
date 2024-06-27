// FinishDlg.cpp : implementation file


#include "pch.h"
#include "FinishDlg.h"


// Finish dialog

IMPLEMENT_DYNAMIC(FinishDlg, CDialogEx)


FinishDlg::FinishDlg(CWnd* pParent) : CDialogEx(FinishDlg::IDD, pParent) {
  finalMsg =
    "A new version of Product.wxs is created by this program.  One additional file "
    "(My_en-us.wxl) is copied into the directory where Product.wxs resides if not already "
    "there.\r\n\r\n"
    "To specify the output directory of the Installer.msi file select the properties dialog of "
    "the installer project, then select the Build tab (on left hand side of window) and "
    "enter: \r\n\r\n"
    "   \"$(SolutionDir)..\\$(Configuration)\\\"\r\n\r\n"
    "into the Output Path edit box."
    ;
  }


FinishDlg::~FinishDlg() { }


void FinishDlg::DoDataExchange(CDataExchange* pDX) {
  CDialogEx::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_FinalMsg, finalMsg);
  }


BEGIN_MESSAGE_MAP(FinishDlg, CDialogEx)
END_MESSAGE_MAP()

