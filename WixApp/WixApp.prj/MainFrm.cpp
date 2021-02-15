// MainFrm.cpp : implementation of the MainFrame class

#include "stdafx.h"
#include "WixApp.h"

#include "MainFrm.h"


// MainFrame

IMPLEMENT_DYNAMIC(MainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(MainFrame, CFrameWnd)
  ON_WM_CREATE()
  ON_WM_SETFOCUS()
END_MESSAGE_MAP()

// MainFrame construction/destruction

MainFrame::MainFrame() { }

MainFrame::~MainFrame() { }


int MainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) {

  if (CFrameWnd::OnCreate(lpCreateStruct) == -1) return -1;

  // create a view to occupy the client area of the frame

  if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(100, 100, 300, 300), this,
                                                                              AFX_IDW_PANE_FIRST, NULL))
    {TRACE0("Failed to create view window\n"); return -1;}

  return 0;
  }


BOOL MainFrame::PreCreateWindow(CREATESTRUCT& cs) {

  if (!CFrameWnd::PreCreateWindow(cs)) return FALSE;

  cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE | WS_THICKFRAME | WS_SYSMENU;

  cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
  cs.lpszClass = AfxRegisterWndClass(0);
  return TRUE;
  }


// MainFrame diagnostics

#ifdef _DEBUG
void MainFrame::AssertValid() const
{
  CFrameWnd::AssertValid();
}

void MainFrame::Dump(CDumpContext& dc) const
{
  CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// MainFrame message handlers

void MainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
  // forward focus to the view window
  m_wndView.SetFocus();
}

BOOL MainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
  // let the view have first crack at the command
  if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
    return TRUE;

  // otherwise, do default handling
  return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

