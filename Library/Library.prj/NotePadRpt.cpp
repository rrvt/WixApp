// NotePad Report -- the data is stored in the global notePad


#include "stdafx.h"
#include "NotePadRpt.h"
#include "CScrView.h"




void NotePadRpt::print(CScrView& vw) {

  printing = true; maxLines = vw.noLinesPrPg(); vw.disableWrap(printing); detNoPages(vw);

  create(vw);
  }




void NotePadRpt::create(CScrView& vw) {
NtPdIter iter(notePad);
Note*    note;
int      i;

  np.clear();   noLines = BigNmbr;                      // Skip first header

  for (i = 0, note = iter(); note; i++, note = iter++) {

    if (noLines + 1 > maxLines) {

      if (i) np << nEndPage;

      noLines = header(np, printing);

      restoreTabs();
      }

    saveTab(*note);

    Note& n = *note->alloc();   n = *note;

    if (np.append(&n)) noLines += 1;
    }
  }


void NotePadRpt::saveTab(Note& note) {

  if (note.clrTabs)    tabs.clear();

  if (note.tabValue)  {RBtab rbTab;   rbTab.pos = note.tabValue;                       tabs = rbTab;}

  if (note.rTabValue) {RBtab rbTab;   rbTab.pos = note.rTabValue; rbTab.right = true;  tabs = rbTab;}
  }


void NotePadRpt::restoreTabs() {
RbTbIter iter(*this);
RBtab*   p;

  np << nClrTabs;

  for (p = iter(); p; p = iter++) {
    int  pos   = p->pos;
    bool right = p->right;

    if (right) np << nSetRTab(pos);
    else       np << nSetTab(pos);
    }
  }


int NotePadRpt::header(NotePad& np, bool printing) {
Date   dt;
String s;

  if (!printing) return 0;

  dt.getToday();   s = dt.getDate() + _T(" ") + dt.getHHMM();

  np << title << nRight << s << nCrlf << nCrlf;   return 2;
  }


void NotePadRpt::footer(Device& dev, int pageN) {

  if (pageN > maxPages) maxPages = pageN;

  dev << dCenter << pageN << _T(" of ") << maxPages << dFlushFtr;
  }


