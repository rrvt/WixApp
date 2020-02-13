
// ChildView.h : interface of the ChildView class
//


#pragma once


// ChildView window

class ChildView : public CWnd
{
// Construction
public:
	ChildView();

// Attributes
public:

// Operations
public:

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~ChildView();

	// Generated message map functions
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};

