// Archive File Interface


#pragma once
#include "FileIO.h"



class ArchFile {

bool   storing;
bool   opened;
FileIO fil;
void*  obj;

public:

           ArchFile(TCchar* fileName, int mode);
           ArchFile(void*   arbObj,   int mode);

          ~ArchFile() {fil.close();}

  void*    getObj()                   {return obj;}

  bool     isStoring()                {return storing;}
  bool     isLoading()                {return !storing;}
  bool     isOpen()                   {return opened;}


  String   getFilePath()              {String s;  if (isOpen()) s = fil.getFilePath();   return s;}

  void     seekEnd()                  {fil.seekEnd(); storing = true;}

  void     setTabSize(int nSpaces)    {fil.setTabSize(nSpaces);}         // Set tab size for output
  void     tab(       int nTab)       {fil.tab(nTab);}                   // Tab to nth tab position
  int      spaces(    int nSpaces)    {return fil.spaces(nSpaces);}

  bool     write(String&       s)     {return fil.write(s);}             // interprets \n or \r
  bool     write(TCchar*      ts)     {return fil.write(ts);}
  bool     write(Tchar        ch)     {return fil.write(ch);}
  void     crlf()                     {fil.crlf();}

  bool     read(String&            s) {return fil.read(s);}           // Interpretation of \n or \r
  bool     read(String& s,     int n) {return fil.read(s, n);}        // Read n chars into a string
  bool     read(Tchar&            ch) {return fil.read(ch);}

// One can also write binary data to files.  No interpretation of \n or \r

  bool     wrtBin(int           x)        {return ArchFile::wrtBin(&x, sizeof(x));}
  bool     wrtBin(ulong         x)        {return ArchFile::wrtBin(&x, sizeof(x));}
  bool     wrtBin(const void* blk, int n) {return fil.write((void*) blk, n);}
  bool     wrtBin(Byte          v)        {return fil.write(v);}

  bool     readBin(int&    x)             {int sz = sizeof(x); return ArchFile::readBin(&x, sz);}
  bool     readBin(ulong&  x)             {int sz = sizeof(x); return ArchFile::readBin(&x, sz);}
  bool     readBin(void* blk, int& n)     {return fil.read(blk, n);}
  bool     readBin(Byte&   v)             {return fil.read(v);}

  Tchar*   getLastError()             {return fil.getLastError();}    // Returns last error

private:

  void     setStoring(int mode);

  ArchFile() { }

  friend class Archive;
  };

