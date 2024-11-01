// A Little Class to hold the details of a number (int, uint, double) width and precision
// (double only).


#include "pch.h"
#include "NoteNmbr.h"
#include "TxtOut.h"


static String nmbr;


String& NoteNmbr::operator() () {
String s   = stg();
int    lng = s.length();
int    excess;

  excess = (width >= 0 ? width : -width) - lng;

  if (excess > 0) {
    if (width > 0) {addSpcs(excess);   nmbr += s;   return nmbr;}
    if (width < 0) {nmbr = s;   addSpcs(excess);    return nmbr;}
    nmbr = s;
    }
  else nmbr = s;

  return nmbr;
  }


String& NoteNmbr::stg() {

  switch (typ) {
    case NilNmbrTyp : nmbr.clear();                                 break;
    case IntNmbrTyp : nmbr = intToString(  longVal, width);         break;
    case UIntNmbrTyp: nmbr = uintToString(uLongVal, width);         break;
    case DblNmbTyp  : nmbr = dblToString(   dblVal, width, prec);   break;
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
  }


// ----------------------------------

#if 0
String s;
int    lng;
int    nWidth;
int    excess;
String t;

  if (!nn.typ) return;

  s = nn.stg();   lng = s.length();   nWidth = nn.width;

  excess = (nWidth >= 0 ? nWidth : -nWidth) - lng;

#if 1

  if (nWidth > 0 && excess > 0) {addSpcs(t, excess);   t += s;   return t;}
  if (nWidth < 0 && excess > 0) {t = s;   addSpcs(t, excess);    return t;}
#else
  if (nWidth > 0 && excess > 0) movPos(tPos, tPos.getCharPos() + excess, ar);

  ar.write(s);   tPos.move(s.length());

  if (nWidth < 0 && excess > 0) movPos(tPos, tPos.getCharPos() + excess, ar);
#endif
#endif

