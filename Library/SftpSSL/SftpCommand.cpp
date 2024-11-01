

#include "pch.h"
#include "SftpCommand.h"
#include "FileName.h"
#include "NotePad.h"
#include "OpenSSL\err.h"
#include "SftpDataIter.h"
#include "SftpErr.h"
#include "SftpTransport.h"
#include "SftpUtilities.h"
#include "SftpOps.h"
#include "Utilities.h"


SftpCommand::SftpCommand(SftpTransport& trnspt, SftpErr& e) :
          transport(trnspt), sftpOps(*(new SftpOps(e))), err(e), skt(sftpOps.skt) { }


SftpCommand::~SftpCommand() {WSACleanup();   delete &sftpOps;}


// Opens Socket to site

bool SftpCommand::open(TCchar* host) {

  if (!openSkt(host)) return false;

  return openSSL();
  }


void SftpCommand::closeAll() {sftpOps.closeAll();}


void SftpCommand::closeSSL(SSL* newSSL) {sftpOps.closeSSL(newSSL);}


void SftpCommand::closeSSL() {sftpOps.closeSSL();}


void SftpCommand::openSSLThreadStop() {OPENSSL_thread_stop();}


bool SftpCommand::openSkt(TCchar* host) {
String      web;
SockAddrIn4 sin;
char        lhost[1024];
char*       pos;

  memset(&sin, 0, sizeof(sin));   sin.sin_family = AF_INET;

  web = _T("ftp."); web += host;   ToAnsi h(web);

  strcpy_s(lhost, sizeof(lhost), h());    pos = strchr(lhost, ':');

  if (!sftpOps.getServerByName("ftp", sin)) return false;

  if (!sftpOps.inetAddr(lhost, sin.sin_addr.s_addr) &&
                                        !sftpOps.findHostIPaddr(lhost, sin.sin_addr.s_addr)) return false;
  if (!sftpOps.openSkt(_T("Command"))) return false;

  if (::connect(skt, (SockAddr*) &sin, sizeof(sin)))
                                              {err.wsa(_T("connect"));      closeSkt();   return false;}
  return true;
  }


void SftpCommand::closeSkt() {sftpOps.closeSkt();}


bool SftpCommand::openSSL() {
SSL* newSSL;

  closeSSL();

  newSSL = sftpOps.getNewSSL();  if (!newSSL) return false;

  if (!readRsp(220))            {closeSSL(newSSL); return false;}   // 220 - Service ready for new user.

  if (!sendCmd(_T("AUTH TLS"))) {closeSSL(newSSL); return false;}

  if (!readRsp(234))            {closeSSL(newSSL); return false;}   // 234 - Server accepts the security
                                                                    // mechanism specified by the client;
  switch (sftpOps.connect(newSSL)) {                                // no security data needs to be
    case SSLFail    : return false;                                 // exchanged.
    case SSLWantRead: readRsp(_T('2'));
    }

  return true;
  }


bool SftpCommand::login(TCchar* userId, TCchar* password) {

  if (!sendCmd(_T("USER"), userId) || !readRsp(331)) return false;  // 331 - User name okay, need password

  return sendCmd(_T("PASS"), password) && readRsp(230);             // 230 - User logged in, proceed.
  }


bool SftpCommand::avbl(String& avail) {
int i;

  for (i = 0; i < 5 && sendCmd(_T("AVBL"), 0, avail) && avail.find(_T("500")) != 0; i++) Sleep(10);

  return i < 5;
  }


bool SftpCommand::noop(String& rslt) {
int i;

  for (i = 0; i < 5 && sendCmd(_T("NOOP"), 0, rslt) && rslt.find(_T("200")) != 0; i++) Sleep(10);

  return i < 5;
  }


bool SftpCommand::stat(String& rslt) {
int i;

  for (i = 0; i < 5 && sendCmd(_T("STAT"), 0, rslt) && rslt.find(_T("211")) != 0; i++) Sleep(10);

  return i < 5;
  }


