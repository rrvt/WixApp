// Sftp Data Iterator -- Iterate over data in an Sftp Store.


#include "pch.h"
#include "SftpDataIter.h"
#include "SftpStore.h"


// Logic for reading lines out of the buffers

String* SftpDataIter::operator() () {

  blk = iter();   if (!blk) return 0;

  p = blk->bfr;   endBlk = p + blk->n;   return (*this)++;
  }


String* SftpDataIter::operator++ (int) {
char ch;

  for (line.clear(); get(ch); ) {
    if (ch == '\n') return &line;

    line += Tchar(ch);
    }

  if (line.isEmpty()) return 0;

  return &line;
  }


bool SftpDataIter::get(char& ch) {

  if (!blk) return false;

  if (p >= endBlk) {
    blk = iter++;     if (!blk) return false;

    p = blk->bfr;   endBlk = p + blk->n;    return get(ch);
    }

  ch = *p++;   if (ch == '\r') return get(ch);

  return true;
  }

