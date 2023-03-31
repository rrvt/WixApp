// Report Base


#include "pch.h"
#include "ReportBase.h"
#include "CScrView.h"


void ReportBase::display(CScrView& vw) {printing = false;  getData(vw);}


void ReportBase::getPageAttr(CScrView& vw) {getData(vw);   maxPages = vw.getNoPages();}


void ReportBase::prtHeader(DevBase& dev, int pageNo) {
Date   dt;
String s;

  if (!printing) return;

  dt.getToday();   s = dt.getDate() + _T(" ") + dt.getHHMM();

  dev << title << dRight << s << dCrlf << dCrlf;
  }


void ReportBase::prtFooter(DevBase& dev, int pageNo) {

  if (pageNo > maxPages) maxPages = pageNo;

  dev << dRight << _T("Page ") << pageNo << _T(" of ") << maxPages << dFlushFtr;
  }


void ReportBase::txtOut(Archive& ar, double tabFactor) {setArchiveAttr(tabFactor); np.archive(ar);}


void ReportBase::setArchiveAttr(double f)
                               {int w = prtrOrietn == PortOrient ? 106 : 128;   np.setArchiveAttr(w, f);}

