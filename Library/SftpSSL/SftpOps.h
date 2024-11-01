// Primitive Operations and Data Storage for Sftp Operations


#pragma once
#include <openssl/ssl.h>
#include "SftpErr.h"
#include "SftpStore.h"


typedef struct sockaddr_in SockAddrIn4;
enum                       LastOp {NilOp, ReadOp, WriteOp};


class SftpOps : public SftpStore {

SftpErr& err;

protected:

Socket   skt;                             // Windows Socket

// OpenSSL Data
SSL*     ssl;                             // The OpenSSL library objects needed for
SSL_CTX* ctx;                             // encrypted communication
BIO*     sbio;

// Store Data
LastOp   lastOp;                          // Last Operation
String   lastCmd;                         // Last Command Sent
String   firstResp;                       // First Response String
String   lastResp;                        // Last Response String
Tchar    lastCh;                          // Last initial char of Response String
SSLrslt  lastReadRslt;                    // Last read() result

public:
          SftpOps(SftpErr& errHndl) :
                  err(errHndl), skt(0), ssl(0), ctx(0), sbio(0), lastOp(NilOp), lastReadRslt(SSLfalse) { }
         ~SftpOps() {closeAll();}

  void    clear() {clrLast(); SftpStore::clear();}

  bool    openSkt(TCchar* prfx);

  SSL*    getNewSSL();
  SSLRslt connect(SSL* newSSL);


  bool    sendCmd(TCchar* cmd, TCchar* arg = 0);
  bool    sendCmd(TCchar* cmd, TCchar* args, String& response);

  int     write(Cchar* buf, int noBytes);

  bool    setType(SSLFileType flTyp);

  bool    readRsp(int     code);
  bool    readRsp(Tchar   ch);
  bool    readRsp(String& rsp);

  bool    read();
  SSLrslt getLastReadRslt() {return lastReadRslt;}

  void    closeAll();
  void    closeSkt();
  void    closeSSL(SSL* newSSL);
  void    closeSSL() {int cnt = 0; closeSSL(cnt);}
  void    closeSSL(int& cnt);

protected:

  bool    getServerByName(Cchar* service, SockAddrIn4& sin);
  bool    inetAddr(Cchar* p, u_long& addr);
  bool    findHostIPaddr(Cchar* url, u_long& addr);

  void    clrLast()             {lastOp = NilOp; firstResp.clear(); lastResp.clear(); lastCh = 0; }
  void    saveFirst(TCchar* tc) {firstResp = tc;}
  void    saveLast(TCchar* tc)  {lastResp  = tc;}

private:

  SSLrslt readClr(   SftpBlk& ftpBlk);
  SSLrslt readSkt(   SftpBlk& blk);

  SSLrslt readEncrpt(SftpBlk& ftpBlk);
  bool    blkd(bool blocking);

//  bool    readBlkd(  SftpBlk& ftpBlk);
//  bool    readUnblkd(SftpBlk& ftpBlk);

  SSLrslt readSSL(SftpBlk& blk);

  bool    setTimeout(int noSecs);

  TCchar* eMsg(TCchar* prfx, TCchar* err)
                                         {static String s = prfx;   s += _T(' ');   s += err;   return s;}

          SftpOps() : err(*(SftpErr*)0) { }

  friend class SftpTransport;
  friend class SftpCommand;
  };




//bool    oldReadEncrpt(SftpBlk& ftpBlk);
//bool    oldReadSSL(SftpBlk& blk);
//bool    oldRead();
//bool    testReadSSL(SftpBlk& blk);
//bool    testReadEncrpt(SftpBlk& ftpBlk);
//void    testRead();
//  bool    readPending();
//  void    setFileType(Tchar ch) {fileType = ch;}

