// Report Base


#include "pch.h"
#include "ReportBase.h"
#include "Archive.h"
#include "CScrView.h"
#include "DevStream.h"


void ReportBase::display(CScrView& vw) {printing = false;  getData();}


void ReportBase::getPageAttr(CScrView& vw) {getData();   maxPages = vw.getNoPages();}


void ReportBase::prtHeader(Device& dev, int pageNo) {
Date   dt;
String s;

  if (!printing) return;

  dt.getToday();   s = dt.getDate() + _T(" ") + dt.getHHMM();

  dvs << title << dRight << s << dCrlf << dCrlf;
  }


void ReportBase::prtFooter(Device& dev, int pageNo) {

  if (pageNo > maxPages) maxPages = pageNo;

  dvs << dRight << _T("Page ") << pageNo << _T(" of ") << maxPages << dFlushFtr;
  }


void ReportBase::txtOut(Archive& ar, double tabFactor) {setArchiveAttr(tabFactor); ar << np;}


void ReportBase::setArchiveAttr(double f)
                         {int w = prtrOrietn == PortOrient ? 106 : 128;   np.setArchiveAttr(w, f);}

