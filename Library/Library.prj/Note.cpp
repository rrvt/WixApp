// Note
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.


#include "pch.h"
#include "Note.h"
#include "NoteNmbr.h"



Note::Note() {
  fSize = tabValue = 0; leftMargin = -1;   rightTab = false;   nmbr.clear();

  clrTabs = tab = center = right = beginLine = endLine = endPage = crlf = debug = false;

  fFace.clear(); bold = prevFont = italic = underline = strikeOut = false;
  }


Note* Note::clone() {NewAlloc(Note);   Note* p = AllocNode;  p->copy(*this);   return p;}


void Note::copy(Note& n) {
  clrTabs     = n.clrTabs;
  leftMargin  = n.leftMargin;
  tabValue    = n.tabValue;
  rightTab    = n.rightTab;
  tab         = n.tab;
  fFace       = n.fFace;
  fSize       = n.fSize;
  prevFont    = n.prevFont;
  bold        = n.bold;
  italic      = n.italic;
  underline   = n.underline;
  strikeOut   = n.strikeOut;
  center      = n.center;
  right       = n.right;
  beginLine   = n.beginLine;
  line        = n.line;
  nmbr        = n.nmbr;
  endLine     = n.endLine;
  endPage     = n.endPage;
  crlf        = n.crlf;
  debug       = n.debug;
  }




bool Note::isAfter(NoteAttr attr) {

  switch (attr) {
    case PFontNAttr  : if (prevFont)        return true;
    case LMgnNAttr   : if (leftMargin >= 0) return true;
    case ClrTbsNAttr : if (clrTabs)         return true;
    case StTbsNAttr  : if (tabValue)        return true;
    case TbNAttr     : if (tab)             return true;
    case CtrNAttr    : if (center)          return true;
    case RghtNAttr   : if (right)           return true;
    case BgLnNAttr   : if (beginLine)       return true;
    case LineNAttr   : if (!line.isEmpty()) return true;
    case NmbrNAttr   : if (nmbr.typ)        return true;
    case EndLnNAttr  : if (endLine)         return true;
    case CrlfNAttr   : if (crlf)            return true;
    case EndPgNAttr  : if (endPage)         return true;
    case DbgNAttr    : if (debug)           return true;
    }
  return false;
  }



//  startHdr = endHdr = false;
//  startHdr    = n.startHdr;
//  endHdr      = n.endHdr;
//  isTable     = n.isTable;
//  editBoxX    = n.editBoxX;
//  wrap        = n.wrap;

