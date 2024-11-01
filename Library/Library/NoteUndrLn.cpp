// Capture Under Line Position in Archive Output and create an " ---" underline


#include "pch.h"
#include "NoteUndrLn.h"



String& NoteUndrLn::operator() () {
int i;

  s.clear();

  if (endPos <= beginPos) return s;

  for (i = 0;        i < beginPos; i++) s += _T(' ');
  for (i = beginPos; i < endPos;   i++) s += _T('-');

  beginPos = endPos = 0;

  return s;
  }

