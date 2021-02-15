// Finish.cpp : implementation file
//

#include "stdafx.h"
#include "WixApp.h"
#include "Finish.h"
#include "afxdialogex.h"


// Finish dialog

IMPLEMENT_DYNAMIC(Finish, CDialogEx)


Finish::Finish(CWnd* pParent) : CDialogEx(Finish::IDD, pParent) {
  finalMsg =
    "A new version of Product.wxs is created by this program.  One additional file "
    "(My_en-us.wxl) is copied into the directory where Product.wxs resides if not already there.\r\n\r\n"
    "To specify the output directory of the Installer.msi file select the properties dialog of the "
    "installer project, then select the Build tab (on left hand side of window) and enter: \r\n\r\n"
    "   \"$(SolutionDir)..\\$(Configuration)\\\"\r\n\r\n"
    "into the Output Path edit box."
    ;
  }


Finish::~Finish() { }


void Finish::DoDataExchange(CDataExchange* pDX) {
  CDialogEx::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_FinalMsg, finalMsg);
}


BEGIN_MESSAGE_MAP(Finish, CDialogEx)
END_MESSAGE_MAP()


// Finish message handlers
