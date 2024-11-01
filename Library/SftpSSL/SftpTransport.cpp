// Transport Data


#include "pch.h"
#include "SftpTransport.h"
#include "FileIO.h"
#include "NotePad.h"
#include "SftpDataIter.h"
#include "SftpErr.h"
#include "SftpOps.h"


typedef struct linger  Linger;


SftpTransport::SftpTransport(SftpErr& e) : sftpOps(*(new SftpOps(e))), err(e), skt(sftpOps.skt) { }
SftpTransport::~SftpTransport() {delete &sftpOps;}


void SftpTransport::clear() {sftpOps.clear();}


bool SftpTransport::open(Sin& sin) {
Linger lng = {0, 1000};

  if (!sftpOps.openSkt(_T("Transport"))) return false;

  if (setsockopt(skt, SOL_SOCKET, SO_LINGER, (Cchar*) &lng, sizeof(lng)) == -1)
                                  {err.wsa(_T("Transport Set Linger"));       closeSkt();   return false;}

  if (::connect(skt, &sin.sa, sizeof(sin.sa)))
                                  {err.wsa(_T("Transport Connect Failed"));   closeSkt();   return false;}
  return true;
  }


bool SftpTransport::read(SSLFileType flTyp) {

  if (!skt) return false;

  clear();    sftpOps.fileType = flTyp;

  return sftpOps.read();  ;
  }


// Compute the number of bytes in the store buffer

int  SftpTransport::nBytes() {
SftpStrIter iter(sftpOps);
SftpBlk*    blk;
int         n;

  for (n = 0, blk = iter(); blk; blk = iter++) n += blk->n;

  return n;
  }


bool SftpTransport::write() {
SftpStrIter iter(sftpOps);
SftpBlk*    blk;

  for (blk = iter(); blk; blk = iter++)
                                    if (blk->n > 0 && sftpOps.write(blk->bfr, blk->n) <= 0) return false;
  return true;
  }


// load store buffer from local file

bool SftpTransport::load(TCchar* path) {
FileIO   fio;
SftpBlk* blk;

  clear();

  if (!fio.open(path, FileIO::Read)) return false;

  for (blk = sftpOps.data.allocate(); blk; blk = sftpOps.data.allocate()) {
    blk->n = sizeof(FtpBfr);

    if (!fio.read(blk->bfr, blk->n)) {sftpOps.data.deallocate(blk); break;}

    sftpOps.data += blk;
    }

  fio.close();   return sftpOps.data.end() > 0;
  }


// stote store buffer in a local file

bool SftpTransport::store(TCchar* path) {
FileIO      fio;
SftpStrIter iter(sftpOps);
SftpBlk*    blk;
int         mode = FileIO::Write | FileIO::Create;
bool        rslt = true;

  if (!fio.open(path, (FileIO::OpenParms) mode)) return false;

  for (blk = iter(); blk; blk = iter++) {
    if (!fio.write(blk->bfr, blk->n)) {rslt = false; break;}
    }

  fio.close();   return rslt;
  }


// Store buffer content to a local file represented by archive

void SftpTransport::store(Archive& ar) {

  switch (sftpOps.fileType) {
    case AsciiFlTyp : storeAscii(ar); break;
    case ImageFlTyp : storeImage(ar); break;
    default         : String s = ar.getFilePath(); break;
    }
  }


void SftpTransport::storeAscii(Archive& ar) {
SftpDataIter ix(sftpOps);
String*      s;
String       crlf = _T("\r\n");

  for (s = ix(); s; s = ix++) {ar.write(*s);   ar.write(crlf);}
  }


void SftpTransport::storeImage(Archive& ar) {
SftpStrIter iter(sftpOps);
SftpBlk*    blk;

  for (blk = iter(); blk; blk = iter++)
    if (!ar.write(blk->bfr, blk->n)) break;
  }


void SftpTransport::closeSkt() {sftpOps.closeSkt();}


void SftpTransport::closeAll() {sftpOps.closeAll();}

