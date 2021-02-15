
// MainFrm.h : interface of the MainFrame class
//

#pragma once
#include "ChildView.h"

class MainFrame : public CFrameWnd
{

public:
  MainFrame();
protected:
  DECLARE_DYNAMIC(MainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// Implementation
public:
  virtual ~MainFrame();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  ChildView    m_wndView;
  CWnd         viewWnd;

// Generated message map functions
protected:
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnSetFocus(CWnd *pOldWnd);
  DECLARE_MESSAGE_MAP()

public:
};


