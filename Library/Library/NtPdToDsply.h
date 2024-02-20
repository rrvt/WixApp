// Display Device


#pragma once
#include "ToDevBase.h"
#include "DevDsply.h"


class NtPdToDsply : public ToDevBase {

DevDsply dev;

public:
           NtPdToDsply(NotePad& np) : ToDevBase(np, dev) { }
          ~NtPdToDsply() { }

  void     prepare(CDC* dc) {dev.prepare(dc); dev.setBeginPage();}
  void     initFont(TCchar* face, double fontSize) {dev.initFont(face, fontSize);}


  void     disableWrap()              {dev.disableWrap();}
  void     enableWrap()               {dev.enableWrap();}

  int      lgChWidth()                {return dev.lgChWidth();}
  int      chHeight()                 {return dev.chHeight();}
  void     getMaxPos(long& maxX, long& maxY) {dev.getMaxPos(maxX, maxY);}

  DevBase& getDevDC(CDC*& dc) {dc = dev.getDC(); return dev;}

private:

  NtPdToDsply() : ToDevBase(*(NotePad*)0, *(DevBase*)0) { }
  };

