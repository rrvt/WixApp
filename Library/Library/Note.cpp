// Note
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.


#include "pch.h"
#include "Note.h"
#include "NoteNmbr.h"



Note::Note() : noteTab() {
  fSize = 0; leftMgn = rightMgn = -1;   nmbr.clear();

  clrTabs = tabSeen = center = right = beginLine = endLine = endPage = crlf = debug = false;

  fFace.clear(); bold = prevFont = italic = underline = strikeOut = false;
  }


Note* Note::clone() {NewAlloc(Note);   Note* p = AllocNode;  p->copy(*this);   return p;}


void Note::copy(Note& n) {

  leftMgn     = n.leftMgn;
  rightMgn    = n.rightMgn;

  fFace       = n.fFace;
  fSize       = n.fSize;
  bold        = n.bold;
  italic      = n.italic;
  underline   = n.underline;
  strikeOut   = n.strikeOut;
  prevFont    = n.prevFont;

  clrTabs     = n.clrTabs;
  noteTab     = n.noteTab;

  tabSeen     = n.tabSeen;
  center      = n.center;
  right       = n.right;

  beginLine   = n.beginLine;

  line        = n.line;
  nmbr        = n.nmbr;

  endLine     = n.endLine;
  crlf        = n.crlf;
  endPage     = n.endPage;
  debug       = n.debug;
  }


bool Note::isAfter(NoteOrdr attr) {

  switch (attr) {
    case LeftMgnNO       : if (leftMgn  >= 0)    return true;
    case RightMgnNO      : if (rightMgn >= 0)    return true;

    case FontFaceNO      : if (!fFace.isEmpty()) return true;
    case FontSizeNO      : if (fSize != 0.0)     return true;
    case BoldNO          : if (bold)             return true;
    case ItalicNO        : if (italic)           return true;
    case UnderlineFontNO : if (underline)        return true;
    case StrikeOutNO     : if (strikeOut)        return true;
    case PrevFontNO      : if (prevFont)         return true;

    case ClrTabsNO       : if (clrTabs)          return true;
    case SetTabNO        : if (noteTab.pos)      return true;

    case TabNO           : if (tabSeen)          return true;
    case CenterNO        : if (center)           return true;
    case RightInLineNO   : if (right)            return true;

    case BegUndrlineNO   : if (beginLine)        return true;
    case LineNO          : if (!line.isEmpty())  return true;
    case NmbrNO          : if (nmbr.typ)         return true;
    case EndUndrlineNO   : if (endLine)          return true;

    case CrlfNO          : if (crlf)             return true;
    case EndPageNO       : if (endPage)          return true;
    case DebugNO         : if (debug)            return true;
    }
  return false;
  }


