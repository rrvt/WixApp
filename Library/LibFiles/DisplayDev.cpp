// Display Device Data


#include "stdafx.h"
#include "DisplayDev.h"




void DisplayDev::initialize() {note = 0; lastPageNo = 0; lastLeftMargin = 0; endDoc = debugging = false;}



void DisplayDev::startDev()
                        {note = notePadLp.start(); dev.noPages = 0; lastPageNo = 0; lastLeftMargin = 0;}


  // Output to Device (Display or Printer)

void DisplayDev::toDevice() {
bool endLoopNow = false;

  for (; note && !endLoopNow; note = notePadLp.nextNode()) {

    if (note->beginLine)     dev << dBeginLine;
    if (note->leftMargin != lastLeftMargin)
                            {dev << dSetLMargin(note->leftMargin); lastLeftMargin = note->leftMargin;}
    if (note->clrTabs)       dev << dClrTabs;
    if (note->tabValue)      dev << dSetTab(note->tabValue);
    if (note->rTabValue)     dev << dSetRTab(note->rTabValue);

    if (note->fSize)         dev << dFSize(note->fSize);
    if (note->prevFont)      dev << dPrevFont;
    if (note->bold)          dev << dBoldFont;
    if (note->italic)        dev << dItalicFont;
    if (note->underline)     dev << dUnderLineFont;
    if (note->strikeOut)     dev << dStrikeOutFont;

    if (note->tab == true)   dev << dTab;
    if (note->center)        dev << dCenter;
    if (note->right)         dev << dRight;

    if (note->editBoxX >= 0) dev << dEditBox(note->editBoxX);

                             dev << note->line;
    if (note->endLine)       dev << dEndLine;

    if (note->endPage)      {dev << dEndPage; if (dev.isEndPage()) endLoopNow = true;}

    if (note->crlf)         {dev << dCrlf;    if (dev.isEndPage()) endLoopNow = true;}

    if (note->debug)         debugging = true;
    }

  endDoc = note == 0;
  }



void DisplayDev::printFooter(int pageNo, String& license, Date& licDate) {
int i;

  if (dev.withinBounds())
    for (i = 0; !dev.isEndPage() && i < 65; i++) dev << dCrlf;

  if (!license.empty()) dev << license;

  dev << dCenter << toString(pageNo);

  if (!licDate.isEmpty()) {dev << dRight; dev << licDate.getDate();}

  dev << dflushFtr;
  }

