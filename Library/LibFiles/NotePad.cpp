// NotePad
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.

#include "stdafx.h"
#include "NotePad.h"
#include "TextPosition.h"


NotePad notePad;
String  dbgLine;


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

    if (note->crlf) {ar.write(_T('\n'));   tPos.crlf();}
    }
  }



static void noFontReq(NotePad& n);


// Manipulator functions -- do not call directly
static NotePad& doCrlf(      NotePad& n) {return n.crlf();}
static NotePad& doEndPage(   NotePad& n) {return n.endPage();}
static NotePad& setTableName(NotePad& n) {n.getNote().isTable   = true; return n;}
static NotePad& doEndLine(   NotePad& n) {n.getNote().endLine   = true; return n;}
static NotePad& doDebug(     NotePad& n) {n.getNote().debug     = true; return n;}




static NotePad& doClrTabs(   NotePad& n) {
Note& note = n.getNote();

  if (!note.line.empty() || note.clrTabs || note.right || note.tab ||
                                                              note.tabValue || note.rTabValue) n.note = 0;

  n.getNote().clrTabs   = true; return n;
  }


static NotePad& doBeginLine( NotePad& n) {
Note& note = n.getNote();

  if (!note.line.empty() || note.right || note.tab) n.note = 0;

  n.getNote().beginLine = true; return n;
  }


static NotePad& doCenter( NotePad& n)    {
Note& note = n.getNote();

  if (!note.line.empty()) n.note = 0;

  n.getNote().center = true; return n;
  }


static NotePad& doRight(  NotePad& n)    {
Note& note = n.getNote();

  if (!note.line.empty() || note.right || note.tab || note.beginLine) n.note = 0;

  n.getNote().right  = true; return n;
  }

static NotePad& doTab(NotePad& n) {
Note& note = n.getNote();

  if (!note.line.empty() || note.tab || note.beginLine || note.right) n.note = 0;

  n.getNote().tab = true; return n;
  }


// no Argument Manipulator

NoteManip nClrTabs;             // add to stream to clear tabs:                  dsp << dClrTabs;
NoteManip nCrlf;                // add to stream to terminate a line on display: dsp << "xyz" << dCrlf;
NoteManip nEndPage;             // add to stream to terminate page when printing otherwise ignore
NoteManip nTab;                 // add to stream to tab to next tab position:    dsp << dTab << "xyz";
NoteManip nTableName;           // Set bit to identify table name note:        dsp << dTableName << "xyz";
NoteManip nCenter;              // Set bit to center from here to crlf;
NoteManip nRight;               // Set bit to right align from here to crlf;
NoteManip nBeginLine;
NoteManip nEndLine;             // Begin and end line under text.
NoteManip nDebug;
NoteManip nFont;                // set previous font
NoteManip nBold;                // set font to bold
NoteManip nItalic;              // set font to italic
NoteManip nUnderLine;           // underline font
NoteManip nStrikeOut;           // strike out font


NotePad& doSetLMargin(NotePad& n, int v) {
  if (n.getNote().leftMargin) n.note = 0;

  n.getNote().leftMargin = v; return n;
  }


static NotePad& doSetTab(NotePad& n, int v) {
  if (n.getNote().tabValue) n.note = 0;

  n.getNote().tabValue = v; return n;
  }


static NotePad& doSetRTab(NotePad& n, int v) {
  if (n.getNote().rTabValue) n.note = 0;

  n.getNote().rTabValue = v; return n;
  }

static NotePad& doSetWidth(NotePad& n, int val) {
  n.getNote().width = val; return n;
  }

static NotePad& doSetPrec(NotePad& n, int prec) {
  n.getNote().precision = prec; return n;
  }


static NotePad& doEditBox(NotePad& n, int v) {

  if (n.getNote().editBoxX) n.note = 0;

  n.getNote().editBoxX = v; return n;
  }


static NotePad& doFSize(NotePad& n, int v) {
  noFontReq(n);  n.getNote().fSize = v; return n;
  }


static NotePad& doPrevFont(NotePad& n) {
  noFontReq(n);  n.getNote().prevFont = true; return n;
  }


NotePad& doBoldFont( NotePad& n) {
  noFontReq(n);    n.getNote().bold = true; return n;
  }


NotePad& doItalicFont(NotePad& n) {
  noFontReq(n);    n.getNote().italic = true; return n;
  }


NotePad& doUnderLineFont(NotePad& n) {
  noFontReq(n);    n.getNote().underline = true; return n;
  }


NotePad& doStrikeOutFont(NotePad& n) {

  noFontReq(n);    n.getNote().strikeOut = true; return n;
  }



static void noFontReq(NotePad& n) {
Note& note = n.getNote();

  if (note.fSize || note.prevFont || note.bold || note.italic || note.underline ||
                                                        note.strikeOut || !note.line.empty()) n.note = 0;
  }




NoteManip1& nSetLMargin(int val)  {NoteManip1* m = new NoteManip1(doSetLMargin, val);  return *m;}
NoteManip1& nSetTab(    int val)  {NoteManip1* m = new NoteManip1(doSetTab,     val);  return *m;}
NoteManip1& nSetRTab(   int val)  {NoteManip1* m = new NoteManip1(doSetRTab,    val);  return *m;}
NoteManip1& nSetWidth(  int val)  {NoteManip1* m = new NoteManip1(doSetWidth,   val);  return *m;}
NoteManip1& nSetPrec(   int prec) {NoteManip1* m = new NoteManip1(doSetPrec,    prec); return *m;}

NoteManip1& nFSize(     int val)  {NoteManip1* m = new NoteManip1(doFSize,      val);  return *m;}
NoteManip1& nEditBox(   int val)  {NoteManip1* m = new NoteManip1(doEditBox,    val);  return *m;}


NotePad::NotePad() {note = 0; noLines = 0; maxLines = 65;}


Note& NotePad::getNote() {
  if (!note) {note = new Note; noteList.append(note);}

  return *note;
  }


NotePad& NotePad::crlf() {
  getNote().crlf    = true; dbgLine = note->line; note = 0; noLines++;

  if (noLines > maxLines) endPage();

  return *this;
  }


NotePad& NotePad::endPage() {

  if (noLines) {note = 0; getNote().endPage = true; dbgLine = note->line; note = 0; noLines = 0;}

  return *this;
  }



void NotePad::open() {
  nClrTabs.n   = this; nClrTabs.func   = doClrTabs;
  nCrlf.n      = this; nCrlf.func      = doCrlf;
  nEndPage.n   = this; nEndPage.func   = doEndPage;
  nTab.n       = this; nTab.func       = doTab;
  nTableName.n = this; nTableName.func = setTableName;
  nCenter.n    = this; nCenter.func    = doCenter;
  nRight.n     = this; nRight.func     = doRight;
  nBeginLine.n = this; nBeginLine.func = doBeginLine;
  nEndLine.n   = this; nEndLine.func   = doEndLine;
  nDebug.n     = this; nDebug.func     = doDebug;
  nFont.n      = this; nFont.func      = doPrevFont;
  nBold.n      = this; nBold.func      = doBoldFont;
  nItalic.n    = this; nItalic.func    = doItalicFont;
  nUnderLine.n = this; nUnderLine.func = doUnderLineFont;
  nStrikeOut.n = this; nStrikeOut.func = doStrikeOutFont;

  noLines      = 0;
  }


// To clear notepad for new input.

void NotePad::close() {
  noteList.removeAll(); note = 0; open();
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


NotePad& NotePad::append(Date       v) {String s = v; return append(s);}          // ** Fix
NotePad& NotePad::append(CTimeSpan& v) {String s; return append(s);}

