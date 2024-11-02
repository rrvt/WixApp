// Site Update Dialog


#include "pch.h"
#include "UpdateDlg.h"
#include "SiteFileDscs.h"
#include "Utilities.h"


// SiteUpdateDlg dialog

IMPLEMENT_DYNAMIC(SiteUpdateDlg, CDialogEx)


SiteUpdateDlg::SiteUpdateDlg(SiteFileDscs& siteFileDscs, CWnd* pParent) :
                                           CDialogEx(IDD_SiteUpdate, pParent), fileDscs(siteFileDscs) { }

SiteUpdateDlg::~SiteUpdateDlg() { }


BOOL SiteUpdateDlg::OnInitDialog() {
FileDscsIter iter(fileDscs);
SiteFileDsc* siteFile;
int          cnt;
String       s;
int          index;
int          ht;
CRect        rect;

  CDialogEx::OnInitDialog();

  for (siteFile = iter(), cnt = 0; siteFile; siteFile = iter++) {
    switch (siteFile->status) {
      case WebPutSts:
      case DifPutSts: if (siteFile->key.dir) s = _T(" Create Web Dir:    ");
                      else                   s = _T(" Put File To Web:   ");
                      break;

      case GetSts   : if (siteFile->key.dir) s = _T(" Create Local Dir:  ");
                      else                   s = _T(" Get File From Web: ");
                      break;

      case DelSts   : if (siteFile->key.dir) s = _T(" Delete Web Dir:    ");
                      else                   s = _T(" Delete Web File:   ");
                      break;
      default       : continue;
      }

    s += toRemotePath(siteFile->key.path);

    index = listCtrl.AddString(s);   listCtrl.SetItemData(index, (DWORD_PTR) siteFile);    cnt++;
    }

  // Curious -- GetClientRect produces a vertical height that does not work well with the scroll bar
  // This code is used to produce a scroll bar and to adjust the scroll thumb and page size

  ht = listCtrl.GetItemHeight(0);   listCtrl.GetClientRect(rect);

  if (cnt * ht >= rect.Height() - 3*ht) {
    SCROLLINFO sbInfo = {sizeof(SCROLLINFO), SIF_ALL};

    listCtrl.ShowScrollBar(SB_VERT, TRUE);
    listCtrl.GetScrollInfo(SB_VERT, &sbInfo);   sbInfo.nPage -= 3;
    listCtrl.SetScrollInfo(SB_VERT, &sbInfo);
    }

  return TRUE;
  }


void SiteUpdateDlg::DoDataExchange(CDataExchange* pDX) {
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_UpdateList, listCtrl);
  }


BEGIN_MESSAGE_MAP(SiteUpdateDlg, CDialogEx)
  ON_BN_CLICKED(IDC_UpdateAll, &SiteUpdateDlg::OnUpdateAll)
END_MESSAGE_MAP()


// SiteUpdateDlg message handlers


void SiteUpdateDlg::OnOK() {
int       n = listCtrl.GetCount();
int       i;
SiteFileDsc* file;

  for (i = 0; i < n; i++) {

    file = (SiteFileDsc*) listCtrl.GetItemData(i);

    if (file) file->check = listCtrl.GetCheck(i) == BST_CHECKED;
    }

  CDialogEx::OnOK();
  }


void SiteUpdateDlg::OnUpdateAll() {
int          n = listCtrl.GetCount();
int          i;
SiteFileDsc* file;

  for (i = 0; i < n; i++) {

    file = (SiteFileDsc*) listCtrl.GetItemData(i);

    if (file) file->check = true;
    }

  CDialogEx::OnOK();
  }
