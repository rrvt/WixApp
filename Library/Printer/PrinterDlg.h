// Printer Dialog Box -- i.e. Get Printer attributes including margins

#pragma once
#include "PrinterAttr.h"
#include "PrinterPgSz.h"



// PrinterDlg dialog

class PrinterDlg : public CDialogEx {

  DECLARE_DYNAMIC(PrinterDlg)

PrinterPgSz  pageSizeTbl;
PrinterInfo& info;

public:

int       prtrSelect{0};
int       orient{0};
Cstring   copies;
BOOL      collate{false};
Cstring   color;
int       colorItem{0};
Cstring   plex;
int       plexItem{0};
PaperSz   paperSz{NilPSz};
Cstring   sizeName;
Cstring   width;
Cstring   length;
Cstring   topMgn;
Cstring   leftMgn;
Cstring   rightMgn;
Cstring   bottomMgn;

CComboBox prtrSelectCtl;
CComboBox orientCtl;
CEdit     copiesCtl;
CButton   collateCtl;
CComboBox colorCtl;
CComboBox plexCtl;
CComboBox sizeCtl;
CEdit     widthCtl;
CEdit     lengthCtl;
CEdit     topMgnCtl;
CEdit     leftMgnCtl;
CEdit     rightMgnCtl;
CEdit     bottomMgnCtl;

               PrinterDlg(PrinterInfo& prtrInfo, CWnd* pParent = 0);
  virtual     ~PrinterDlg();

  virtual BOOL OnInitDialog();
  virtual void OnOK();

// Dialog Data
#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_PrtrDlg };
#endif

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()

private:

  void initPrtrCbo();
  void initOrientCbo();
  void initColorCbo();
  void initPlexCbo();

  void loadAttr();
  bool isFloatingPt(Cstring txt);

  void setSize();
//  bool validatePaperSz();

public:
  afx_msg void onPrtrselect();
//  afx_msg void onOrient();
  afx_msg void onSizeName();
  afx_msg void onLeaveWidth();
  afx_msg void onLeaveLength();
  afx_msg void onLeaveTopMgn();
  afx_msg void onLeaveLeftMgn();
  afx_msg void onLeaveRightMgn();
  afx_msg void onLeaveBottomMgn();
  };
