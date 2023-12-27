// Printer Dialog Box


#pragma once
#include "PrintMgr.h"
#include "ResourceExtra.h"


// OptionsDlg dialog

class OptionsDlg : public CDialogEx {

  DECLARE_DYNAMIC(OptionsDlg)

public:

Cstring   dspScale;
CEdit     dspSclCtrl;

CComboBox printerNameCtrl;
Cstring   printerName;

Cstring   topMargin;
Cstring   botMargin;
Cstring   leftOdd;
Cstring   rightOdd;
Cstring   leftEven;
Cstring   rightEven;

Cstring   prtScale;

CEdit     topCtrl;
CEdit     botCtrl;
CEdit     leftOddCtrl;
CEdit     rightOddCtrl;
CEdit     leftEvenCtrl;
CEdit     rightEvenCtrl;
CEdit     prtSclCtrl;


  OptionsDlg(CWnd* pParent = nullptr);   // standard constructor
  virtual ~OptionsDlg();

  virtual BOOL OnInitDialog();
  virtual void OnOK();

// Dialog Data
#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_Options };
#endif

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()

private:

  void       floatingOnly(CEdit& ctrl);
  void       printers();
  double     toDbl(Cstring& cs);
  String     toStg(double v);

public:
  afx_msg void onSelectPrinter();
  afx_msg void OnChangeTop();
  afx_msg void OnChangeBot();
  afx_msg void OnChangeLeftOdd();
  afx_msg void OnChangeRightOdd();
  afx_msg void OnChangeLeftEven();
  afx_msg void OnChangeRightEven();

  afx_msg void OnChngDspScl();
  afx_msg void OnChngPrtScl();
  };
