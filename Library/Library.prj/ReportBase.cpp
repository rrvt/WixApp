// Report Base


#include "stdafx.h"
#include "ReportBase.h"
#include "CScrView.h"


void ReportBase::display(CScrView& vw) {printing = false; maxLines = BigNmbr; create();}


void ReportBase::print(CScrView& vw)
                            {printing = true; maxLines = vw.noLinesPrPg();  detNoPages(vw);   create();}


void ReportBase::detNoPages(CScrView& vw) {int nLns;   create();   vw.trialRun(nLns, maxPages);}


void ReportBase::footer(Device& dev, int pageNo) {

  if (pageNo > maxPages) maxPages = pageNo;

  dev << dRight << _T("Page ") << pageNo << _T(" of ") << maxPages << dflushFtr;
  }

