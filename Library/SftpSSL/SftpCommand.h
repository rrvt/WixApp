
#pragma once
#include "Date.h"
#include "SftpUtilities.h"


class SftpTransport;
class SftpOps;
class SftpErr;
typedef struct ssl_st SSL;


class SftpCommand {

SftpTransport& transport;

SftpOps&       sftpOps;                               // Allocated during construction
SftpErr&       err;
Socket&        skt;                                   // Windows Socket, linked to allocated sftpOps

public:

          SftpCommand(SftpTransport& trnspt, SftpErr& e);
         ~SftpCommand();

  bool    open(TCchar* host);                           // Opens Socket to host site

  bool    login(TCchar* userId, TCchar* password);

  bool    avbl(String& avail);
  bool    noop(String& rslt);
  bool    stat(String& rslt);

  bool    readPending();

  bool    pwd( String& rslt);                           // Return the response string(s)
  bool    cwd( TCchar* dir);                            // Change Working Directory
  bool    mkd( TCchar* dir);                            // Make Directory
  bool    rmd( TCchar* dir);                            // Remove Directory
  bool    rmda(TCchar* dir);                            // Remove Directory Tree

  bool    list( TCchar* path, TCchar* args);            // List directory or file for path into Store
  bool    size( TCchar* path,    int& size);            // File size when true
  bool    date( TCchar* path,   Date& val);             // File last modified date when true

  bool    stor(TCchar* webPath);                        // copy sftpTransport buffer to web host file
  bool    stou(TCchar* webPath, String& fileName);      // copy transport buffer to unique file name
                                                        // in current direcotry
  bool    append(TCchar* webPath);                      // Append sftpTransport buffer to web host file
  bool    retr(TCchar* webPath);                        // copy file from web host to sftpTransport buffer
  bool    del(TCchar* webPath);

  bool    sendCmd(TCchar* cmd, TCchar* args, String& response);
  bool    initPassiveMode(TCchar* cmd, TCchar* arg);

  void    closeAll();
  void    closeSSL(SSL* newSSL);
  void    closeSSL();
  void    openSSLThreadStop();

  String& lastResp();

private:

  bool    openSkt(TCchar* host);
  bool    openSSL();                                    // Opens SftpOps connection to site
  void    closeSkt();

  bool    sendCmd(TCchar* cmd, TCchar* arg = 0);

  bool    readRsp(Tchar ch);
  bool    readRsp(int code);
  bool    readRsp(String& rsp);

  bool    pasvRsp();

  SftpCommand() : transport(*(SftpTransport*)0), sftpOps(*(SftpOps*)0), err(*(SftpErr*)0),
                                                                                      skt(*(Socket*)0) { }

  friend class SftpSSL;
  };


