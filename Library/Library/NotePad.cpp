// NotePad
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.

#include "pch.h"
#include "NotePad.h"
//#include "Archive.h"
//#include "NtPdToArch.h"


NotePad notePad;


static NtManipInt& setupManipInt(NtManipInt::Func func, int     val);
static NtManipDbl& setupManipDbl(NtManipDbl::Func func, double  val);
static NtManipStg& setupManipStg(NtManipStg::Func func, TCchar* val);

NoteManip nClrTabs;         // add to stream to clear tabs:                  dsp << dClrTabs;
NoteManip nCrlf;            // add to stream to terminate a line on display: dsp << "xyz" << dCrlf;
NoteManip nEndPage;         // add to stream to terminate page when printing otherwise ignore
NoteManip nTab;             // add to stream to tab to next tab position:    dsp << dTab << "xyz";
NoteManip nCenter;          // Set bit to center from here to crlf;
NoteManip nRight;           // Set bit to right align from here to crlf;
NoteManip nBeginLine;       // Begin and end line under text.
NoteManip nEndLine;
NoteManip nSetHex;          // Specify that next integer (unsigned or signed) be output as hex
NoteManip nDebug;
NoteManip nFont;            // set previous font
NoteManip nBold;            // set font to bold
NoteManip nItalic;          // set font to italic
NoteManip nUnderLine;       // underline font
NoteManip nStrikeOut;       // strike out font

NtManipDbl& nSetLMargin(double val) {return setupManipDbl(NotePad::doSetLMargin, val);}
NtManipDbl& nSetRMargin(double val) {return setupManipDbl(NotePad::doSetRMargin, val);}

NtManipStg& nFFace( TCchar* face){return setupManipStg(NotePad::doFFace,      face);}
NtManipDbl& nFSize(  double val) {return setupManipDbl(NotePad::doFSize,      val);}

NtManipInt& nSetTab(    int val) {return setupManipInt(NotePad::doSetTab,     val);}
NtManipInt& nSetRTab(   int val) {return setupManipInt(NotePad::doSetRTab,    val);}

NtManipInt& nSetWidth(  int val) {return setupManipInt(NotePad::doSetWidth,   val);}
NtManipInt& nSetPrec(   int val) {return setupManipInt(NotePad::doSetPrec,    val);}

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

  nBold.n      = this; nBold.func      = NotePad::doBoldFont;
  nItalic.n    = this; nItalic.func    = NotePad::doItalicFont;
  nUnderLine.n = this; nUnderLine.func = NotePad::doUnderLineFont;
  nStrikeOut.n = this; nStrikeOut.func = NotePad::doStrikeOutFont;
  nFont.n      = this; nFont.func      = NotePad::doPrevFont;

  nClrTabs.n   = this; nClrTabs.func   = NotePad::doClrTabs;

  nTab.n       = this; nTab.func       = NotePad::doTab;
  nCenter.n    = this; nCenter.func    = NotePad::doCenter;
  nRight.n     = this; nRight.func     = NotePad::doRight;

  nBeginLine.n = this; nBeginLine.func = NotePad::doBeginLine;
  nEndLine.n   = this; nEndLine.func   = NotePad::doEndLine;

  nSetHex.n    = this; nSetHex.func    = NotePad::doSetHex;

  nCrlf.n      = this; nCrlf.func      = NotePad::doCrlf;
  nEndPage.n   = this; nEndPage.func   = NotePad::doEndPage;
  nDebug.n     = this; nDebug.func     = NotePad::doDebug;

  noLines = 0;
  }




NotePad& NotePad::doSetLMargin(NotePad& n, double v)
                                                  {n.getNote(LeftMgnNO).leftMgn = v;     return n;}
NotePad& NotePad::doSetRMargin(NotePad& n, double v)
                                                  {n.getNote(RightMgnNO).rightMgn = v;   return n;}

NotePad& NotePad::doClrTabs(   NotePad& n)        {n.getNote(ClrTabsNO).clrTabs = true;  return n;}
NotePad& NotePad::doSetTab(    NotePad& n, int v)
                                                  {n.getNote(SetTabNO).noteTab.set(v);   return n;}
NotePad& NotePad::doSetRTab(   NotePad& n, int v)
                                              {n.getNote(SetTabNO).noteTab.set(v, true); return n;}

NotePad& NotePad::doTab(       NotePad& n)      {n.getNote(TabNO).tabSeen       = true; return n;}
NotePad& NotePad::doCenter(    NotePad& n)      {n.getNote(CenterNO).center     = true; return n;}
NotePad& NotePad::doRight(     NotePad& n)      {n.getNote(RightInLineNO).right = true; return n;}

