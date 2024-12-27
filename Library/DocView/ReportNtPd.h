// Transform Display Output into a Report with a header and footer


#pragma once
#include "ReportBase.h"


class ReportNtPd : public ReportBase {

NotePad& np;

public:

  ReportNtPd(NotePad& np) : np(np), ReportBase(np, PrinterID) { }
 ~ReportNtPd() { }

  void dspHeader(DevStream& dev, int pageNo = 1);
  void dspFooter(DevStream& dev, int pageNo = 1);

  void onPreparePrinting(CPrintInfo* info);
  void onBeginPrinting(CScrView& vw);

  void prtHeader(DevStream& dev, int pageNo);
  void prtFooter(DevStream& dev, int pageNo);

  void getData();

  void endPrinting(CDC* pDC, CPrintInfo* pInfo);

private:

  ReportNtPd() : np(*(NotePad*)0), ReportBase(*(NotePad*)0, PrinterID) { }
  };

