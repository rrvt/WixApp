// MainFrm.cpp : implementation of the MainFrame class


#include "pch.h"
#include "MainFrame.h"
#include "AboutDlg.h"
#include "CmdTest.h"
#include "SFTP.h"
#include "SFTPDoc.h"
#include "SFTPView.h"
#include "WorkerThrd.h"


// MainFrame

IMPLEMENT_DYNCREATE(MainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(MainFrame, CFrameWndEx)
  ON_WM_CREATE()
  ON_WM_SYSCOMMAND()

  ON_WM_MOVE()
  ON_WM_SIZE()

  // Thread Shutdown Messages

  ON_MESSAGE(ID_SetWdwScroll,                &onSetWdwScroll)
  ON_MESSAGE(ID_DisplayMsg,                  &onDisplayMsg)

  ON_MESSAGE(ID_WebCmprMsg,                  &onPrepUpLdMsg)

  ON_MESSAGE(ID_UpdateMsg,                   &onUpdateMsg)
  ON_MESSAGE(ID_StepPrgBarMsg,               &onStepPrgBarMsg)

  ON_MESSAGE(ID_FinAPPEMsg,                  &onFinAPPEMsg)
  ON_MESSAGE(ID_FinSTORMsg,                  &onFinSTORMsg)
  ON_MESSAGE(ID_FinSTOUMsg,                  &onFinSTOUMsg)
  ON_MESSAGE(ID_FinWebFilesMsg,              &onFinWebFilesMsg)

  ON_REGISTERED_MESSAGE(AFX_WM_RESETTOOLBAR, &OnResetToolBar)              // MainFrame::

END_MESSAGE_MAP()


static UINT indicators[] = {
  ID_SEPARATOR,           // status line indicator
  ID_INDICATOR_CAPS,
  ID_INDICATOR_NUM,
  ID_INDICATOR_SCRL,
  };

// MainFrame construction/destruction

MainFrame::MainFrame() noexcept : isInitialized(false) { }

MainFrame::~MainFrame() {winPos.~WinPos();}


BOOL MainFrame::PreCreateWindow(CREATESTRUCT& cs) {

  cs.style &= ~FWS_ADDTOTITLE;  cs.lpszName = _T("AddProj");         // Sets the default title left part

  return CFrameWndEx::PreCreateWindow(cs);
  }


int MainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) {
CRect winRect;

  if (CFrameWndEx::OnCreate(lpCreateStruct) == -1) return -1;

  if (!m_wndMenuBar.Create(this)) {TRACE0("Failed to create menubar\n"); return -1;}

  CMFCPopupMenu::SetForceMenuFocus(FALSE);

  if (!toolBar.create(this, IDR_MAINFRAME)) {TRACE0("Failed to create status bar\n"); return -1;}

  addAboutToSysMenu(IDD_AboutBox, IDS_AboutBox);

  if (!m_wndStatusBar.Create(this)) {TRACE0("Failed to create status bar\n"); return -1;}

  m_wndStatusBar.SetIndicators(indicators, noElements(indicators));  //sizeof(indicators)/sizeof(UINT)

  GetWindowRect(&winRect);   winPos.initialPos(this, winRect);

  DockPane(&m_wndMenuBar);   DockPane(&toolBar);

  CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
                                                                         // Affects look of toolbar, etc.
  isInitialized = true;   return 0;
  }


void MainFrame::OnSysCommand(UINT nID, LPARAM lParam) {

  if ((nID & 0xFFF0) == sysAboutID) {AboutDlg aboutDlg; aboutDlg.DoModal(); return;}

  CMainFrm::OnSysCommand(nID, lParam);
  }


void MainFrame::OnMove(int x, int y)
           {CRect winRect;   GetWindowRect(&winRect);   winPos.set(winRect);   CFrameWndEx::OnMove(x, y);}


void MainFrame::OnSize(UINT nType, int cx, int cy) {
CRect winRect;

  CFrameWndEx::OnSize(nType, cx, cy);

  if (!isInitialized) return;

  GetWindowRect(&winRect);   winPos.set(winRect);
  }


