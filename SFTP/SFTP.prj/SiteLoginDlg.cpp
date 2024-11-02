// Web Login Dialog Box


#include "pch.h"
#include "SiteLoginDlg.h"
#include "Resource.h"
#include "GetPathDlg.h"
#include "Utilities.h"

IMPLEMENT_DYNAMIC(SiteLoginDlg, CDialogEx)


BEGIN_MESSAGE_MAP(SiteLoginDlg, CDialogEx)
    ON_BN_CLICKED(IDC_LclRootBtn, &SiteLoginDlg::OnClickedLclrootbtn)
END_MESSAGE_MAP()


SiteLoginDlg::SiteLoginDlg(CWnd* pParent) : CDialogEx(IDD_WebLogin, pParent), urlName(_T("")),
                                                                     userID(_T("")), password(_T("")), remoteRoot(_T(""))
{ }

SiteLoginDlg::~SiteLoginDlg() { }


BOOL SiteLoginDlg::OnInitDialog() {
  CDialogEx::OnInitDialog();

  localRoot.SetWindowText(rootTxt);

  return TRUE;
  }


void SiteLoginDlg::DoDataExchange(CDataExchange* pDX) {
  CDialogEx::DoDataExchange(pDX);
  DDX_Text(   pDX, IDC_Name,       name);
  DDX_Control(pDX, IDC_LclRoot,    localRoot);
  DDX_Control(pDX, IDC_LclRootBtn, localRootBtn);
  DDX_Text(   pDX, IDC_URLName,    urlName);
  DDX_Text(   pDX, IDC_UserID,     userID);
  DDX_Text(   pDX, IDC_Password,   password);
  DDX_Text(   pDX, IDC_RemoteRoot, remoteRoot);
  }


void SiteLoginDlg::OnClickedLclrootbtn() {
String path;

  getDirPathDlg(_T("Local Root Path"), path);   fixLocalPath(path);

  rootTxt = path;  localRoot.SetWindowText(rootTxt);
  }


void SiteLoginDlg::OnOK() {

  localRoot.GetWindowText(rootTxt);

  CDialogEx::OnOK();
  }
