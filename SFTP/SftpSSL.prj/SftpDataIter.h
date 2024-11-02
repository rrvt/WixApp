// Sftp Data Iterator -- Iterate over data in an Sftp Store.


#pragma once
#include "SftpStore.h"

class SftpBlk;


class SftpDataIter {
SftpStore&  store;
SftpStrIter iter;
SftpBlk*    blk;
char*       p;
char*       endBlk;
String      line;

public:

  SftpDataIter(SftpStore& str) : store(str), iter(str), blk(0), p(0), endBlk(0) { }
 ~SftpDataIter() { }

  String* operator() ();
  String* operator++ (int);

private:

  bool    get(char&  ch);

  void    setNBlks();
  void    setCurBlk();

  SftpDataIter() : store(*(SftpStore*)0), iter(*(SftpStore*)0) { }
  };

