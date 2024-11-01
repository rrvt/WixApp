// SSH FTP Interface


#include "pch.h"
#include "SftpSSL.h"
#include "SftpOps.h"


SftpSSL sftpSSL;


SftpSSL::SftpSSL() : cmd(transport, err), transport(err) {
int wsaErr = WSAStartup(MAKEWORD(2, 2), &wsa);                   // Required in Windows

  if (wsaErr) {err.skt(_T("WSAStartup() failed"), wsaErr);}
  }


SftpStore& SftpSSL::fileData() {return transport.sftpOps;}

