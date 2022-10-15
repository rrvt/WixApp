// Finish dialog


#pragma once
#include "resource.h"

class FinishDlg : public CDialogEx {

  DECLARE_DYNAMIC(FinishDlg)

public:

CString finalMsg;

  FinishDlg(CWnd* pParent = NULL);   // standard constructor

  virtual ~FinishDlg();

// Dialog Data
  enum { IDD = IDD_Finish };

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()
  };
