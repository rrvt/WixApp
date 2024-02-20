// Text Operations


#include "pch.h"
#include "TxtOps.h"
#include "DevBase.h"
#include "DevTabs.h"
#include "NoteNmbr.h"
#include "TxtOut.h"
#include "VertMgmt.h"


TxtOps::TxtOps(TxtOut& to, AfterTxt aftr, double aftrVal, TCchar* face) :
      txtOut(to),
      dvx(to.dvx),
      horz(to.horz),
      vert(to.dev.vert),
      afterTxt(aftr),
      afterVal(aftrVal),
      fontFace(face ? face : _T("")),
      pos(0),
      wrap(dvx)
      { }


TxtOps::~TxtOps() { }


void TxtOps::clear() {afterTxt = NilAftr;   afterVal = 0;   fontFace.clear();   pos = 0;   wrap.clear();}


void TxtOps::start(bool wrapEnabled) {wrap.set(wrapEnabled, horz.remaining(), horz.maxExtent());}


void TxtOps::cleanupWrap(String& sum) {txtOut.nonBlankLine = true;   output(sum);}


bool TxtOps::output(String& sum) {

  while (wrap(sum)) {

    fragmentOut(wrap.stg());   txtOut.crlf();   horz.restorePos(pos);

    if (txtOut.dev.isEndPage()) return false;
    }

  fragmentOut(wrap.stg());   return true;
  }


void TxtOps::evalNmbr(NoteNmbr& nmbr) {
String s        = nmbr.stg();
int    w        = dvx.width(s);
int    nWidth   = nmbr.width;
double fldWidth = dvx.avgLgChWidth * (nWidth < 0 ? -nWidth : nWidth);
int    excess   = int(fldWidth - w);

  if (nWidth > 0 && excess > 0) horz.move(excess);

  fragmentOut(s);

  if (nWidth < 0 && excess > 0) horz.move(excess);
  }


void TxtOps::fragmentOut(String& frag) {
int w = dvx.width(frag);   if (!w) return;

  if (clipLine.isOpen()) clipTabInvert();

  clipLine.clipRegion(frag, horz.currentPos(), w, vert, dvx);

  dvx.txtOut(frag, horz.currentPos(), vert.pos());

  horz.move(w);   clipLine.setHzPos(horz.currentPos());
  }


void TxtOps::clipTabInvert() {
int    hzPos = clipLine.tabWidth(horz.currentPos(), dvx.lgChWidth());
int    w     = horz.currentPos() - hzPos;    if (w <= 0) return;
String s;

  do {s += _T(' ');} while (dvx.width(s) < w);

  dvx.txtOut(s, hzPos, vert.pos());
  }


void TxtOps::afterTxtOut() {

  switch (afterTxt) {
    case NilAftr      :                                                           break;
    case FaceAftr     : dvx.setFace(fontFace);      setMetric();                  break;
    case SizeAftr     : dvx.setSize(afterVal);      setMetric();                  break;
    case BoldAftr     : dvx.setBold();              setMetric();                  break;
    case ItalicAftr   : dvx.setItalic();            setMetric();                  break;
    case UnderLineAftr: dvx.setUnderLine();         setMetric();                  break;
    case StrikeOutAftr: dvx.setStrikeOut();         setMetric();                  break;
    case PopAftr      : dvx.pop();                  setMetric();                  break;

    case CRAftr       : horz.cr();                                                break;
    case LeftAftr     : dvx.leftMgn = afterVal;  horz.setLftEdge();               break;
    case clrTbsAftr   : txtOut.devTabs.clear();                                   break;
    case TabAftr      : txtOut.tab(); txtOut.findNextTab();                       break;
    case BegULAfter   : txtOut.undrLn.setBegin(horz.currentPos(), int(afterVal)); break;
    case EndULAftr    : txtOut.undrLn.setEnd(  horz.currentPos(), int(afterVal));
                        txtOut.undrLn.output(dvx);                                break;
    case CenterAftr   : txtOut.center = true;                                     break;
    case RightAftr    : txtOut.right  = true;                                     break;

    case NmbrAftr     : evalNmbr(txtOut.nmbr);                                    break;
    case DoCrlfAftr   : if (txtOut.nonBlankLine) txtOut.crlf();                   break;
    case DoEndPgAftr  : txtOut.dev.setEndPage();   horz.cr();                     break;
    }
  }


void TxtOps::setMetric() {horz.setEdges(); vert.getMaxHeight();}



