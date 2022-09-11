#pragma once


// SolutionPath dialog

class SolutionPath : public CDialogEx {

  DECLARE_DYNAMIC(SolutionPath)

public:

CString solutionPath;
CString explanation;
CString Expanation2;

  SolutionPath(CWnd* pParent = NULL);   // standard constructor
  virtual ~SolutionPath();

// Dialog Data
  enum { IDD = IDD_SelectSolutionPath };

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedButton1();
  };
