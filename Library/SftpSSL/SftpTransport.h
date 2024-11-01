// Transport Data


#pragma once
#include "SftpUtilities.h"

class SftpOps;
class SftpErr;
class Archive;


typedef union {
struct sockaddr    sa;
struct sockaddr_in in;
} Sin;


class SftpTransport {

SftpOps& sftpOps;
SftpErr& err;

Socket&  skt;                             // Windows Socket

public:

  SftpTransport(SftpErr& e);
 ~SftpTransport();

  void clear();

  bool open(Sin& sin);

  bool read(SSLFileType flTyp);             // Read everything from web host into store

  int  nBytes();                            // Compute the number of bytes in the store buffer

  bool write();                             // Write everything in store to web host

  bool load(TCchar* path);                  // load store buffer from local file
  bool store(TCchar* path);                 // store store buffer in a local file
  void store(Archive& ar);                  // Store buffer content to a local file represented by archive

  void closeSkt();
  void closeAll();

private:

  void storeAscii(Archive& ar);
  void storeImage(Archive& ar);

  SftpTransport() : sftpOps(*(SftpOps*)0), err(*(SftpErr*)0), skt(*(Socket*)0) { }

  friend class SftpSSL;
  };


