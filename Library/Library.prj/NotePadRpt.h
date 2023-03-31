// NotePad Report -- the data is stored in the global notePad


#pragma once
#include "ReportBase.h"


class Note;


class NotePadRpt : public ReportBase {

public:

               NotePadRpt(NotePad& notePad) : ReportBase(notePad) { }
              ~NotePadRpt() { }

  virtual void onPreparePrinting(CPrintInfo* info);
  virtual void onBeginPrinting(CScrView& vw);

  virtual void dspHeader(DevBase& dev, int pageNo = 1) { }
  virtual void dspFooter(DevBase& dev, int pageNo = 1) { }

  virtual void prtHeader(DevBase& dev, int pageNo);
  virtual void prtFooter(DevBase& dev, int pageNo);

protected:

  virtual void getData(CScrView& vw);

private:

  NotePadRpt() : ReportBase(*(NotePad*)0) { }
  };




