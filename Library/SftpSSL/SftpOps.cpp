// Primitive Operations and Data Storage for Sftp Operations


#include "pch.h"
#include "SftpOps.h"
#include "IsSSLErr.h"
#include "SftpDataIter.h"
#include "SftpErr.h"
#include <ws2tcpip.h>


typedef struct servent      ServerEnt;
typedef struct in_addr      InAddr;


bool SftpOps::getServerByName(Cchar* service, SockAddrIn4& sin) {
ServerEnt* pse = getservbyname(service, "tcp");

  if (!pse) {err.wsa(_T("getservbyname"));  return false;}

  sin.sin_port = pse->s_port;

  int x = pse->s_port;

  return true;
  }


bool SftpOps::inetAddr(Cchar* p, u_long& addr) {
InAddr inAddr;

  if (inet_pton(AF_INET, p, &inAddr) <= 0)  return false;

  addr = inAddr.S_un.S_addr;    return true;
  }


typedef struct addrinfo     AddrInfo;
typedef short               AddrFamily;
typedef struct sockaddr_in6 SockAddrIn6;


bool SftpOps::findHostIPaddr(Cchar* url, u_long& addr) {
AddrInfo     hints;
AddrInfo*    info;
char         stringBuf[46];
AddrFamily   family;
SockAddrIn4* sockaddr_ipv4;
SockAddrIn6* sockaddr_ipv6;
int          rslt;

  ZeroMemory( &hints, sizeof(hints) );
  hints.ai_family   = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  rslt = GetAddrInfoA(url, "http", &hints, &info);

  if (rslt) {
    ToUniCode uni(url);
    String    s = _T("findHostIPaddr: ");   s += uni();

    err.skt(s, rslt);  return false;
    }

  stringBuf[0] = 0;  family = info->ai_family;

  switch (family) {
    case AF_UNSPEC: err.put(_T("Family Unspecified"));   return false;

    case AF_INET  : sockaddr_ipv4 = (SockAddrIn4*) info->ai_addr;

                    addr = sockaddr_ipv4->sin_addr.s_addr; break;

    case AF_INET6 : sockaddr_ipv6 = (SockAddrIn6*) info->ai_addr;

                    sockaddr_ipv6->sin6_addr;

                    err.put(_T("Family: IP6 address"));  return false;

                    break;

    default       : err.put(_T("Gsmily: Other format")); return false;
    }

  return true;
  }


bool SftpOps::openSkt(TCchar* prfx) {
int on = 1;

  if (skt) closeSkt();

  skt = ::socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (skt == INVALID_SOCKET) {err.wsa(eMsg(prfx, _T("Socket")));   skt = 0;   return false;}

  if (setsockopt(skt, SOL_SOCKET, SO_REUSEADDR, (Cchar*) &on, sizeof(on)) == -1)
                                           {err.wsa(_T("Set Sock Option"));   closeSkt();   return false;}
  return true;
  }


void SftpOps::closeAll() {int cnt = 0;   closeSSL(cnt);   closeSkt();}


void SftpOps::closeSSL(SSL* newSSL) {if (ssl) closeSSL();   ssl = newSSL;   closeSSL();}


void SftpOps::closeSSL(int& cnt) {
int rslt;

  if (ssl) {

    rslt = SSL_shutdown(ssl);   cnt++;

    if (rslt == 0 && cnt < 2) {closeSSL(cnt);  return;}

    SSL_free(ssl);
    }

  if (ctx) SSL_CTX_free(ctx);

  ssl = 0;   ctx = 0;   sbio = 0;   clear();   lastOp = NilOp;
  }


void SftpOps::closeSkt() {

  if (!skt) return;

  if (lastOp == WriteOp) {if (shutdown(skt, SD_SEND)) err.wsa(_T("Shutdown"));   read();}

  closesocket(skt);   skt = 0;
  }


SSL* SftpOps::getNewSSL() {

  if (!skt) return 0;
  if (ssl)  return ssl;

  ctx = SSL_CTX_new(TLS_client_method());   SSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, NULL);

  SSL_CTX_set_mode(ctx, SSL_MODE_AUTO_RETRY);

  return SSL_new(ctx);
  }


SSLRslt SftpOps::connect(SSL* newSSL) {
int rslt;

  ssl = newSSL;

  sbio = BIO_new_socket(skt, BIO_NOCLOSE);

  SSL_set_bio(ssl, sbio, sbio);

  blkd(true);   rslt = SSL_connect(ssl);     if (rslt != 1) return sslRslt(ssl, rslt);

  return SSLSuccess;
  }


// Set the type of response from the server
// 200 - The requested action has been successfully completed

bool SftpOps::setType(SSLFileType flTyp) {return sendCmd(_T("TYPE"), sftpFileType(flTyp)) && readRsp(200);}



bool SftpOps::sendCmd(TCchar* cmd, TCchar* args, String& response) {
String s = cmd;  if (args) {s += _T(' ');   s += args;}

  if (!sendCmd(s)) return false;

  return readRsp(response);
  }


// The command and arguments are combined (provided the arg pointer is not zero) and the command is sent
// Clears the response buffer and returns result of send the command

bool SftpOps::sendCmd(TCchar* cmd, TCchar* arg) {
String s = cmd;    lastCmd = cmd;

  if (arg) {s += _T(' ');   s += arg;}   s += _T("\r\n");

  ToAnsi buf(s);   return write(buf(), buf.length()) == s.length();
  }


