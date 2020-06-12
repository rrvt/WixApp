// Note
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.


#include "stdafx.h"
#include "Note.h"



Note::Note() {
  fSize = leftMargin = tabValue = rTabValue = width = precision = 0; editBoxX = -1;

  isTable = clrTabs = tab = center = right = beginLine = endLine = endPage = crlf = debug = false;

  bold = prevFont = italic = underline = strikeOut = false;
  }


Note* Note::clone() {NewAlloc(Note);   Note* p = AllocNode;  p->line = line;  return p;}

