// Print Device


#pragma once
#include "DevPrntr.h"
#include "ToDevBase.h"


class NtPdToPrtr : public ToDevBase {

DevPrntr dev;

public:
           NtPdToPrtr(NotePad& np) : ToDevBase(np, dev) { }
          ~NtPdToPrtr() { }

  void     prepare(CDC* dc, CPrintInfo* pInfo)     {dev.prepare(dc, pInfo);}
  void     initFont(TCchar* face, double fontSize) {dev.initFont(face, fontSize);}

  bool     isEndDoc()   {return endDoc && dev.isWrapFin();}

  void     suppressOutput() {dev.suppressOutput();}
  void     negateSuppress() {dev.negateSuppress();}
  void     disableWrap()    {dev.disableWrap();}
  void     enableWrap()     {dev.enableWrap();}

private:

  NtPdToPrtr() : ToDevBase(*(NotePad*)0, *(DevBase*)0) { }
  };



