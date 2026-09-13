// Print the content of the NotePad


#pragma once
#include "DisplayMgr.h"
#include "NotePad.h"

class ScrollView;
class NPIter;
class Note;

typedef void HdrFtrFn(NotePad& np, int pageNo, int noPages);



class DisplayNtPd {

DisplayMgr& dm;
ScrollView& scrollView;           // Use virtual functions to gather info
NotePad&    np;                   // contains the body of the output
NPIter      iter{np};
bool        startNp{false};       // Set to true to start np scan

bool        outputHdr{false};
bool        outputFtr{false};
HdrFtrFn*   getHdr{0};
HdrFtrFn*   getFtr{0};


public:

  DisplayNtPd(NotePad& notePad, DisplayMgr& displayMgr, ScrollView& view) :
                                                  np(notePad), dm(displayMgr), scrollView(view) { }
 ~DisplayNtPd() { }

  void setHeader(TCchar* face, int tenthsPt, HdrFtrFn* getHeader);
  void setFooter(TCchar* face, int tenthsPt, HdrFtrFn* getFooter);

  void operator() (TCchar* docName);

private:

  bool displayPage(NPItem& item, int pageNo);
  bool displayBody(NPItem& item);
  bool process(    NPItem& item);
  bool procOneItem(NPItem& item);

  void displayHeader(int pageNo);
  void displayFooter(int pageNo);
  void displayNp(NotePad& np);
  };




