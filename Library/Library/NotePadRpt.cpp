// NotePad Report -- the data is stored in the global notePad


#include "pch.h"
#include "NotePadRpt.h"
#include "CScrView.h"


void NotePadRpt::onPreparePrinting(CPrintInfo* info) {printer.set(prtrOrietn);}


void NotePadRpt::onBeginPrinting(CScrView& vw) {printing = true;  vw.disablePrtWrap();  getPageAttr(vw);}


void NotePadRpt::getData(CScrView& vw) {
NtPdIter iter(notePad);
Note*    note;

  np.clear();

  for (note = iter(); note; note = iter++) {Note& n = *note->alloc();   n = *note;    np.append(&n);}
  }


void NotePadRpt::prtHeader(DevBase& dev, int pageNo) {
Date   dt;
String s;

  dt.getToday();   s = dt.getDate() + _T(" ") + dt.getHHMM();

  dev << title << dRight << s << dCrlf << dCrlf;
  }


void NotePadRpt::prtFooter(DevBase& dev, int pageN) {

  if (pageN > maxPages) maxPages = pageN;

  dev << dCenter << pageN << _T(" of ") << maxPages << dFlushFtr;
  }


