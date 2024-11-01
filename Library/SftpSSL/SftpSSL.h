// SSH FTP Interface


#pragma once
#include "SftpCommand.h"
#include "SftpErr.h"
#include "SftpTransport.h"


class SftpStore;


class SftpSSL {
WSADATA       wsa;

SftpCommand   cmd;
SftpTransport transport;

SftpErr       err;

public:

       SftpSSL();
      ~SftpSSL() { }

  // SFTP Commands

  bool open(TCchar* host)                      {return cmd.open(host);}
  bool login(TCchar* userId, TCchar* password) {return cmd.login(userId, password);}

//  bool readPending();

  bool avbl(String& avail) {return cmd.avbl(avail);}
  bool noop(String& rslt)  {return cmd.noop(rslt);}
  bool stat(String& rslt)  {return cmd.stat(rslt);}

  bool pwd( String& rslt)  {return cmd.pwd(rslt);}
  bool cwd( TCchar* dir)   {return cmd.cwd(dir);}         // Change Working Directory
  bool mkd( TCchar* dir)   {return cmd.mkd(dir);}         // Make Directory
  bool rmd( TCchar* dir)   {return cmd.rmd(dir);}         // Remove Directory
  bool rmda(TCchar* dir)   {return cmd.rmda(dir);}        // Remove Directory Tree

  bool list(TCchar* path, TCchar* args = _T("-l -a")) {return cmd.list(path, args);}
                                                          // Load directory for path into Transport
                                                          // Store
  bool size(TCchar* path, int&  size) {return cmd.size(path, size);}    // File size when true
  bool date(TCchar* path, Date& val)  {return cmd.date(path, val);}     // File last modified date
                                                                        // when true

  bool stor(TCchar* webPath) {return cmd.stor(webPath);}  // copy sftpTransport buffer to web host
                                                          // file
  bool stou(TCchar* webPath, String& fileName)            // copy transport buffer to unique file
                    {return cmd.stou(webPath, fileName);} // name in current directory

  bool append(TCchar* webPath) {return cmd.append(webPath);}  // Append sftpTransport buffer to web
                                                              // host file

  bool retr(TCchar* webPath) {return cmd.retr(webPath);}  // copy file from web host to
                                                          // sftpTransport buffer
  int  nBytes() {return transport.nBytes();}

  void store(Archive& ar) {transport.store(ar);}          // Store file in the sftpTransport buffer

  bool del(TCchar* webPath) {return cmd.del(webPath);}

  void closeAll()           {cmd.closeAll();  transport.closeAll();}
  void openSSLThreadStop()  {cmd.openSSLThreadStop();}

  bool sendCmd(TCchar* cmmd, TCchar* args, String& response)
                                                        {return cmd.sendCmd(cmmd, args, response);}
  bool initPassiveMode(TCchar* cmmd, TCchar* arg)       {return cmd.initPassiveMode(cmmd, arg);}

  // Transport Functions

  SftpTransport& sftpTransport()           {return transport;}
  bool           getLocalFile(TCchar* src) {return transport.load(src);}
  bool           putLocalFile(TCchar* dst) {return transport.store(dst);}
  SftpStore&     fileData();
  String&        lastResp()                {return cmd.lastResp();}
  };


extern SftpSSL sftpSSL;



//  void dspLines(String& s) {cmd.dspLines(s);}
//bool testStor(TCchar* webPath) {return cmd.testStor(webPath);}

