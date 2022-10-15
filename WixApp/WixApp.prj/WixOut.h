// Wix file Output


#pragma once
#include "FileIO.h"
#include "IniFile.h"


class WixOut : protected FileIO {

int curTab;

public:

  WixOut() : curTab(0) {}
 ~WixOut() {}

  bool open(String& path) {FileIO& f = *this; return f.open(path, Write);}
  void close()            {FileIO& f = *this;        f.close();}

  void operator() (int     n)    {curTab = n;}                    // Set tab in spaces
  void operator() (TCchar* tc);                                   // Output tc after the current tab
  void operator() (TCchar* prefix, TCchar* tc, TCchar* suffix);   // output line at current tab if tc
                                                                  // not empty
  void crlf(int n = 1);                                           // Output carriage return/line feed
  };


extern WixOut  wix;                 // Wix Input File (output from this app)
extern IniFile wxd;                 // WixApp data file (contains description of installer)





//  void lit(int tab, TCchar* p) {String s = p; stg(tab, s);}
//  void stg(TCchar* tc);
//  void out(TCchar* prefix, TCchar* tc, TCchar* suffix);
//  void spaces(int n) {FileIO& f = *this; f.spaces(n);}

 // int count(String& line);
// {out(prefix, tc, suffix);}
