// Debug Anonymous Block of Data by saving in various formats to a file


#pragma once
#include "FileIo.h"

class DebugBlk {
FileIO fio;

public:

  DebugBlk() { }
 ~DebugBlk() {fio.close();}

  bool open(TCchar* name) {return fio.open(name, FileIO::Write | FileIO::Create);}

  void label(TCchar* txt) {fio.write(txt);   fio.write(_T(": "));}
  void txt(TCchar* txt)   {fio.write(txt);}
  void hex(void* blk, int n);
  void crlf() {fio.crlf();}

  void close() {fio.close();}
  };


extern DebugBlk dbgBlk;
