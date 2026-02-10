// Main Dialog Box


#pragma once
#include "ComboBox.h"
#include "FinishDlg.h"
#include "Resource.h"
#include "StatusBar.h"
#include "MyToolBar.h"
#include "WinPos.h"

class StatusBar;


// WixAppDlg dialog

class WixAppDlg : public CDialogEx {

DECLARE_DYNAMIC(WixAppDlg)

String    helpPath;

CRect     winRect;
MyToolBar toolBar;
StatusBar statusBar;

bool      isInitialized;

public:

CEdit     productNameEB;
CEdit     companyEB;
CEdit     wixNameEB;
CEdit     wixVersionEB;
ComboBox  prodIconCB;

ComboBox  featureCB;
CEdit     progFileEB;
CEdit     startMenuEB;

ComboBox  componentCB;
CEdit     descriptionEB;
CEdit     pathEB;

CButton   isStartMenu;
CButton   isDeskTop;
CButton   isVersionAvail;
CButton   isOnPathCH;

CButton   isStartupApp;

CButton   isWin10ch;
CButton   isWin7ch;
CButton   isWinXPch;

ComboBox  iconCB;

               WixAppDlg(TCchar* helpPth, CWnd* pParent = nullptr);
  virtual     ~WixAppDlg() {winPos.~WinPos();}

  virtual BOOL OnInitDialog();

// Dialog Data

#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_WixDlg };
#endif

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()

private:

  void     setupToolBar();
  void     updateIconCB();
  void     updateCtrlPanelIconCB();

public:

  afx_msg int     OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg LRESULT OnResetToolBar(WPARAM wParam, LPARAM lParam);
  afx_msg BOOL    OnTtnNeedText(UINT id, NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void    OnMove(int x, int y);

  // Commands
  afx_msg void    onNewProject();
  afx_msg void    onOpenProject();
  afx_msg void    onSaveAllFiles();
  afx_msg void    onSaveWXDFile();
  afx_msg void    onSaveData();

  afx_msg void    onGetSolution();
  afx_msg void    onOptions();
  afx_msg void    onValidate();
  afx_msg void    onDisplayDir();

  afx_msg void    onHelp();
  afx_msg void    onAppAbout();
  afx_msg void    onExit();

  // Text Field & "Do Something" Buttons
  afx_msg void    onLeaveProductname();
  afx_msg void    onLeaveCompanyname();
  afx_msg void    onLeaveWixname();
  afx_msg void    onLeaveWixVersion();
  afx_msg void    onUpdateCtrlPanelIcon();
  afx_msg void    OnBrowseCtrlPanelIcon();
  afx_msg void    onChangeFeature();
  afx_msg void    onLeaveFeature();
  afx_msg void    onNewFeature();
  afx_msg void    onDelFeature();

  afx_msg void    onLeavingProgFile();
  afx_msg void    onLeavingStartMenu();

  afx_msg void    onChangeComponent();
  afx_msg void    onLeavingComponent();
  afx_msg void    onNewComponent();
  afx_msg void    onDeleteComponent();
  afx_msg void    onBrowseComponentPath();

  afx_msg void    onLeavingDescription();

  afx_msg void    onStartupClicked();
  afx_msg void    onBnClickedOndesktop();
  afx_msg void    onBnClickedVersionavail();
  afx_msg void    onBnClickedOnstartmenu();
  afx_msg void    onBnClickedOnPath();

  afx_msg void    onBnClickedIsWin10();
  afx_msg void    onBnClickedIsWin7();
  afx_msg void    onBnClickedIsWinXP();

  afx_msg void    onUpdateIcon();
  afx_msg void    onBrowseForIcon();
  afx_msg void    OnSize(UINT nType, int cx, int cy);
  };



