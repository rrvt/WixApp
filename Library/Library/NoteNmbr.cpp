// A Little Class to hold the details of a number (int, uint, double) width and precision
// (double only).


#include "pch.h"
#include "NoteNmbr.h"


static String nmbr;

String& NoteNmbr::operator() () {return stg();}


String& NoteNmbr::stg() {

  switch (typ) {
    case NilNmbrTyp : nmbr.clear();                                 break;
    case IntNmbrTyp : nmbr = hex ? hexToString( longVal,        prec) :
                                   intToString( longVal, width, prec);
                      break;
    case UIntNmbrTyp: nmbr = hex ? hexToString( uLongVal,        prec) :
                                   uintToString(uLongVal, width, prec);
                      break;
    case DblNmbTyp  : nmbr =       dblToString( dblVal,   width, prec);   break;
    }

  return nmbr;
  }


void NoteNmbr::addSpcs(int n) {for (int i = 0; i < n; i++) nmbr += _T(' ');}



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
  hex   = nn.hex;
  }


// ----------------------------------

