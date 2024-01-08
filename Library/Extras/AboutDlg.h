// About Dialog


#pragma once
#include "Resource.h"
#include "ResourceExtra.h"


// AboutDlg dialog used for App About

class AboutDlg : public CDialogEx {
bool useProdName;
public:

enum { IDD = IDD_AboutBox };     // Dialog Data

  AboutDlg(bool useInternalName) : useProdName(!useInternalName), CDialogEx(AboutDlg::IDD) { }
  AboutDlg()                     : useProdName(true),             CDialogEx(AboutDlg::IDD) { }

  virtual BOOL OnInitDialog();

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
public:
    DECLARE_MESSAGE_MAP()
    afx_msg void onOk();
};