// return true and the current remote directory or false if it fails

bool SftpCommand::pwd(String& rslt) {
int    i;
String s;
int    pos;
int    pos2;

  for (i = 0; i < 5 && sendCmd(_T("PWD"), 0, s) && s.find(_T("257")) != 0; i++) Sleep(10);
  if (i >= 5) return false;

  pos = s.find(_T('"')) + 1;    pos2 = s.find(_T('"'), pos);

  rslt = s.substr(pos, pos2 - pos);   return true;
  }


// Change Working Directory

bool SftpCommand::cwd(TCchar* dir) {
int    i;
String s;

  for (i = 0; i < 5 && sendCmd(_T("CWD"), dir, s) && s.find(_T("250")) != 0; i++) Sleep(10);

  return i < 5;
  }


// Make Directory

bool SftpCommand::mkd(TCchar* dir) {
int    i;
String s;

  for (i = 0; i < 5 && sendCmd(_T("MKD"), dir, s) && s.find(_T("257")) != 0; i++) Sleep(10);

  return i < 5;
  }


// Remove Directory

bool SftpCommand::rmd( TCchar* dir) {
int    i;
String s;

  for (i = 0; i < 5 && sendCmd(_T("RMD"), dir, s) && s.find(_T("250")) != 0; i++) Sleep(10);

  return i < 5;
  }


// Remove Directory Tree

bool SftpCommand::rmda(TCchar* dir) {
int    i;
String s;

  for (i = 0; i < 5 && sendCmd(_T("RMDA"), dir, s) && s.find(_T("250")) != 0; i++) Sleep(10);

  return i < 5;
  }


// Returns true when the Unix like directory list (each line contains a line for each entity in the
// current directory.  The data is stored in the data store which can be read later.

bool SftpCommand::list(TCchar* path, TCchar* args) {
String cmd = _T("LIST ");   cmd += args;
bool   rslt;

  if (!sftpOps.setType(AsciiFlTyp)) return false;

  if (!initPassiveMode(cmd, path)) return false;

  rslt = transport.read(AsciiFlTyp);   transport.closeSkt();   return rslt;
  }


// Return true when file is deleted, else return false

bool SftpCommand::del(TCchar* webPath) {return sendCmd(_T("DELE"), webPath) && readRsp(250);}


// Return true when the size of the file is found, else return false

bool SftpCommand::size(TCchar* path, int& size) {
int  code;
uint pos;

  if (!sendCmd(_T("SIZE"), path) || !readRsp(213)) return false;        // 213 - File status

  code = sftpOps.lastResp.stoi(pos);

  if (code != 213 || sftpOps.lastResp[pos] != _T(' ')) return false;

  size = sftpOps.lastResp.substr(pos+1).stoi(pos);   return true;
  }


// return true when the date of the file is found, else return false

bool SftpCommand::date(TCchar* path, Date& val) {
int  code;
uint pos;

  if (!sendCmd(_T("MDTM"), path) || !readRsp(213)) return false;        // 213 - File status.

  code = sftpOps.lastResp.stoi(pos);

  if (code != 213 || sftpOps.lastResp[pos] != _T(' ')) return false;

  try {ToDate to(sftpOps.lastResp.substr(pos+1));    val = to();}
    catch (...) {notePad << _T("Failed Date Interpretation: ") << sftpOps.lastResp << nCrlf;
    }

  val.toLocalTime(val);   return true;
  }


// Copy sftpTransport store to web host file at webPath

bool SftpCommand::stor(TCchar* webPath) {
SSLFileType flTyp = sftpFileType(webPath);
bool        rslt;

  if (!sftpOps.setType(flTyp)) return false;

  if (!initPassiveMode(_T("STOR"), webPath)) return false;

  rslt = transport.write();   transport.closeSkt();   transport.clear();

  return rslt & readRsp(226);                         // 226 - Closing data connection. Requested file
  }                                                   // action successful


