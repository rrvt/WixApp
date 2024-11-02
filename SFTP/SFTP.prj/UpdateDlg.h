// Site Update Dialog -- Presents the files that the user may consider for updating


#pragma once
#include "Resource.h"

class SiteFileDscs;


// SiteUpdateDlg dialog

class SiteUpdateDlg : public CDialogEx {

  DECLARE_DYNAMIC(SiteUpdateDlg)

SiteFileDscs& fileDscs;

public:
CCheckListBox listCtrl;

               SiteUpdateDlg(SiteFileDscs& siteFileDscs, CWnd* pParent = nullptr);
  virtual     ~SiteUpdateDlg();

  virtual BOOL OnInitDialog();
  virtual void OnOK();

  afx_msg void OnUpdateAll();

// Dialog Data

#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_SiteUpdate };
#endif

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()
  };
