#pragma once


// CalibDspPrtDlg dialog

class CalibDspPrtDlg : public CDialogEx {

  DECLARE_DYNAMIC(CalibDspPrtDlg)

public:

double  leftVal;
double  value;
double  rightVal;

int     calib;

CString calibInstr;
CString scaleValue;

int     procdr;

CStatic leftValueCtrl;
CEdit   scaleValueCtrl;
CStatic rightValueCtrl;

  CalibDspPrtDlg(CWnd* pParent = nullptr);   // standard constructor
  virtual ~CalibDspPrtDlg();

  virtual BOOL OnInitDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_Calibrate };
#endif

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()
public:

  afx_msg void OnSelectLeftleft();
  afx_msg void OnSelectLeftmid();
  afx_msg void OnBnClickedRightmid();
  afx_msg void OnSelectRightright();
  afx_msg void OnChangeScale();
  afx_msg void OnSelectDisplay();
  afx_msg void OnSelectPrinter();

private:

  void setValues();
public:
  afx_msg void OnOk();
};
