// Display View -- Called from a View Class and calls the Display Class
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.


#pragma once
#include "Display.h"
#include "MessageBox.h"
#include "NotePad.h"
#include "TabStops.h"

//#include "Debug.h"

enum { DebugEnd = 3 };


class DisplayView {
NotePadLoop notePadLp;

CDC*        dc;
Note*       note;
bool        pFooter;
bool        endDoc;
bool        maxPageSet;
bool        debugging;
String      license;
Date        licDate;
uint        lastPageNo;
int         lastLeftMargin;

public:
Display     dsp;
bool        scrolling;

  DisplayView() : dc(0), lastLeftMargin(0)
              {note = 0; lastPageNo = 0; endDoc = maxPageSet = debugging = pFooter = scrolling = false;}
 ~DisplayView() {}


  bool isEndDoc() {return endDoc;}
  void resetMaxY() {dsp.resetMaxY();}


  void OnPreparePrinting(CPrintInfo* pInfo)  {

  if (!pInfo) return;

  pInfo->SetMinPage(1);
  pInfo->SetMaxPage(9999);
  }


  void OnBeginPrinting(CDC* cdc, CPrintInfo* pInfo, String& lic, Date& lDate) {

    dc = cdc; license = lic; licDate = lDate;  pFooter = false;

    startDoc();
    }


  void startDoc() {
    note = notePadLp.start(); endDoc = false; dsp.noPages = 0; maxPageSet = false; lastPageNo = 0;
    }


  void OnPrepareDC(TCchar* face, int fontSize, CDC* pDC, CPrintInfo* pInfo) {

    if (pInfo) {

      pInfo->m_bContinuePrinting = true;

      if (pInfo->m_bPreview) {
        if (pInfo->m_nNumPreviewPages == 2) pInfo->m_nNumPreviewPages = 1;

        if (pInfo->m_nCurPage < lastPageNo) repositionTo(face, fontSize, pDC, pInfo);
        }

      else if (endDoc) startDoc();

      lastPageNo = pInfo->m_nCurPage;

      dsp.beginPrinting(pDC, pInfo, true); dsp.preparePrinting(face, fontSize);
      }

    else {startDoc(); dsp.prepareDisplay(face, fontSize, pDC, pInfo, true);}
    }


  void repositionTo(TCchar* face, int fontSize, CDC* pDC, CPrintInfo* pInfo) {
  int i;

    startDoc();

    for (i = 0; dsp.noPages < pInfo->m_nCurPage-1 && !endDoc; i++) {

      dsp.beginPrinting(pDC, pInfo, false); dsp.preparePrinting(face, fontSize);

      OnPrint( pInfo);
      }
    }


  // Draw on Printer (i.e. Output to Printer)

  void OnPrint(CPrintInfo* pInfo) {

    toDevice();

    printFooter(pInfo->m_nCurPage);

    pInfo->m_bContinuePrinting = !endDoc;

    if (endDoc) {pInfo->SetMaxPage(pInfo->m_nCurPage); maxPageSet = true;}

    dsp.clrFont();
    }



  // Draw on Display

  void OnDraw() {
  EditBoxX* x;

    toDevice();

    x = tabStops.getRover();    if (x) x->setFocus();

    dsp.clrFont();
    }


  // Output to Device (Display or Printer)

  void toDevice() {
  bool endLoopNow = false;

    for (; note && !endLoopNow; note = notePadLp.nextNode()) {

      if (note->beginLine)     dsp << dBeginLine;
      if (note->leftMargin != lastLeftMargin)
                              {dsp << dSetLMargin(note->leftMargin); lastLeftMargin = note->leftMargin;}
      if (note->clrTabs)       dsp << dClrTabs;
      if (note->tabValue)      dsp << dSetTab(note->tabValue);
      if (note->rTabValue)     dsp << dSetRTab(note->rTabValue);

      if (note->fSize)         dsp << dFSize(note->fSize);
      if (note->prevFont)      dsp << dPrevFont;
      if (note->bold)          dsp << dBoldFont;
      if (note->italic)        dsp << dItalicFont;
      if (note->underline)     dsp << dUnderLineFont;
      if (note->strikeOut)     dsp << dStrikeOutFont;

      if (note->tab == true)   dsp << dTab;
      if (note->center)        dsp << dCenter;
      if (note->right)         dsp << dRight;

      if (note->editBoxX >= 0) dsp << dEditBox(note->editBoxX);

                               dsp << note->line;
      if (note->endLine)       dsp << dEndLine;

      if (note->endPage)      {dsp << dEndPage; if (dsp.isEndPage()) endLoopNow = true;}

      if (note->crlf)         {dsp << dCrlf;    if (dsp.isEndPage()) endLoopNow = true;}

      if (note->debug)         debugging = true;
      }

    endDoc = note == 0;
    }


  void printFooter(int pageNo) {
  int i;
  pFooter = true;

    if (dsp.withinBounds())
      for (i = 0; !dsp.isEndPage() && i < 65; i++)
        dsp << dCrlf;

    if (!license.empty()) dsp << license;

    dsp << dCenter << toString(pageNo);

    if (!licDate.empty()) {dsp << dRight; dsp << licDate.getDate();}

    dsp << dflushFtr;
    }


  void getScrollSize(RECT& winSize, CSize& s, CSize& pageSize, CSize& scrollSize) {
  int   maxX   = 0;
  int   maxY   = 0;
  int   height = 0;
  int   t      = 0;

    dsp.getMaxPos(maxX, maxY); height = dsp.getHeight();

    if (height) {t = (winSize.bottom - 1) / height; t *= height;}
    else         t = 1;

    pageSize.cy = t; pageSize.cx = winSize.right;

    scrollSize.cx = dsp.getWidth(); scrollSize.cy = height;

    s.cx = maxX; s.cy = maxY;
    }


  void getScrollSize(int noLines, RECT& winSize, CSize& s, CSize& pageSize, CSize& scrollSize) {
  int   maxX   = 0;
  int   maxY   = 0;
  int   height = 0;
  int   t      = 0;

    dsp.getMaxPos(maxX, maxY); height = dsp.getHeight();

    if (height) {t = (winSize.bottom - 1) / height; t *= height;}
    else         t = 1;

    pageSize.cy = t; pageSize.cx = winSize.right;

    scrollSize.cx = dsp.getWidth(); scrollSize.cy = height;

    t = height * noLines;

    s.cx = maxX; s.cy = maxY > t ? maxY : t;
    }

  };

