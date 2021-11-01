// NotePad
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.

#include "stdafx.h"
#include "NotePad.h"
#include "TextPosition.h"


NotePad notePad;


static NtManipInt& setupManipInt(NtManipInt::Func func, int     val);
static NtManipDbl& setupManipDbl(NtManipDbl::Func func, double  val);
static NtManipStg& setupManipStg(NtManipStg::Func func, TCchar* val);

NoteManip nClrTabs;         // add to stream to clear tabs:                  dsp << dClrTabs;
NoteManip nCrlf;            // add to stream to terminate a line on display: dsp << "xyz" << dCrlf;
NoteManip nEndPage;         // add to stream to terminate page when printing otherwise ignore
NoteManip nTab;             // add to stream to tab to next tab position:    dsp << dTab << "xyz";
NoteManip nTableName;       // Set bit to identify table name note:          dsp << dTableName << "xyz";
NoteManip nCenter;          // Set bit to center from here to crlf;
NoteManip nRight;           // Set bit to right align from here to crlf;
NoteManip nBeginLine;       // Begin and end line under text.
NoteManip nEndLine;
NoteManip nDebug;
NoteManip nFont;            // set previous font
NoteManip nBold;            // set font to bold
NoteManip nItalic;          // set font to italic
NoteManip nUnderLine;       // underline font
NoteManip nStrikeOut;       // strike out font

NtManipInt& nSetTab(    int val) {return setupManipInt(NotePad::doSetTab,     val);}
NtManipInt& nSetRTab(   int val) {return setupManipInt(NotePad::doSetRTab,    val);}
NtManipInt& nSetLMargin(int val) {return setupManipInt(NotePad::doSetLMargin, val);}
NtManipInt& nSetWidth(  int val) {return setupManipInt(NotePad::doSetWidth,   val);}
NtManipInt& nSetPrec(   int val) {return setupManipInt(NotePad::doSetPrec,    val);}
NtManipInt& nEditBox(   int val) {return setupManipInt(NotePad::doEditBox,    val);}
NtManipStg& nFFace( TCchar* face){return setupManipStg(NotePad::doFFace,      face);}
NtManipDbl& nFSize(  double val) {return setupManipDbl(NotePad::doFSize,      val);}

NtManipInt& setupManipInt(NtManipInt::Func func, int val)
          {NewAlloc(NtManipInt);   NtManipInt* m = AllocNode;  m->set(func, val); return *m;}

NtManipDbl& setupManipDbl(NtManipDbl::Func func, double val)
          {NewAlloc(NtManipDbl);   NtManipDbl* m = AllocNode;  m->set(func, val); return *m;}

NtManipStg& setupManipStg(NtManipStg::Func func, TCchar* val)
          {NewAlloc(NtManipStg);   NtManipStg* m = AllocNode;  m->set(func, val); return *m;}


NotePad::NotePad() : arWidth(106), tabFactor(1.0) {clear();}


// To clear notepad for new input.

void NotePad::clear() {noteList.removeAll(); note = 0; initialize();}


void NotePad::initialize() {

  nCrlf.n      = this; nCrlf.func      = NotePad::doCrlf;
  nClrTabs.n   = this; nClrTabs.func   = NotePad::doClrTabs;
  nEndPage.n   = this; nEndPage.func   = NotePad::doEndPage;
  nTab.n       = this; nTab.func       = NotePad::doTab;
  nTableName.n = this; nTableName.func = NotePad::setTableName;
  nCenter.n    = this; nCenter.func    = NotePad::doCenter;
  nRight.n     = this; nRight.func     = NotePad::doRight;
  nBeginLine.n = this; nBeginLine.func = NotePad::doBeginLine;
  nEndLine.n   = this; nEndLine.func   = NotePad::doEndLine;
  nDebug.n     = this; nDebug.func     = NotePad::doDebug;
  nFont.n      = this; nFont.func      = NotePad::doPrevFont;
  nBold.n      = this; nBold.func      = NotePad::doBoldFont;
  nItalic.n    = this; nItalic.func    = NotePad::doItalicFont;
  nUnderLine.n = this; nUnderLine.func = NotePad::doUnderLineFont;
  nStrikeOut.n = this; nStrikeOut.func = NotePad::doStrikeOutFont;

  noLines = 0;
  }