// copy transport buffer to unique file name in current direcotry

bool SftpCommand::stou(TCchar* webPath, String& fileName) {
SSLFileType flTyp = sftpFileType(webPath);
int         pos;
bool        rslt;

  if (!sftpOps.setType(flTyp)) return false;

  if (!initPassiveMode(_T("STOU"), 0)) return false;

  pos = sftpOps.firstResp.find(_T("FILE: "));

  if (pos >= 0) fileName = sftpOps.firstResp.substr(pos + 6);

  rslt = transport.write();   transport.closeSkt();   transport.clear();

  return rslt & readRsp(226);                         // 226 - Closing data connection. Requested file
  }                                                   // action successful


// Copy sftpTransport store to web host file at webPath

bool SftpCommand::append(TCchar* webPath) {
SSLFileType flTyp = sftpFileType(webPath);
bool        rslt;

  if (!sftpOps.setType(flTyp)) return false;

  if (!initPassiveMode(_T("APPE"), webPath)) return false;

  rslt = transport.write();   transport.closeSkt();   transport.clear();

  return rslt & readRsp(_T('2'));
  }


// copy file from web host to sftpTransport buffer

bool SftpCommand::retr(TCchar* webPath) {
SSLFileType flTyp = sftpFileType(webPath);
bool        rslt;

  transport.clear();

  if (!sftpOps.setType(flTyp)) return false;

  if (!initPassiveMode(_T("RETR"), webPath)) return false;

  rslt = transport.read(flTyp);   transport.closeSkt();   return rslt;
  }


bool SftpCommand::initPassiveMode(TCchar* cmd, TCchar* arg) {
int     pos;
String  s;
TCchar* cp;
int     v[6];
Sin     sin;

  if (!sendCmd(_T("PASV")) || !pasvRsp()) return false;

  pos = sftpOps.lastResp.find('(');    if (pos < 0) return false;

  s = sftpOps.lastResp.substr(pos+1);  cp = s.str();

  _stscanf_s(cp, _T("%u,%u,%u,%u,%u,%u"), &v[2], &v[3], &v[4], &v[5], &v[0], &v[1]);

  ZeroMemory(&sin, sizeof(sin) );
  sin.in.sin_family = AF_INET;

  sin.sa.sa_data[2] = char(v[2]);
  sin.sa.sa_data[3] = char(v[3]);
  sin.sa.sa_data[4] = char(v[4]);
  sin.sa.sa_data[5] = char(v[5]);
  sin.sa.sa_data[0] = char(v[0]);
  sin.sa.sa_data[1] = char(v[1]);

  if (!sendCmd(cmd, arg)) {err.put(_T("Passive Transport Command Failure"));  return false;}

  if (!transport.open(sin)) {err.put(_T("Passive Transport Connection Failed"));   return false;}

  return readRsp(150);                            // 150 - File status okay; about to open data connection
  }



bool SftpCommand::pasvRsp() {
int i;

  for (i = 0; i < 2; i++) {if (readRsp(227)) return true;   Sleep(100);} // 227 - Entering Passive Mode
                                                                         // (h1,h2,h3,h4,p1,p2).
  return false;
  }



bool SftpCommand::sendCmd(TCchar* cmd, TCchar* args, String& response)
                                                            {return sftpOps.sendCmd(cmd, args, response);}


bool SftpCommand::sendCmd(TCchar* cmd, TCchar* arg) {return sftpOps.sendCmd(cmd, arg);}


bool SftpCommand::readRsp(Tchar ch) {return sftpOps.readRsp(ch);}


bool SftpCommand::readRsp(int code) {return sftpOps.readRsp(code);}


bool SftpCommand::readRsp(String& rsp) {return sftpOps.readRsp(rsp);}


String& SftpCommand::lastResp() {return sftpOps.lastResp;}


