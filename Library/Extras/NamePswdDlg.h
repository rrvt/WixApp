// Name and Password Dialog Box


#pragma once
#include "Utilities.h"


// NamePswdDlg dialog

class NamePswdDlg : public CDialogEx {

  DECLARE_DYNAMIC(NamePswdDlg)

Cstring name;
CEdit   nameCtl;

Cstring pswd;
CEdit   pswdCtl;

public:
           NamePswdDlg(CWnd* pParent = nullptr);   // standard constructor
  virtual ~NamePswdDlg();


  virtual BOOL OnInitDialog();
  virtual void OnOK();
          void expunge() {::expunge(pswd);}

// Dialog Data
#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_NamePswd };
#endif

protected:

  virtual void DoDataExchange(CDataExchange* pDX);

public:

  DECLARE_MESSAGE_MAP()public:
  afx_msg void OnKillfocusName();
  afx_msg void OnKillfocusPswd();
  };
