// SFTPView.h : interface of the SFTPView class


#pragma once
#include "CScrView.h"
#include "NotePadRpt.h"


class SFTPDoc;


class SFTPView : public CScrView {

bool       scrollWdw;

protected: // create from serialization only

  SFTPView() noexcept;

  DECLARE_DYNCREATE(SFTPView)

public:

  virtual        ~SFTPView() { }

  void            initRptOrietn();
  void            saveRptOrietn();

  virtual BOOL    PreCreateWindow(CREATESTRUCT& cs);
  virtual void    OnInitialUpdate();

  virtual void    setWidwScroll(bool scroll = true) {scrollWdw = scroll;}

  virtual void    displayHeader(DevBase& dev);
  virtual void    displayFooter(DevBase& dev);

  virtual void    onPreparePrinting(CPrintInfo* info);
  virtual void    onBeginPrinting();
  virtual void    onDisplayOutput();

  virtual void    printHeader(DevBase& dev, int pageNo);
  virtual void    printFooter(DevBase& dev, int pageNo);
  virtual void    OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

      SFTPDoc*    GetDocument() const;

public:

#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

public:

  DECLARE_MESSAGE_MAP()

  afx_msg void onOptions();
  afx_msg void onRptOrietn();

  afx_msg void OnSetFocus(CWnd* pOldWnd);

  afx_msg void OnLButtonDown(  UINT nFlags, CPoint point);
  afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

  afx_msg void OnContextMenu(  CWnd* ,      CPoint point);
  afx_msg void onCopy();
  afx_msg void onPup1();
  afx_msg void onPup2();
  };



#ifndef _DEBUG  // debug version in SFTPView.cpp
inline SFTPDoc* SFTPView::GetDocument() const {return reinterpret_cast<SFTPDoc*>(m_pDocument);}
#endif

