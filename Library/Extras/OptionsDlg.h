// Options Dialog Box


#pragma once
#include "ExtraResource.h"


extern TCchar* PortraitKey;
extern TCchar* LandscapeKey;


// OptionsDlg dialog

class OptionsDlg : public CDialogEx {

  DECLARE_DYNAMIC(OptionsDlg)

public:

CString   dspScale;

CComboBox orientCtrl;
CString   orient;

CString   topMargin;
CString   leftMargin;
CString   rightMargin;
CString   botMargin;
CEdit     dspSclCtrl;
CEdit     topCtrl;
CEdit     leftCtrl;
CEdit     rightCtrl;
CEdit     botCtrl;
CEdit     prtSclCtrl;

CString   prtScale;

  OptionsDlg(CWnd* pParent = nullptr);   // standard constructor
  virtual ~OptionsDlg();

  virtual BOOL OnInitDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_Options };
#endif

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()

private:

  void floatingOnly(CEdit& ctrl);

public:
  afx_msg void OnChangeTop();
  afx_msg void OnChangeLeft();
  afx_msg void OnChangeRight();
  afx_msg void OnChangeBot();

  afx_msg void OnChngDspScl();
  afx_msg void OnChngPrtScl();
  };