void NotePad::archive(Archive& ar) {
NtPdIter     iter(*this);
Note*        note;
TextPosition tPos;
Tab          tab;
bool         hasTab;
bool         rightTab;
int          cnt;

  for (note = iter(); note; note = iter++) {

    if (note->clrTabs)   tPos.clrTabs();
    if (note->tabValue)  tPos.setTab(applyTabFactor(note->tabValue));
    if (note->rTabValue) tPos.setRTab(applyTabFactor(note->rTabValue));

    hasTab = note->tab;

    if (hasTab) {
      tab = tPos.findNextTab();   rightTab = tab.right;

      if (!rightTab) movPos(tPos, tab.pos, ar);
      }

    if (note->right) {
      tab.pos = arWidth; tab.right = rightTab = hasTab = true;
      }

    cnt = note->line.length();

    if (note->center) movPos(tPos, (arWidth - cnt) / 2, ar);

    if (hasTab && rightTab) movPos(tPos, tab.pos - cnt, ar);

    if (cnt) {ar.write(note->line); tPos.move(cnt);}

    if (note->crlf) {ar.write(_T('\n'));   tPos.doCR();}
    }
  }


int NotePad::applyTabFactor(int tb) {
double val = tb * tabFactor;
double v2  = val + 0.5;
int    t   = int(v2);
  return t;
  }


void NotePad::movPos(TextPosition& from, int to, Archive& ar) {
int x = from.getCharPos();

  if (x >= to) to = x + 3;

  for ( ; x < to; x++) {ar << _T(" "); from.move(1);}
  }


NotePad& NotePad::append(Wrap& w) {
Note& note = getNote();  if (!note.line.isEmpty()) this->note = 0;

  getNote().wrap = w; return *this;
  }



NotePad& NotePad::doClrTabs(NotePad& n) {
Note& note = n.getNote();

  if (!note.line.empty() || note.clrTabs || note.right || note.tab ||
                                                             note.tabValue || note.rTabValue) n.note = 0;
  n.getNote().clrTabs   = true;   return n;
  }


NotePad& NotePad::doTab(NotePad& n) {
Note& note = n.getNote();

  if (!note.line.empty() || note.tab || note.beginLine || note.right || note.editBoxX >= 0) n.note = 0;

  n.getNote().tab = true; return n;
  }


NotePad& NotePad::doSetLMargin(NotePad& n, int v) {
Note& note = n.getNote();

  if (note.leftMargin != v) n.note = 0;   n.getNote().leftMargin = v; return n;
  }


NotePad& NotePad::doCrlf(NotePad& n) {
Note& note = n.getNote();

  if (note.leftMargin) n.note = 0;

  return n.crlf();
  }


NotePad& NotePad::crlf() {getNote().crlf = true;   note = 0;   noLines++;   return *this;}


NotePad& NotePad::endPage() {

  if (noLines) {note = 0; getNote().endPage = true; note->line; note = 0; noLines = 0;}

  return *this;
  }


NotePad& NotePad::doCenter( NotePad& n)    {
Note& note = n.getNote();

  if (!note.line.empty()) n.note = 0;

  n.getNote().center = true; return n;
  }


NotePad& NotePad::doRight(  NotePad& n)    {
Note& note = n.getNote();

  if (!note.line.empty() || note.right || note.tab || note.beginLine) n.note = 0;

  n.getNote().right  = true; return n;
  }


NotePad& NotePad::doBeginLine( NotePad& n) {
Note& note = n.getNote();

  if (!note.line.empty() || note.right || note.tab) n.note = 0;

  n.getNote().beginLine = true; return n;
  }


NotePad& NotePad::doSetTab(NotePad& n, int v) {
  if (n.getNote().tabValue) n.note = 0;

  n.getNote().tabValue = v;   return n;
  }


NotePad& NotePad::doSetRTab(NotePad& n, int v) {
  if (n.getNote().rTabValue) n.note = 0;

  n.getNote().rTabValue = v;  return n;
  }


NotePad& NotePad::doEditBox(NotePad& n, int v) {

  if (n.getNote().editBoxX) n.note = 0;

  n.getNote().editBoxX = v; return n;
  }


void NotePad::noFontReq(NotePad& n) {
Note& note = n.getNote();

  if (!note.fFace.isEmpty() || note.fSize     || note.prevFont  || note.bold || note.italic ||
                               note.underline || note.strikeOut || !note.line.empty()) n.note = 0;
  }


NotePad& NotePad::append(int v) {
Note&    note    = getNote();
NotePad& notePad = append(intToString(v, note.width));

  note.width = 0; return notePad;
  }


NotePad& NotePad::append(ulong v) {
Note&    note    = getNote();
NotePad& notePad = append(intToString(v, note.width));

  note.width = 0; return notePad;
  }


NotePad& NotePad::append(double v) {
Note&    note    = getNote();
NotePad& notePad = append(dblToString(v, note.width, note.precision));

  note.width = 0; note.precision = 0; return notePad;
  }

