// Wix file Output


#pragma once
#include "FileIO.h"
#include "IniFile.h"


const int BreakBeforePos = 84;
const int MaxLineLength  = 99;
const int TabVal         = 2;


class WixOut : protected FileIO {

int spcs;                                                     // Spaces at beginning of line

public:

  WixOut() : spcs(0) {}
 ~WixOut() {}

  bool open(String& path) {FileIO& f = *this; return f.open(path, Write);}
  void close()            {FileIO& f = *this;        f.close();}

  void operator() (TCchar* tc);                               // Output tc after the current tab
  void operator() (int nSpaces, TCchar* tc);                  // Optput one tabbed line

  // output line at current tab if tc not empty
  void operator() (int nSpaces, TCchar* prefix, TCchar* tc, TCchar* suffix);

  void crlf(int n = 1);                                       // Output carriage return/line feed

private:

  bool findBrk(int tab, String& line, String& line2, int brkPt);
  void blkOut(int tab, TCchar* tc, bool eol = true);
  };


extern WixOut  wix;                 // Wix Input File (output from this app)
extern IniFile wxd;                 // WixApp data file (contains description of installer)



