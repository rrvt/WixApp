// Replaces CArchive to allow for Unicode internally and ansi in external files
// Copyright Bob -- K6RWY, 2019


#pragma once
#include "FileIO.h"
#include "ManipT.h"

class CDoc;

class Archive;

typedef ManipT<Archive> ArchManip;



class Archive {

bool   opened;
bool   storing;
FileIO fil;
void*  obj;

public:

  Archive(String& fileName, int mode) : opened(false), storing(false), obj(0) {
    opened = fil.open(fileName, mode);

    if (mode != (FileIO::Read | FileIO::Write)) storing = mode & (FileIO::Create | FileIO::Write);

    initialize();
    }

  Archive(void* arbObj, int mode) : obj(arbObj)
                                    {opened = true;   storing = mode & (FileIO::Create | FileIO::Write);}

          ~Archive()                     {fil.close();}

  bool     isStoring()                   {return storing;}
  bool     isLoading()                   {return !storing;}

  void*    getObj()                      {return obj;}

  bool     isOpen()                      {return opened;}

  void     seekEnd()                     {fil.seekEnd(); storing = true;}

  bool     write(String&       s)        {return fil.write(s);}             // interprets \n or \r
  bool     write(TCchar*      ts)        {return fil.write(ts);}
  bool     write(Tchar        ch)        {return fil.write(ch);}

  bool     write(int           x)        {return write(&x, sizeof(x));}     // no interpretation of \n or
                                                                            // \r
  bool     write(const void* blk, int n) {return fil.write((void*) blk, n);}
  bool     write(Byte          v)        {return fil.write(v);}

  void     setTabSize(int nSpaces)       {fil.setTabSize(nSpaces);}        // Set tab size for output
  void     tab(       int nTabs)         {fil.tab(nTabs);}
  void     spaces(    int nSpaces)       {fil.spaces(nSpaces);}
  void     crlf()                        {fil.crlf();}

  Archive& operator<< (String&    s)     {write(s);  return *this;}
  Archive& operator<< (TCchar*    p)     {write(p);  return *this;}
  Archive& operator<< (Tchar     ch)     {write(ch); return *this;}
  Archive& operator<< (int        x)     {write(x);  return *this;}
  Archive& operator<< (ArchManip& m)     {return m.func(*this);}

  bool     read(String&        s)        {return fil.read(s);}             // Interpretation of \n or \r
  bool     read(String& s, int n)        {return fil.read(s, n);}          // Read n chars into a string
  bool     read(Tchar&        ch)        {return fil.read(ch);}

  bool     read(int&           x)        {return readBlk(&x, sizeof(x));}  // No interpretation of \n or
  int      readBlk(void* blk, int n)     {return fil.read(blk, n);}     // \r
  bool     read(Byte&          v)        {return fil.read(v);}

  Tchar*   getLastError()                {return fil.getLastError();}      // Returns last error

private:

  void initialize();
  static Archive& doCrlf(Archive& n) {n.crlf(); return n;}
  };


extern ArchManip aCrlf;       // add to stream to terminate a line on display: ar << "xyz" << nCrlf;

