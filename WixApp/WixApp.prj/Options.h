// Options Dialog Box


#pragma once
//#include "afxwin.h"
#include "PathDesc.h"
#include "Resource.h"
#include "afxwin.h"


extern TCchar* DefLicensePathKey;


// Options dialog

class Options : public CDialogEx {

  DECLARE_DYNAMIC(Options)

public:
enum { IDD = IDD_Options };                    // Dialog Data

BOOL     allowSameVer;
CButton  addLicenseCH;
CEdit    licensePathEB;
bool     addLic;
PathDesc licPath;

  Options(CWnd* pParent = NULL);   // standard constructor
  virtual ~Options();

  afx_msg void OnAddLicense();

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()
public:
  virtual BOOL OnInitDialog();
};
