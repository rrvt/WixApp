// Wix file Output


#pragma once
#include "FileIO.h"
#include "IniFile.h"


class WixOut : protected FileIO {

public:

  WixOut() {}
 ~WixOut() {}

  bool open(String& path) {FileIO& f = *this; return f.open(path, Write);}
  void close()            {FileIO& f = *this;        f.close();}

  void lit(int tab, TCchar*  p) {String s = p; stg(tab, s);}
  void out(int tab, TCchar*  prefix, String& s, TCchar* suffix);
  void stg(int tab, String& se);

  void spaces(int n) {FileIO& f = *this; f.spaces(n);}
  void crlf()        {FileIO& f = *this; f.crlf();}


private:

  int count(String& line);
  };


extern WixOut  wix;                 // Wix Input File (output from this app)
extern IniFile wxd;                 // WixApp data file (contains description of installer)