// MainFrame message handlers

afx_msg LRESULT MainFrame::OnResetToolBar(WPARAM wParam, LPARAM lParam) {setupToolBar();  return 0;}


void MainFrame::setupToolBar() {CRect winRect;   GetWindowRect(&winRect);   toolBar.set(winRect);}


// Progress Bar functions

void MainFrame::startPrgBar(int noSteps) {
CRect rect;
CRect winRect;

  GetClientRect(winRect);
  rect.left  = 20;                   rect.top = winRect.bottom / 2 - 5;
  rect.right = winRect.right - 20;   rect.bottom = rect.top + 10;

  progressBar.Create(WS_CHILD | WS_VISIBLE | PBS_SMOOTH, rect, this, IDC_ProgCtrl);

  progressBar.SetRange(0, noSteps);    progressBar.SetPos(0);   progressBar.SetStep(1);
  }


LRESULT MainFrame::onStepPrgBarMsg(WPARAM wParam, LPARAM lParam) {progressBar.StepIt();   return 0;}


LRESULT MainFrame::onDisplayMsg(WPARAM wParam, LPARAM lParam)   {doc()->display();   return 0;}
LRESULT MainFrame::onSetWdwScroll(WPARAM wParam, LPARAM lParam)
                                                         {view()->setWidwScroll(wParam); return 0;}
LRESULT MainFrame::onFinAPPEMsg(WPARAM wParam, LPARAM lParam)
                                                            {return cmdTest.onFinAPPEMsg(wParam, lParam);}
LRESULT MainFrame::onFinSTORMsg(WPARAM wParam, LPARAM lParam)
                                                            {return cmdTest.onFinSTORMsg(wParam, lParam);}
LRESULT MainFrame::onFinSTOUMsg(WPARAM wParam, LPARAM lParam)
                                                            {return cmdTest.onFinSTOUMsg(wParam, lParam);}

// MainFrame diagnostics

#ifdef _DEBUG
void MainFrame::AssertValid() const          {CFrameWndEx::AssertValid();}
void MainFrame::Dump(CDumpContext& dc) const {CFrameWndEx::Dump(dc);}
#endif //_DEBUG



//  ON_MESSAGE(ID_GetThrdMsg,                  &onGetThrdMsg)
//  ON_MESSAGE(ID_PickThrdMsg,                 &onPickThrdMsg)
//  ON_MESSAGE(ID_ConfirmMsg,                  &onConfirmMsg)

// Thread Shutdown Messages
#if 0
LRESULT MainFrame::onGetThrdMsg(WPARAM wParam, LPARAM lParam) {
#if 1

  doc()->onGetThrdMsg(wParam, lParam); return 0;

#else

SFTPDoc& d = *doc();

  switch (wparam) {
//    case ID_IncProgress : if (stepSize) incProgress();     break;
    case ID_EndThread   : d.finGetSite(lParam);     break;
    default             : d.finGetSite(TE_Unknown); break;
    }

  return 0;
#endif
  }
#endif


#if 0
LRESULT MainFrame::onPickThrdMsg(WPARAM wParam, LPARAM lParam) {
#if 1

  doc()->onPickThrdMsg(wParam, lParam);   return 0;

#else
SFTPDoc& d = *doc();

  switch (wparam) {
//    case ID_IncProgress : if (stepSize) incProgress();     break;
    case ID_EndThread   : d.finLoadSite(lParam);     break;
    default             : d.finLoadSite(TE_Unknown); break;
    }

  return 0;
#endif
  }


LRESULT MainFrame::onConfirmMsg(WPARAM wParam, LPARAM lParam) {
#if 1

  doc()->onConfirmMsg(wParam, lParam);   return 0;

#else
SFTPDoc& d = *doc();

  switch (wparam) {
//    case ID_IncProgress : d.cnfrmPrgs(lParam);   if (stepSize) incProgress();   break;
    case ID_EndThread   : d.finConfirm(lParam);     break;
    default             : d.finConfirm(TE_Unknown); break;
    }

  return 0;
#endif
  }
#endif





