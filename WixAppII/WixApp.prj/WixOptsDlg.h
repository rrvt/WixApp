// Wix OptionDlg Dialog Box


#pragma once
#include "RelPath.h"
#include "Resource.h"
#include "afxwin.h"


extern TCchar* DefLicensePathKey;


// WixOptsDlg dialog

class WixOptsDlg : public CDialogEx {

  DECLARE_DYNAMIC(WixOptsDlg)

public:
enum { IDD = IDD_WixOpts };

BOOL    allowSameVer;
CButton addLicenseCH;
CEdit   licensePathEB;
bool    addLic;
RelPath licPath;

  WixOptsDlg(CWnd* pParent = NULL);   // standard constructor
  virtual ~WixOptsDlg();

  afx_msg void OnAddLicense();

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()
public:
  virtual BOOL OnInitDialog();
  };
