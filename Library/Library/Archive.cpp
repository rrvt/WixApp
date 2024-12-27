// Replaces CArchive to allow for Unicode internally and ansi in external files
// Copyright Bob -- K6RWY, 2019


#include "pch.h"
#include "Archive.h"
#include "NotePad.h"


ArManip aClrTabs;
ArManip aTab;
ArManip aCenter;
ArManip aRight;

ArManip aBeginLine;
ArManip aEndLine;
ArManip aSetHex;
ArManip aCrlf;            // add to stream to terminate a line on display: dsp << "xyz" << dCrlf;


static ArManipInt& setupManipInt(ArManipInt::Func func, int    val);
static ArManipDbl& setupManipDbl(ArManipDbl::Func func, double val);



Archive::Archive(String& fileName, int mode) : ArchPos(fileName, mode) {initialize();}



void Archive::initialize() {

  aClrTabs.n   = this; aClrTabs.func   = Archive::doClrTabs;
  aTab.n       = this; aTab.func       = Archive::doTab;
  aCenter.n    = this; aCenter.func    = Archive::doCenter;
  aRight.n     = this; aRight.func     = Archive::doRight;
  aBeginLine.n = this; aBeginLine.func = Archive::doBeginLine;
  aEndLine.n   = this; aEndLine.func   = Archive::doEndLine;
  aSetHex.n    = this; aSetHex.func    = Archive::doSetHex;
  aCrlf.n      = this; aCrlf.func      = Archive::doCrlf;
  }




Archive& Archive::operator << (NotePad& np) {
NtPdIter iter(np);
Note*    nt;

  arLine.clear();   nmbr.clear();

  for (nt = iter(); nt; nt = iter++) {
    if (nt->leftMgn  >= 0)   setLMargin(nt->leftMgn);
    if (nt->rightMgn >= 0)   setRMargin(nt->rightMgn);
    if (nt->clrTabs)         clrTabs();
    if (nt->noteTab.pos) {   if (nt->noteTab.right) setRTab(nt->noteTab.pos);
                             else                   setTab( nt->noteTab.pos);
                             }
    if (nt->tabSeen)         tab();
    if (nt->center)          center();
    if (nt->right)           right();
    if (nt->beginLine)       beginULine();
    if (!nt->line.isEmpty()) arLine.append(nt->line);
    if (nt->nmbr.typ)       {nmbr = nt->nmbr;  append(nmbr());}
    if (nt->endLine)         endULine();
    if (nt->crlf)            crlf();
    }

  flush();   return *this;
  }


Archive& Archive::append(Cchar*  cs) {ToUniCode uni(cs);   return append(uni());}

Archive& Archive::append(long v)
     {nmbr.longVal = v;   nmbr.typ = IntNmbrTyp;    append(nmbr());   nmbr.clear();  return *this;}


Archive& Archive::append(ulong v)
     {nmbr.longVal = v;   nmbr.typ = UIntNmbrTyp;   append(nmbr());   nmbr.clear();  return *this;}


Archive& Archive::append(double v)
     {nmbr.dblVal = v;    nmbr.typ = DblNmbTyp;     append(nmbr());   nmbr.clear();  return *this;}


Archive& Archive::append(Date& dt) {String s = dt;   return append(s);}


Archive& Archive::flush() {write(arLine);   return *this;}


// Set left margin (no. chars)

ArManipDbl& aSetLMargin(double val)  {return setupManipDbl(Archive::doSetLMargin, val);}
Archive& Archive::doSetLMargin(Archive& n, double v) {n.setLMargin(v);    return n;}


ArManipDbl& aSetRMargin(double val)  {return setupManipDbl(Archive::doSetRMargin, val);}
Archive& Archive::doSetRMargin(Archive& n, double v) {n.setRMargin(v);    return n;}

// Set width (length) of line in file (no. chars)

ArManipInt& aPageWidth(int val)  {return setupManipInt(Archive::doPageWidth, val);}
Archive& Archive::doPageWidth(Archive& n, int v) {n.setPageWidth(v);    return n;}


ArManipInt& aSetTab(int val) {return setupManipInt(Archive::doSetTab, val);}
Archive& Archive::doSetTab(Archive& n, int v) {n.setTab(v);    return n;}

ArManipInt& aSetRTab(int val) {return setupManipInt(Archive::doSetRTab, val);}
Archive& Archive::doSetRTab(Archive& n, int v) {n.setRTab(v);    return n;}


ArManipInt& aSetWidth(int val) {return setupManipInt(Archive::doSetWidth, val);}
Archive& Archive::doSetWidth(Archive& n, int v) {n.nmbr.width = v;  return n;}


ArManipInt& aSetPrec( int val) {return setupManipInt(Archive::doSetPrec,  val);}
Archive& Archive::doSetPrec(Archive& n, int v) {n.nmbr.prec  = v;  return n;}


ArManipInt& setupManipInt(ArManipInt::Func func, int val)
          {NewAlloc(ArManipInt);   ArManipInt* m = AllocNode;  m->set(func, val); return *m;}

ArManipDbl& setupManipDbl(ArManipDbl::Func func, double val)
          {NewAlloc(ArManipDbl);   ArManipDbl* m = AllocNode;  m->set(func, val); return *m;}

ArManipStg& setupManipStg(ArManipStg::Func func, TCchar* val)
          {NewAlloc(ArManipStg);   ArManipStg* m = AllocNode;  m->set(func, val); return *m;}



int Archive::spaces(int nSpaces)
                {int i;   for (i = 0; i < nSpaces; i++) arLine.append(_T(' '));   return nSpaces;}


