// Display Device Data


#include "pch.h"
#include "NtPdToDev.h"

//#include "MessageBox.h"         // Debugging Only


NtPdToDev::NtPdToDev(NotePad& np, DeviceID id) :
  printDev(id >= PrinterID), npIter(np), note(0), dev(*devices.get(id)), devStrm(id),
  endDoc(false), debugging(false), lastPageNo(0) { }


void NtPdToDev::prepare(CDC* dc, CPrintInfo* pInfo) {

  dev.setPrinting(printDev);    dev.setDC(dc);

  if (printDev) {pInfo->m_bContinuePrinting = true;   dev.dcDim();   dev.setBeginPage();}

  else          {dev.initTxt();   dev.wdwDim();   dev.setBeginPage();}
  }


// Output to Device (Display or Printer)

void NtPdToDev::operator() () {

  dev.cleanupWrap();

  for ( ; note && !doEndPageChk(); note = npIter++) {

    if (note->leftMgn  >= 0)      dev.setLeftMargin(note->leftMgn);
    if (note->rightMgn >= 0)      dev.setRightMargin(note->rightMgn);

    if (!note->fFace.isEmpty())   dev.setFontFaceII(note->fFace);
    if (note->fSize)              dev.setFontSizeII(note->fSize);
    if (note->bold)               dev.setBold();
    if (note->italic)             dev.setItalic();
    if (note->underline)          dev.setUnderLine();
    if (note->strikeOut)          dev.setStrikeOut();
    if (note->prevFont)           dev.popFontAttr();

    if (note->clrTabs)            dev.clrTabs();
    if (!note->noteTab.isEmpty()) dev.setTab(note->noteTab);

    if (note->tabSeen)            dev.setTabSeen();
    if (note->center)             dev.setCenter();
    if (note->right)              dev.setRight();

    if (note->beginLine)          dev.setBeginUnderLine();

                                  dev.append(note->line);
    if (note->nmbr.typ)           dev.append(note->nmbr);

    if (note->endLine)            dev.setEndUnderLine();

    if (note->crlf)               dev.crlf();
    if (note->endPage)            dev.setEndPage();

    if (note->debug)              debugging = true;
    }

  endDoc = note == 0;
  }




/*
void NotePad::archive(Archive& ar) {
NtPdIter iter(*this);
Note*    nt;

  ar.arLine.clear();   ar.nmbr.clear();

  for (nt = iter(); nt; nt = iter++) {
    if (nt->leftMgn  >= 0)   ar.setLMargin(nt->leftMgn);
    if (nt->rightMgn >= 0)   ar.setRMargin(nt->rightMgn);
    if (nt->clrTabs)         ar.clrTabs();
    if (nt->tabValue) {      if (nt->rightTab) ar.setRTab(nt->tabValue);
                             else              ar.setTab( nt->tabValue);
                             }
    if (nt->tab)             ar.tab();
    if (nt->center)          ar.center();
    if (nt->right)           ar.right();
    if (nt->beginLine)       ar.beginULine();
    if (!nt->line.isEmpty()) ar.arLine.append(nt->line);
    if (nt->nmbr.typ)       {ar.nmbr = nt->nmbr;  ar.append(ar.nmbr());}
    if (nt->endLine)         ar.endULine();
    if (nt->crlf)            ar.crlf();
    }

  ar.flush();
  }
*/


