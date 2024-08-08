// A Little Class to hold the details of a number (int, uint, double) width and precision
// (double only).


#include "pch.h"
#include "NoteNmbr.h"
#include "TxtOut.h"


String NoteNmbr::stg() {

  switch (typ) {
    case NilNmbrTyp : break;
    case IntNmbrTyp : return intToString(  longVal, width);
    case UIntNmbrTyp: return uintToString(uLongVal, width);
    case DblNmbTyp  : return dblToString(   dblVal, width, prec);
    }
  return _T("");
  }



void NoteNmbr::copy(NoteNmbr& nn) {
  typ = nn.typ;

  switch (typ) {
    case NilNmbrTyp : dblVal   = 0;           break;
    case IntNmbrTyp : longVal  = nn.longVal;  break;
    case UIntNmbrTyp: uLongVal = nn.uLongVal; break;
    case DblNmbTyp  : dblVal   = nn.dblVal;   break;
    }

  width = nn.width;
  prec  = nn.prec;
  }



