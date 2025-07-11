// Text Output to device


#include "pch.h"
#include "TextAccm.h"
#include "Device.h"
#include "VertMgmt.h"



TextAccm::TextAccm() : tabSeen(false),   center(false),       right(false),
                       begUndrLn(false), endUndrLn(false),    nonBlankLine(false) { }



void TextAccm::initialize() {

  undrLn.clear();   nmbr.clear();   sum.clear();

  tabSeen = center = right = begUndrLn = endUndrLn = nonBlankLine = false;
  }


void TextAccm::clear() {

  Wrap::clear();   devTabs.clear();   undrLn.clear();   nmbr.clear();   sum.clear();

  pos = 0;

  tabSeen = center = right = begUndrLn = endUndrLn = nonBlankLine = false;
  }


void TextAccm::copy(TextAccm& tac) {
  Wrap::copy((Wrap&) tac);
  devTabs      = tac.devTabs;
  undrLn       = tac.undrLn;
  nmbr         = tac.nmbr;
  pos          = tac.pos;
  tabSeen      = tac.tabSeen;
  center       = tac.center;
  right        = tac.right;
  begUndrLn    = tac.begUndrLn;
  sum          = tac.sum;
  endUndrLn    = tac.endUndrLn;
  nonBlankLine = tac.nonBlankLine;
  }



void TextAccm::flush(bool newTab) {
int wth = width(sum);                                                   //    closeClip();

  if (tabSeen) {
    RealTab curTab = devTabs.findNextTab(horz);

    if (!curTab.right) {horz.set(curTab.pos);        tabSeen = false;}
    else               {horz.set(curTab.pos - wth);  tabSeen = false;}
    }

  if (center && wth) {horz.centerText(wth); center = false;}

  if (right && wth)  {horz.rightText(wth);  right  = false;}

  if (begUndrLn) {
    undrLn.setBegin(horz.currentPos(), vert.getUlinePos());  begUndrLn = false;
    }

  if (!sum.isEmpty()) write(isEnabled());

  if (endUndrLn) {
    undrLn.setEnd(horz.currentPos(), vert.getUlinePos());
    undrLn.output((DeviceContext&) *this);  endUndrLn = false;
    }
  }


void TextAccm::write(bool wrapEnabled) {

  setWrap(wrapEnabled, horz.remaining(), horz.maxExtent());

  nonBlankLine = true;   pos = horz.pos();    output();
  }


void TextAccm::cleanupWrap() {nonBlankLine = true;   output();}


void TextAccm::output() {

  while (wrap(sum)) {

    fragmentOut(Wrap::stg());   crlf();   horz.restorePos(pos);

    if (vert.isEndPage()) return;
    }

  fragmentOut(Wrap::stg());
  }


void TextAccm::fragmentOut(String& frag) {
int w = width(frag);   if (!w) return;
#if 0
  if (clipLine.isOpen()) clipTabInvert();

  clipLine.clipRegion(frag, horz.currentPos(), w, vert, dvx);
#endif
  txtOut(frag, horz.currentPos(), vert.pos());

  horz.move(w);   clipLine.setHzPos(horz.currentPos());
  }



void TextAccm::crlf() {

  flush();

  if (!nonBlankLine) return;

  lf();   cr();   //   clipLine.setHzPos(horz.currentPos());  clipLine.close(dvx);
  }
                    //clrPositions();
