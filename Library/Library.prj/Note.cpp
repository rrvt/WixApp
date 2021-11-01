// Note
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.


#include "stdafx.h"
#include "Note.h"



Note::Note() {
  fSize = tabValue = rTabValue = width = precision = 0; leftMargin = editBoxX = -1;

  isTable = clrTabs = tab = center = right = beginLine = endLine = endPage = crlf = debug = false;

  fFace.clear(); bold = prevFont = italic = underline = strikeOut = false;
  }


Note* Note::clone() {NewAlloc(Note);   Note* p = AllocNode;  p->copy(*this);   return p;}


void Note::copy(Note& n) {
  isTable     = n.isTable;
  editBoxX    = n.editBoxX;
  clrTabs     = n.clrTabs;
  leftMargin  = n.leftMargin;
  tabValue    = n.tabValue;
  rTabValue   = n.rTabValue;
  tab         = n.tab;
  width       = n.width;
  precision   = n.precision;
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
  wrap        = n.wrap;
  line        = n.line;
  endLine     = n.endLine;
  endPage     = n.endPage;
  crlf        = n.crlf;
  debug       = n.debug;
  }

