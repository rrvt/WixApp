// Toolbar Button (i.e. MFC Tool Bar Button)


#include "stdafx.h"
#include "TBButton.h"


TBButton& TBButton::operator= (TBButton& tbBtn) {
  m_bUserButton         = tbBtn.m_bUserButton;          // BOOL      Is user-defined tool button?
  m_bText               = tbBtn.m_bText;                // BOOL      Draw text label
  m_bImage              = tbBtn.m_bImage;               // BOOL      Draw image
  m_bWrap               = tbBtn.m_bWrap;                // BOOL      Wrap toolbar
  m_bWholeText          = tbBtn.m_bWholeText;           // BOOL      Is whole text printed?
  m_bTextBelow          = tbBtn.m_bTextBelow;           // BOOL      Is text below image?
  m_bDragFromCollection = tbBtn.m_bDragFromCollection;  // BOOL      Button is dragged from collection
  m_nID                 = tbBtn.m_nID;                  // UINT      Command ID that this button sends
  m_nStyle              = tbBtn.m_nStyle;               // UINT      TBBS_ styles
  m_dwdItemData         = tbBtn.m_dwdItemData;          // DWORD_PTR User-defined data
  m_strText             = tbBtn.m_strText;         // CString Button text (for user-defined buttons only!)
  m_strTextCustom       = tbBtn.m_strTextCustom;        // CString   Text appear on the customization list
  m_iImage              = tbBtn.m_iImage;      // int index into bitmap of this button's picture
  m_iUserImage          = tbBtn.m_iUserImage;  // int index into user's bitmap of this button's picture
  m_bLocked             = tbBtn.m_bLocked;              // BOOL  Is buttons placed on the "locked" toolbar
  m_bIsHidden           = tbBtn.m_bIsHidden;            // BOOL  Button rectangle is out of bar
  m_bDisableFill        = tbBtn.m_bDisableFill;         // BOOL  Disable interior fill
  m_bExtraSize          = tbBtn.m_bExtraSize;           // BOOL  Is Visual Manager's extra size used?
  m_bHorz               = tbBtn.m_bHorz;                // BOOL  Is located on the horizontal toolbar?
  m_bVisible            = tbBtn.m_bVisible;             // BOOL  Is button  visible
  m_rect                = tbBtn.m_rect;                 // CRect Button location
  m_sizeText            = tbBtn.m_sizeText;             // CSize Actual only if m_bTextBelow is TRUE
  m_pWndParent          = tbBtn.m_pWndParent;           // CWnd* Parent window
  return *this;
  }







