#pragma once
#include "afxwin.h"
#include "Resource.h"

struct TgtValue {
String  os;
String  val;

  TgtValue() {}
  TgtValue(TCchar* osName, TCchar* tgtVal) : os(osName), val(tgtVal) {}
  };


// RequiredOSs dialog

class RequiredOSs : public CDialogEx {

  DECLARE_DYNAMIC(RequiredOSs)

public:

enum { IDD = IDD_SelectOSs };                         // Dialog Data

CListBox reqOSListCtrl;
int      nBase;
TgtValue base[5];
String   line;

  RequiredOSs(CWnd* pParent = NULL);                  // standard constructor

  virtual ~RequiredOSs();

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  virtual BOOL OnInitDialog();
};
