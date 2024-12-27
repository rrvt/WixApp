// Display Device Data


#pragma once
#include "Devices.h"
#include "DevStream.h"
#include "NotePad.h"


class NtPdToDev {

bool      printDev;

protected:

NtPdIter  npIter;
Note*     note;

DevStream devStrm;
Device&   dev;

bool      endDoc;
bool      debugging;

public:

uint      lastPageNo;

          NtPdToDev(NotePad& np, DeviceID id);
         ~NtPdToDev() { }

  void    prepare(CDC* dc, CPrintInfo* pInfo = 0);
  void    initFont(TCchar* face, double fontSize) {dev.initFont(face, fontSize);}
  void    setWrapEnable(bool enbld)               {dev.setWrapEnable(enbld);}

  void    startDev() {note = npIter(); lastPageNo = 0; endDoc = debugging = false;}
  void    operator() ();

  void    clear()                                {dev.clear();}
  void    resetDoc()                             {endDoc = false;}

  void    setHorzMgns(double left, double right) {dev.setHorzMgns(left, right);}
  void    setVertMgns(double top,  double bot)   {dev.setVertMgns(top,  bot);}

  double  getFontScale()                  {return dev.getFontScale();}
  void    setFontScale(double scl)               {dev.setFontScale(scl);}

  int     getCharWidth()                    {return dev.getCharWidth();}
  int     chHeight()                        {return dev.chHeight();}
  void    getMaxPos(long& maxX, long& maxY) {dev.getMaxPos(maxX, maxY);}


  bool    isEndDoc()   {return endDoc && dev.isWrapFin();}

  void    allowOutput(  bool allow) {dev.allowOutput(allow);}

  bool    doEndPageChk() {return dev.doEndPageChk();}

  Device& getDev() {return dev;} // Useful for producing a

private:

  NtPdToDev() : npIter(*(NotePad*)0), devStrm(NilDevID), dev(*(Device*)0) { }
  };