NotePad& NotePad::doBeginLine( NotePad& n) {n.getNote(BegUndrlineNO).beginLine = true; return n;}
NotePad& NotePad::doEndLine(   NotePad& n) {n.getNote(EndUndrlineNO).endLine   = true; return n;}


NotePad& NotePad::append(String const& s) {getNote(NmbrNO).line += s;        return *this;}
NotePad& NotePad::append(TCchar* tc)      {getNote(NmbrNO).line += tc;       return *this;}

NotePad& NotePad::append(Tchar   v)       {getNote(NmbrNO).line += v;        return *this;}
NotePad& NotePad::append(Cchar* cs)
                          {ToUniCode uni(cs); getNote(NmbrNO).line += uni(); return *this;}
NotePad& NotePad::append(Date    v)       {String s = v;   return append((TCchar*)s);}


NotePad& NotePad::doSetWidth(  NotePad& n, int v) {n.getNote(NmbrNO).nmbr.width = v;  return n;}
NotePad& NotePad::doSetPrec(   NotePad& n, int v) {n.getNote(NmbrNO).nmbr.prec  = v;  return n;}
NotePad& NotePad::doSetHex(    NotePad& n)        {n.getNote(NmbrNO).nmbr.hex = true; return n;}

NotePad& NotePad::doNmbr(long   v)
            {getNote(NmbrNO).nmbr.longVal  = v;   note->nmbr.typ = IntNmbrTyp;    return *this;}
NotePad& NotePad::doNmbr(ulong  v)
            {getNote(NmbrNO).nmbr.uLongVal = v;   note->nmbr.typ = UIntNmbrTyp;   return *this;}
NotePad& NotePad::doNmbr(double v)
            {getNote(NmbrNO).nmbr.dblVal   = v;   note->nmbr.typ = DblNmbTyp;     return *this;}


NotePad& NotePad::crlf() {noLines++;   getNote(CrlfNO).crlf = true;                return *this;}
NotePad& NotePad::endPage()
                  {if (noLines) {getNote(EndPageNO).endPage = true; noLines = 0;}  return *this;}





///----------------------------

#if 0
String NotePad::archive(NoteNmbr& nn, TextPosition& tPos, Archive& ar) {
String s;
int    lng;
int    nWidth;
int    excess;
String t;

  if (!nn.typ) return;

  s = nn.stg();   lng = s.length();   nWidth = nn.width;

  excess = (nWidth >= 0 ? nWidth : -nWidth) - lng;

#if 1

  if (nWidth > 0 && excess > 0) {addSpcs(t, excess);   t += s;   return t;}
  if (nWidth < 0 && excess > 0) {t = s;   addSpcs(t, excess);    return t;}
#else
  if (nWidth > 0 && excess > 0) movPos(tPos, tPos.getCharPos() + excess, ar);

  ar.write(s);   tPos.move(s.length());

  if (nWidth < 0 && excess > 0) movPos(tPos, tPos.getCharPos() + excess, ar);
#endif
  }
#endif
#if 1
#else

NtPdIter     iter(*this);
Note*        nt;
TextPosition tPos;
Tab          tab;
bool         hasTab;
bool         rightTab;
int          cnt;
String       s;

  for (nt = iter(); nt; nt = iter++) {

//  if (nt->leftMgn  >= 0) tPos.leftMgn = nt->leftMgn;
//  if (nt->rightMgn >= 0) tPos.rightMgn = nt->rightMgn;

    if (nt->clrTabs)   tPos.clrTabs();
    if (nt->tabValue)  tPos.setTab(applyTabFactor(nt->tabValue), nt->rightTab);

    hasTab = nt->tab;

    if (hasTab) {
      tab = tPos.findNextTab();   rightTab = tab.right;

      if (!rightTab) movPos(tPos, tab.pos, ar);
      }

    if (nt->right) {tab.pos = arWidth; tab.right = rightTab = hasTab = true;}

    if      (!nt->nmbr.isEmpty()) s = nt->nmbr();
    else if (!nt->line.isEmpty()) s = nt->line;
    else                          s.clear();

    cnt = s.length();

    if (nt->center) movPos(tPos, (arWidth - cnt) / 2, ar);

    if (hasTab && rightTab) movPos(tPos, tab.pos - cnt, ar);

    if (nt->beginLine) undrLn.begin(tPos.get());

    if (cnt) {ar << s; tPos.move(cnt);}

    if (nt->endLine) undrLn.end(tPos.get());

    if (nt->crlf) {
      ar << _T('\n');   tPos.doCR();

      if (!undrLn.isEmpty()) {
        s = undrLn();   ar << s;   tPos.move(s.length());   ar << _T('\n');   tPos.doCR();
        }
      }
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
#endif

