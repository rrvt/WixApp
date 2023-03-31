// Text Operations


#pragma once
#include "ClipLine.h"
#include "Horiz.h"
#include "Wrap.h"

class  TxtOut;
class  DevCtx;
class  DevTabs;
class  VertMgmt;
struct NoteNmbr;


// Specifies what to do after wrap completes

enum AfterTxt {NilAftr,    FaceAftr,      SizeAftr,      BoldAftr,
               ItalicAftr, UnderLineAftr, StrikeOutAftr, PopAftr,
               clrTbsAftr, CRAftr,        LeftAftr,      TabAftr,
               CenterAftr, RightAftr,     BegULAfter,    NmbrAftr,
               EndULAftr,  DoCrlfAftr,    DoEndPgAftr
               };


struct TxtOps {
TxtOut&   txtOut;
DevCtx&   dvx;

Horiz&    horz;                               // Manage Horizontal movement in window or page
VertMgmt& vert;                               // Manage vertical movement in window or page

AfterTxt  afterTxt;                           // Operation to perform after text wrap concludes
double    afterVal;                           // The value to be used by the ater operation
String    fontFace;                           // Font Face when afterTxt chooses this option

double    pos;                                // position each line uses during wrap operation
Wrap      wrap;

  TxtOps() : txtOut(*(TxtOut*)0), dvx(*(DevCtx*)0), horz(*(Horiz*)0), vert(*(VertMgmt*)0), wrap(dvx) { }

  TxtOps(TxtOut& to, AfterTxt aftr, double aftrVal, TCchar* face);

 ~TxtOps();

  void clear();

  void start(bool wrapEnabled);

  bool operator() (String& sum) {pos = horz.pos();   return output(sum);}
  void cleanupWrap(String& sum);

  void afterTxtOut();

private:

  bool output(String& sum);
  void fragmentOut(String& frag);
  void clipTabInvert();
  void setMetric();

  void evalNmbr(NoteNmbr& nmbr);
  };

