// Print the content of the NotePad


#pragma once
#include "NotePad.h"
#include "PrinterMgr.h"

class DialogEx;
class NPIter;
class Note;


typedef void HdrFtrFn(NotePad& np, int pageNo, int noPages);


class PrintNtPd {

PrinterMgr& pm;
NotePad&    np;                   // contains the body of the output
NPIter      iter{np};
bool        startNp{false};       // Set to true to start np scan

bool      outputHdr{false};
HdrFtrFn* getHdr{0};
HdrFtrFn* getFtr{0};

bool      outputFtr{false};
int       pageNo{0};
int       noPages{0};

public:

  PrintNtPd(PrinterMgr& printerMgr, NotePad& notePad) : pm(printerMgr), np(notePad) { }
 ~PrintNtPd();

  void setHeader(TCchar* face, int tenthsPt, HdrFtrFn* getHeader);
  void setFooter(TCchar* face, int tenthsPt, HdrFtrFn* getHeader);
  void getNoPages(TCchar* docName);

  void operator() (TCchar* docName);

private:

  bool printPage(   NPItem& item, int pageNo);
  bool printBody(   NPItem& item);
  bool process(     NPItem& item);
  bool procOneItem( NPItem& item);
//  bool flush(      Note* note);
//  bool flush();

  bool printHeader(int pageNo);
  bool printFooter(int pageNo);
  void printNp(NotePad& np);

  PrintNtPd() : pm(*(PrinterMgr*)0), np(*(NotePad*)0) { }
  };



///////////------------------

//DialogEx*  dialogEx{0};
//  void setHeader(TCchar* face, int tenthsPt, DialogEx* dialogEx);
//  void setFooter(TCchar* face, int tenthsPt, DialogEx* dialogEx);

