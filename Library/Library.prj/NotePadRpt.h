// NotePad Report -- the data is stored in the global notePad


#pragma once
#include "ReportBase.h"


class Note;


class NotePadRpt : public ReportBase {

public:

  NotePadRpt(NotePad& notePad) : ReportBase(notePad) { }
 ~NotePadRpt() { }

//  void display(CScrView& vw);
  virtual void print(CScrView& vw);

//  void detWraps(  CScrView& vw);                  // Wrap each entity in the report body

//  void detNoPages(CScrView& vw);                  // Performs a dummy printer output to determine the
                                                    // number of pages that will be produced
  virtual void footer(Device& dev, int pageNo);     // Output page Footer to NotePad

protected:

  virtual void create(CScrView& vw);

  virtual void saveTab(Note& note);
  virtual void restoreTabs();

  virtual int  header(NotePad& np, bool printing);

private:

  NotePadRpt() : ReportBase(*(NotePad*)0) { }
  };