// Write the data in the buffer to the web host using the correct method

int SftpOps::write(Cchar* buf, int noBytes) {
int n;

  lastOp = WriteOp;

  if (ssl) {n = SSL_write(ssl, buf, noBytes);      if (sslRslt(ssl, n)) return n;}
  else     {n = send(     skt, buf, noBytes, 0);   if (n < 0)           return 0;}

  return n;
  }


bool SftpOps::readRsp(int code) {
SftpDataIter ix(*this);
String*      s;
uint         pos;
int          v;
bool         codeSeen;

  clrLast();   if (!read()) return false;

  for (codeSeen = false, s = ix(); s; s = ix++) {
    String t = *s;

    v = t.stoi(pos);   if (pos > 4) continue;

    if (v == code) {

      if (!codeSeen) {codeSeen |= true;   saveFirst(t);   lastCh = t[0];}

      saveLast(t);
      }
    }

  return codeSeen;
  }


bool SftpOps::readRsp(Tchar ch) {
SftpDataIter ix(*this);
String*      s;
ulong        v;
uint         nxt;
bool         rslt;

  clrLast();   if (!read()) return false;

  for (rslt = false, s = ix(); s; s = ix++) {
    String t = *s;

    v = t.stoul(nxt);    if (nxt > 4) continue;

    if (t[0] == ch) {

      if (!rslt) {rslt = true;   saveFirst(t);   lastCh = t[0];}

      saveLast(t);
      }
    }

  return rslt;
  }


bool SftpOps::readRsp(String& rsp) {
SftpDataIter ix(*this);
int          i;
String*      s;

  if (!read()) return false;

  for (i = 0, rsp.clear(), s = ix(); s; i++, s = ix++) rsp += *s + _T('\n');

  return true;
  }


bool SftpOps::read() {
int      i;
SftpBlk* blk;

  data.clear();   blkd(true);   lastOp = ReadOp;

  for (i = 0, blk = 0; ; i++) {
    blk = data.allocate();

    if (ssl) {lastReadRslt = readEncrpt(*blk);}
    else     {lastReadRslt = readClr(   *blk);}

    switch (lastReadRslt) {
      case SSLtrue    : data[i].p = blk; continue;
      case SSLtimeout :
      case SSLfalse   : if (blk->n) data[i].p = blk;
                        else        data.deallocate(blk);
                        return nData() > 0;
      }
    }
  }



SSLrslt SftpOps::readClr(SftpBlk& ftpBlk) {
SSLrslt r;                                                      // && SSL_has_pending(ssl)

  ftpBlk.n = ftpBlk.curX = 0;

  for (r = readSkt(ftpBlk); r == SSLtrue && ftpBlk.n < sizeof(FtpBfr); r = readSkt(ftpBlk))
                                                                              {blkd(false);   Sleep(100);}

  return ftpBlk.n == sizeof(FtpBfr) ? SSLtrue : r;
  }


SSLrslt SftpOps::readSkt(SftpBlk& blk) {
char* bfr  = blk.bfr;
int   size = sizeof(FtpBfr);
int   n;

  setTimeout(5);

  n = recv(skt, bfr + blk.n, size - blk.n, 0);

  if (!n)       return SSLfalse;

  if (n < 0)    return SSLtimeout;

  blk.n += n;   return SSLtrue;
  }



//bool SftpOps::readPending() {return ssl && SSL_has_pending(ssl);}


SSLrslt SftpOps::readEncrpt(SftpBlk& ftpBlk) {
SSLrslt r;                                                      // && SSL_has_pending(ssl)

  for (r = readSSL(ftpBlk); r == SSLtrue && ftpBlk.n < sizeof(FtpBfr);
                                                            r = readSSL(ftpBlk)) {blkd(false); Sleep(10);}
  return ftpBlk.n == sizeof(FtpBfr) ? SSLtrue : r;
  }


SSLrslt SftpOps::readSSL(SftpBlk& blk) {
char*  bfr  = blk.bfr;
int    size = sizeof(FtpBfr);
size_t n;
int    rslt;

  setTimeout(5);

  rslt = SSL_read_ex(ssl, bfr + blk.n, size - blk.n, &n);    if (rslt) {blk.n += n;  return SSLtrue;}

  if (SSL_get_error(ssl, rslt) == SSL_ERROR_SYSCALL && WSAGetLastError() == WSAETIMEDOUT)
                                                                                        return SSLtimeout;
  return SSLfalse;
  }


bool SftpOps::setTimeout(int noSecs) {
struct timeval tv   = {noSecs * 1000, 0};

  if (setsockopt(skt, SOL_SOCKET, SO_RCVTIMEO, (Cchar*) &tv, sizeof(tv)) == -1)
                                          {err.wsa(_T("Set Sock Timeout"));   closeSkt();   return false;}
  return true;
  }



// enables or disables the blocking mode for the
// socket based on the numerical value of iMode.
// If iMode = 0, blocking is enabled;
// If iMode != 0, non-blocking mode is enabled.
/*
  iResult = ioctlsocket(m_socket, FIONBIO, &iMode);
  if (iResult != NO_ERROR) printf("ioctlsocket failed with error: %ld\n", iResult);
*/

bool SftpOps::blkd(bool blocking) {
ulong arg = blocking ? 0 : 1;

  if (!skt) return false;

  if (!ioctlsocket(skt, FIONBIO, &arg)) return true;

  err.wsa(_T("ioctlsocket failed with error")); return false;
  }



