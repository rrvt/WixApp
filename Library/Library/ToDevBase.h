// Display DevBase Data


#pragma once
#include "DevDsply.h"
#include "DevPrntr.h"
#include "NotePad.h"


class ToDevBase {
protected:

NtPdIter npIter;
Note*    note;
DevBase& dev;

bool     endDoc;
bool     debugging;

public:

uint     lastPageNo;

                   ToDevBase(NotePad& np, DevBase& device) : npIter(np), dev(device), note(0),
                                                        endDoc(false), debugging(false), lastPageNo(0) { }
                  ~ToDevBase() { }

  void             clearOps() {dev.clearOps();}
  void             startDev() {note = npIter(); lastPageNo = 0; endDoc = debugging = false;}
  void             operator() ();

  void             clear()                                {dev.clear();}
  void             resetDoc() {endDoc = false;}

  void             setHorzMgns(double left, double right) {dev.setHorzMgns(left, right);}
  void             setVertMgns(double top,  double bot)   {dev.setVertMgns(top, bot);}

  double           getFontScale()                  {return dev.getFontScale();}
  void             setFontScale(double scl)               {dev.setFontScale(scl);}

  DevTabs&         getTabs()         {return dev.getTabs();}
  void             restoreTabs(DevTabs& tm) {dev.restoreTabs(tm);}

          DevBase& getDev() {return dev;}                             // Useful for producing a footer

  virtual DevBase& getDevDC(CDC*& dc) {dc = dev.getDC(); return dev;}

private:

  ToDevBase() : npIter(*(NotePad*)0), dev(*(DevBase*)0) { }
  };


