#pragma once
#include "afxwin.h"


// Finish dialog

class Finish : public CDialogEx {
  DECLARE_DYNAMIC(Finish)

public:
CString finalMsg;

  Finish(CWnd* pParent = NULL);   // standard constructor

  virtual ~Finish();

// Dialog Data
  enum { IDD = IDD_Finish };

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()
  };
