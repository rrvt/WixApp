// Print the content of the NotePad


#include "pch.h"
#include "DisplayNtPd.h"
#include "ScrollView.h"
#include "NotePad.h"




void DisplayNtPd::setHeader(TCchar* face, int tenthsPt, HdrFtrFn* getHeader) {
  outputHdr = true;   dm.setHdr(face, tenthsPt);   getHdr = getHeader;
  }


void DisplayNtPd::setFooter(TCchar* face, int tenthsPt, HdrFtrFn* getFooter) {
  outputFtr = true;   dm.setFtr(face,tenthsPt);   getFtr = getFooter;
  }


void DisplayNtPd::operator() (TCchar* docName) {
int    pageNo = 1;
NPItem item;

  dm.startDoc(docName);

  startNp = true;

  while (displayPage(item, pageNo++)) continue;
  }


bool DisplayNtPd::displayPage(NPItem& item, int pageNo) {
bool finishPage;

  displayHeader(pageNo);

    dm.startPage();   finishPage = displayBody(item);   dm.endPage();

  displayFooter(pageNo);   return finishPage;
  }


// Display whole page from String and then notepad,
// return true at end of page, false when reached the end of the notepad

bool DisplayNtPd::displayBody(NPItem& item) {

  dm.flush();

  if      (startNp)      {item = iter();   startNp = false;}
  else if (item.isFin()) {item = iter++;}

  while (item.note) {

    if (!process(item)) return true;

    if (item.isFin()) item = iter++;
    }

  return !dm.flush();
  }


// Process note, may break in middle of note items.
// Returns true if more data may be output to page on the same output line or the next line

bool DisplayNtPd::process(NPItem& item) {

  for ( ; !item.isFin(); item++) if (!procOneItem(item)) return false;

  return true;
  }


void DisplayNtPd::displayHeader(int pageNo) {
NotePad np;

  if (!outputHdr) return;

  if (getHdr) (*getHdr)(np, pageNo, 1);   if (np.isEmpty()) return;

  dm.startHeader();   displayNp(np);   dm.endHeader();
  }




void DisplayNtPd::displayFooter(int pageNo) {
NotePad np;

  if (!outputFtr) return;

  if (getFtr) (*getFtr)(np, pageNo, 1);   if (np.isEmpty()) return;

  dm.startFooter();   displayNp(np);   dm.finLine();   dm.endFooter();
  }


void DisplayNtPd::displayNp(NotePad& np) {
NPIter iter(np);
NPItem item;

  for (item = iter(); item.note; item = iter++) for ( ; !item.isFin();  item++) procOneItem(item);

  dm.flush();
  }


// Process one note.
// Returns true if more data may be output to page on the same output line or the next line

bool DisplayNtPd::procOneItem(NPItem& item) {
Note& note = *item.note;

  switch (item.cur()) {
    case  0: if (note.leftMgn  >= 0     && !dm.setLeftMgn(note.leftMgn))   return false;    break;
    case  1: if (note.rightMgn >= 0     && !dm.setRightMgn(note.rightMgn)) return false;    break;
    case  2: if (!note.fFace.isEmpty()  && !dm.setFace(note.fFace))        return false;    break;
    case  3: if (note.fSize             && !dm.setPoint(note.fSize))       return false;    break;
    case  4: if (note.bold              && !dm.setBold())                  return false;    break;
    case  5: if (note.italic            && !dm.setItalic())                return false;    break;
    case  6: if (note.underline         && !dm.setFUnderline())            return false;    break;
    case  7: if (note.strikeOut         && !dm.setStrikeout())             return false;    break;
    case  8: if (note.prevFont          && !dm.popFont())                  return false;    break;
    case  9: if (note.clrTabs           && !dm.clearTabs())                return false;    break;
    case 10: if (!note.tab.isEmpty()    && !dm.setTab(note.tab))           return false;    break;
    case 11: if (note.tabSeen           && !dm.moveNextTab())              return false;    break;
    case 12: if (note.center            && !dm.setCenter())                return false;    break;
    case 13: if (note.right             && !dm.setRight())                 return false;    break;
    case 14: if (note.beginLine         && !dm.startUnderlining())         return false;    break;
    case 15:                                dm.setText(note.line);                          break;
    case 16: if (note.nmbr.typ)             dm.setText(note.nmbr());                        break;
    case 17: if (note.endLine           && !dm.stopUnderlining())          return false;    break;
    case 18: if (note.debug)                dm.examineFont();                               break;
    case 19: if (note.crlf              && !dm.crlf())                     return false;    break;
    case 20: if (note.endPage) {            item++;                        return false;}   break;
    }

  return true;
  }


#if 0
bool DisplayNtPd::flush(Note* note) {

  if (!dm.isBufEmpty() && !displayStg() && note) note->endPage = true;

  return !dm.isBufEmpty();
  }



// Ouputs content of strings to printer.
// Returns true if more data may be output to page on the same output line or the next line

bool DisplayNtPd::displayStg() {
int i;

  for (i = 0; !dm.isBufEmpty(); ) {

    if (dm.output()) continue;

    dm.ctx->buf.trimLeft();   dm.crlf();
    }

  return true;
  }
#endif



///////------------------------

//  if (!startDoc(docName)) return;
//  endDoc();

//  docInfo.lpszDocName = _T("ABC Document");       //printerName;

//messageBox(docInfo.lpszDocName);

//  if (!endPage()) return false;

//  scrollView.getHeader(np, pageNo);
#if 0
  if (StartPage() >= 0) return true;

  getError(::GetLastError(), s);   messageBox(s);

  AbortDoc();   return false;
#endif

//    scrollView.getFooter(np, pageNo);
#if 0
bool DisplayMgr::startDoc(TCchar* docName)  {
String s;

  createFont(CourierNew, 120);   return true;
#if 0
  if (StartDoc(docName) > 0) return true;                       //&docInfo

  getError(::GetLastError(), s);   messageBox(s);   return false;
#endif
  }
#endif
#if 0
Note* note;

  if (!dm.flush()) return true;

  for (note = startNp ? iter() : iter++; note; note = iter++) {

    startNp = false;   if (!process(*note)) {dm.flush();  return true;}
    }

  return dm.flush();;
#endif
#if 0
Note*  note;
String line;

  for (note = iter(); note; note = iter++) {
    if (!process(*note)) {dm.flush(); return true;}
    }

  dm.flush();   return false;
#endif
#if 0
void DisplayMgr::startPage()
                           {ctx = &bodyCtx;   ctx->x = leftMgn;  ctx->y = topMgn + ctx->hdrOffset;}
#endif

