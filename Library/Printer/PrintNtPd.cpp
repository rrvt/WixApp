// Print the content of the NotePad


#include "pch.h"
#include "PrintNtPd.h"
#include "DialogEx.h"


static TCchar* CourierNew = _T("Courier New");


PrintNtPd::~PrintNtPd() { }


void PrintNtPd::setHeader(TCchar* face, int tenthsPt, HdrFtrFn* getHeader) {
  outputHdr = true;   pm.setHdr(face, tenthsPt);   getHdr = getHeader;
  }


void PrintNtPd::setFooter(TCchar* face, int tenthsPt, HdrFtrFn* getFooter) {
  outputFtr = true;   pm.setFtr(face,tenthsPt);   getFtr = getFooter;
  }


void PrintNtPd::getNoPages(TCchar* docName) {

  pm.suppress = true;

    try {(*this)(docName);} catch (...) { }

  pm.suppress = false;   noPages = pageNo;
  }


void PrintNtPd::operator() (TCchar* docName) {
NPItem item;

  pageNo = 0;

  if (!pm.startDoc(docName)) return;

  startNp = true;

  while (printPage(item, ++pageNo)) continue;

  pm.endDoc();
  }



// Print full page (i.e. headers, body, footer
// Returns true when end of page
// Returns false when end of notepad

bool PrintNtPd::printPage(NPItem& item, int pageNo) {
bool  finishPage;

  if (!pm.startPage()) return false;

    printHeader(pageNo);   //pm.updateCoordinates();

    finishPage = printBody(item);

    printFooter(pageNo);

  if (!pm.endPage()) return false;

  return finishPage;
  }


// Print whole page from buffer and then notepad,
// return true at end of page, false when reached the end of the notepad

bool PrintNtPd::printBody(NPItem& item) {

  pm.flush();

  if      (startNp)      {item = iter();   startNp = false;}
  else if (item.isFin()) {item = iter++;}

  while (item.note) {

    if (!process(item)) return true;

    if (item.isFin()) item = iter++;
    }

  return !pm.flush();          // Data still in buffer when flush is false, so do one more page
  }


// Process note, may break in middle of note items.
// Returns true if more data may be output to page on the same output line or the next line

bool PrintNtPd::process(NPItem& item) {

  for ( ; !item.isFin(); item++) if (!procOneItem(item)) return false;

  return true;
  }


bool PrintNtPd::printHeader(int pageNo) {
NotePad np;

  if (getHdr) (*getHdr)(np, pageNo, noPages);   if (np.isEmpty())return false;

  pm.startHeader();   printNp(np);   pm.endHeader();   return true;
  }


bool PrintNtPd::printFooter(int pageNo) {
NotePad np;

  if (getFtr) (*getFtr)(np, pageNo, noPages);   if (np.isEmpty()) return false;

  pm.startFooter();   printNp(np);   pm.endFooter();   return true;
  }


void PrintNtPd::printNp(NotePad& np) {
NPIter iter(np);
NPItem item;
//String line;

  for (item = iter(); item.note; item = iter++)
                             for ( ; !item.isFin();  item++) if (!process(item)) {pm.flush(); return;}
  pm.flush();
  }


// Process one item in note, return true if may continue, false at end of page

bool PrintNtPd::procOneItem(NPItem& item) {
Note& note = *item.note;

  switch (item.cur()) {
    case  0: if (note.leftMgn  >= 0     && !pm.setLeftMgn(note.leftMgn))      return false;  break;
    case  1: if (note.rightMgn >= 0     && !pm.setRightMgn(note.rightMgn))    return false;  break;
    case  2: if (!note.fFace.isEmpty()  && !pm.setFace(note.fFace))           return false;  break;
    case  3: if (note.fSize             && !pm.setPoint(note.fSize))          return false;  break;
    case  4: if (note.bold              && !pm.setBold())                     return false;  break;
    case  5: if (note.italic            && !pm.setItalic())                   return false;  break;
    case  6: if (note.underline         && !pm.setFUnderline())               return false;  break;
    case  7: if (note.strikeOut         && !pm.setStrikeout())                return false;  break;
    case  8: if (note.prevFont          && !pm.popFont())                     return false;  break;
    case  9: if (note.clrTabs           && !pm.clearTabs())                   return false;  break;
    case 10: if (!note.tab.isEmpty()    && !pm.setTab(note.tab))              return false;  break;
    case 11: if (note.tabSeen           && !pm.moveNextTab())                 return false;  break;
    case 12: if (note.center            && !pm.setCenter())                   return false;  break;
    case 13: if (note.right             && !pm.setRight())                    return false;  break;
    case 14: if (note.beginLine         && !pm.startUnderlining())            return false;  break;
    case 15:                                pm.setText(note.line);                           break;
    case 16: if (note.nmbr.typ)             pm.setText(note.nmbr());                         break;
    case 17: if (note.endLine           && !pm.stopUnderlining())             return false;  break;
    case 18: if (note.debug)                pm.examineFont();                                break;
    case 19: if (note.crlf              && !pm.crlf())                        return false;  break;
    case 20: if (note.endPage) {            item++;                           return false;} break;
    }

  return true;
  }




///////---------------

#if 0
void PrintNtPd::setHeader(TCchar* face, int tenthsPt, DialogEx* dialogEx) {
  outputHdr = true;

  setHdr(face, tenthsPt);

//  this->dialogEx = dialogEx;
  }


void PrintNtPd::setFooter(TCchar* face, int tenthsPt, DialogEx* dialogEx) {

  outputFtr = true;

  setFtr(face,tenthsPt);

//  this->dialogEx = dialogEx;
  }
#endif


//    dialogEx->getHeader(np, pageNo);
//  docInfo.lpszDocName = _T("ABC Document");       //printerName;

//messageBox(docInfo.lpszDocName);
//    if (printNp(np)) messageBox(_T("Header to long!"));
//&docInfo
//    dialogEx->getFooter(np, pageNo);
//if (printNp(np)) messageBox(_T("Footer too long!"));

#if 0
bool PrintNtPd::flush(Note* note) {

  if (!isBufEmpty() && !printStg() && note) note->endPage = true;

  return !isBufEmpty();
  }
#endif

#if 0
// Ouputs content of strings to printer.
// Returns true if buffer empty
// return false when buffer not empty and more data may not be output to page

bool PrintNtPd::flush() {

  while (!isBufEmpty()) {

    if (output()) continue;

    ctx->buf.trimLeft();   if (!crlf())  return false;       // Force a crlf, fail if at end of lin
    }

  return true;
  }
#endif

