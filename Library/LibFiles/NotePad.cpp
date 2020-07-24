// NotePad
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.

#include "stdafx.h"
#include "NotePad.h"
#include "TextPosition.h"


NotePad notePad;



static NoteManip1& setupManip1(NoteManip1::Func func, int val);


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

NoteManip1& nSetTab(    int val) {return setupManip1(NotePad::doSetTab,     val);}
NoteManip1& nSetRTab(   int val) {return setupManip1(NotePad::doSetRTab,    val);}
NoteManip1& nSetLMargin(int val) {return setupManip1(NotePad::doSetLMargin, val);}
NoteManip1& nSetWidth(  int val) {return setupManip1(NotePad::doSetWidth,   val);}
NoteManip1& nSetPrec(   int val) {return setupManip1(NotePad::doSetPrec,    val);}
NoteManip1& nEditBox(   int val) {return setupManip1(NotePad::doEditBox,    val);}
NoteManip1& nFSize(     int val) {return setupManip1(NotePad::doFSize,      val);}

NoteManip1& setupManip1(NoteManip1::Func func, int val)
          {NewAlloc(NoteManip1);   NoteManip1* m = AllocNode;  new(m) NoteManip1(func, val); return *m;}


NotePad::NotePad() {note = 0; noLines = 0; initialize();}


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
NotePadLoop  notePadLp;
Note*        note;
TextPosition tPos;
Tab          tab;
bool         hasTab;
bool         rightTab;
int          x;
int          cnt;

  for (note = notePadLp.start(); note; note = notePadLp.nextNode()) {

    if (note->clrTabs)   tPos.clrTabs();
    if (note->tabValue)  tPos.setTab(note->tabValue);
    if (note->rTabValue) tPos.setRTab(note->rTabValue);

    hasTab = note->tab;

    if (hasTab) {
      tab = tPos.findNextTab();   rightTab = tab.right;

      if (!rightTab)
        for (x = tPos.getCharPos(); x < tab.pos; x++) {ar << _T(" "); tPos.move(1);}
      }

    cnt = note->line.length();

    if (cnt) {
      if (hasTab && rightTab) {
        for (x = tPos.getCharPos(); x < tab.pos - cnt; x++) {ar << _T(" "); tPos.move(1);}
        }

      ar.write(note->line); tPos.move(cnt);
      }

    if (note->crlf) {ar.write(_T('\n'));   tPos.doCR();}
    }
  }


NotePad& NotePad::append(Wrap& w) {
Note& note = getNote();  if (!note.line.isEmpty()) this->note = 0;

  getNote().wrap = w; return *this;
  }



NotePad& NotePad::doClrTabs(NotePad& n) {
Note& note = n.getNote();

  if (!note.line.empty() || note.clrTabs || note.right || note.tab ||
                                                             note.tabValue || note.rTabValue) n.note = 0;
  n.getNote().clrTabs   = true; return n;
  }


NotePad& NotePad::doTab(NotePad& n) {
Note& note = n.getNote();

  if (!note.line.empty() || note.tab || note.beginLine || note.right) n.note = 0;

  n.getNote().tab = true; return n;
  }


NotePad& NotePad::crlf() {
  getNote().crlf    = true; note->line; note = 0; noLines++; return *this;
  }


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


NotePad& NotePad::doSetLMargin(NotePad& n, int v) {
  if (n.getNote().leftMargin) n.note = 0;

  n.getNote().leftMargin = v; return n;
  }


NotePad& NotePad::doSetTab(NotePad& n, int v) {
  if (n.getNote().tabValue) n.note = 0;

  n.getNote().tabValue = v; return n;
  }


NotePad& NotePad::doSetRTab(NotePad& n, int v) {
  if (n.getNote().rTabValue) n.note = 0;

  n.getNote().rTabValue = v; return n;
  }


NotePad& NotePad::doEditBox(NotePad& n, int v) {

  if (n.getNote().editBoxX) n.note = 0;

  n.getNote().editBoxX = v; return n;
  }


void NotePad::noFontReq(NotePad& n) {
Note& note = n.getNote();

  if (note.fSize || note.prevFont || note.bold || note.italic || note.underline ||
                                                        note.strikeOut || !note.line.empty()) n.note = 0;
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

