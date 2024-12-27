// Transform Display Output into a Report with a header and footer


#include "pch.h"
#include "ReportNtPd.h"
#include "CScrView.h"
#include "NotePad.h"
#include "ResourceData.h"

void ReportNtPd::dspHeader(DevStream& dvs, int pageNo) {
ResourceData res;
String       productName;
String       companyName;
Date         today;    today.getToday();
String       date = today;

  res.getProductName(productName);
  res.getCompanyName(companyName);

  dvs << dBold << dItalic;
  dvs << productName << dCenter << companyName << dRight << date << dCrlf;
  }


void ReportNtPd::dspFooter(DevStream& dev, int pageNo) {
  dev << dCenter << _T("Footer") << dCrlf;
  }


void ReportNtPd::onPreparePrinting(CPrintInfo* info) {printer.set(prtrOrietn);}


void ReportNtPd::onBeginPrinting(CScrView& vw)
                                         {printing = true;  vw.disablePrtWrap();  getPageAttr(vw);}


void ReportNtPd::getData() {
NtPdIter iter(notePad);
Note*    note;

  np.clear();

  for (note = iter(); note; note = iter++) {Note& n = *note->alloc();  n = *note;   np.append(&n);}
  }



void ReportNtPd::prtHeader(DevStream& dev, int pageNo) {
Date   dt;
String s;

  dt.getToday();   s = dt.getDate() + _T(" ") + dt.getHHMM();

  dev << dSetLMargin(0) << dSetRMargin(0);

  dev << title << dRight << s << dCrlf << dCrlf;
  }


void ReportNtPd::prtFooter(DevStream& dev, int pageN) {

  if (pageN > maxPages) maxPages = pageN;

  dev << dSetLMargin(0) << dSetRMargin(0);

  dev << dCenter << pageN << _T(" of ") << maxPages << dFlushFtr;
  }

