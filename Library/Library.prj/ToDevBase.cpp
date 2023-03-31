// Display DevBase Data


#include "pch.h"
#include "ToDevBase.h"

#include "MessageBox.h"         // Debugging Only


// Output to DevBase (Display or Printer)

void ToDevBase::operator() () {

  dev.cleanupWrap();

  for ( ; note && !dev.doEndPageChk(); note = npIter++) {

    if (!note->fFace.isEmpty()) dev << dFFace(note->fFace);
    if (note->fSize)            dev << dFSize(note->fSize);
    if (note->bold)             dev << dBold;
    if (note->italic)           dev << dItalic;
    if (note->underline)        dev << dUnderLine;
    if (note->strikeOut)        dev << dStrikeOut;

    if (note->prevFont)         dev << dFont;

    if (note->leftMargin >= 0)   dev << dSetLMargin(note->leftMargin);

    if (note->clrTabs)           dev << dClrTabs;
    if (note->tabValue)         {if (!note->rightTab) dev << dSetTab( note->tabValue);
                                 else                 dev << dSetRTab(note->tabValue);
                                 }

    if (note->tab == true)      dev << dTab;
    if (note->center)           dev << dCenter;
    if (note->right)            dev << dRight;

    if (note->beginLine)        dev << dBeginLine;

                                dev << note->line;

    if (note->nmbr.typ)         dev << note->nmbr;

    if (note->endLine)          dev << dEndLine;

    if (note->crlf)             dev << dCrlf;

    if (note->endPage)          dev << dEndPage;

    if (note->debug)             debugging = true;
    }

  endDoc = note == 0;
  }


