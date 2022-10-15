// Select root and solution paths


#pragma once
#include "resource.h"


class SolutionPathDlg : public CDialogEx {

  DECLARE_DYNAMIC(SolutionPathDlg)

public:
CString explanation;
CString rootPath;
CString solutionPath;

CEdit   rootPathCtl;
CEdit   solutionPathCtl;


               SolutionPathDlg(CWnd* pParent = NULL);   // standard constructor
  virtual     ~SolutionPathDlg();

  virtual BOOL OnInitDialog();

// Dialog Data
  enum { IDD = IDD_SolutionPath };

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()

public:
  afx_msg void onSolutionBrowse();
  afx_msg void onRootBrowse();
  afx_msg void onAbsolutePaths();
  };
