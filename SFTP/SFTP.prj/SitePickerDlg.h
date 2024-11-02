// Site Picker Dialog -- Presents a list of web sites the app knows and allows the user to pick one


#pragma once
#include "Resource.h"


// SitePickerDlg dialog

class SitePickerDlg : public CDialogEx {

  DECLARE_DYNAMIC(SitePickerDlg)

public:
CString   section;
CComboBox listCtrl;

           SitePickerDlg(CWnd* pParent = nullptr);          // standard constructor
  virtual ~SitePickerDlg();

  virtual BOOL OnInitDialog();

// Dialog Data

#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_SitePicker };
#endif

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()
  };
