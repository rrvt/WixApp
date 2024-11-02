// Store for data received from server


#pragma once
#include "ExpandableP.h"
#include "IterT.h"
#include "SftpUtilities.h"



typedef char FtpBfr[1024];


class SftpBlk {
public:

int    curX;                              // Current Index
int    n;                                 // Number of bytes stored in buffer
FtpBfr bfr;

  SftpBlk() : curX(0), n(0) {bfr[0] = 0;}
  SftpBlk(SftpBlk& b) {copy(b);}
 ~SftpBlk() { }

  SftpBlk& operator= (SftpBlk& b) {copy(b); return *this;}

private:

  void copy(SftpBlk& b) {curX = b.curX; n = b.n; memcpy(bfr, b.bfr, sizeof(FtpBfr));}
  };


class SftpStore;
typedef DatumPtrT<SftpBlk, int> SftpBlkP;            // Usually defined just before iterator
typedef IterT<SftpStore, SftpBlk> SftpStrIter;  // Iterator over array of Datums


class SftpStore {
protected:

SSLFileType                       fileType;
ExpandableP<SftpBlk, int, SftpBlkP, 2> data;

public:

  SftpStore() : fileType(NilFlTyp) { }
 ~SftpStore() {clear();}

  void clear() {data.clear();   fileType = NilFlTyp;}

  int  nData() {return data.end();}     // returns number of data items in array

private:

  // returns either a pointer to data (or datum) at index i in array or zero

  SftpBlk* datum(int i) {return 0 <= i && i < nData() ? data[i].p : 0;}

  void  removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

  friend typename SftpStrIter;
  };

