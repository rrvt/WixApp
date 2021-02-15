// About Dialog


#pragma once
#include "resource.h"


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx {
public:

enum { IDD = IDD_ABOUTBOX };     // Dialog Data

               CAboutDlg();
  virtual BOOL OnInitDialog();

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  };

