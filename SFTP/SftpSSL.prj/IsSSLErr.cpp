


#include "pch.h"
#include "isSSLErr.h"
#include "SftpLog.h"



SSLRslt sslRslt(SSL* ssl, int e) {

  switch (SSL_get_error(ssl, e)) {
    case SSL_ERROR_NONE                 : return SSLSuccess;
    case SSL_ERROR_ZERO_RETURN          : sftpLog(_T("ZERO_RETURN"), e);          break;
    case SSL_ERROR_WANT_READ            : return SSLWantRead;
    case SSL_ERROR_WANT_WRITE           : sftpLog(_T("WANT_WRITE"), e);           break;
    case SSL_ERROR_WANT_ACCEPT          : sftpLog(_T("WANT_ACCEPT"), e);          break;
    case SSL_ERROR_WANT_CONNECT         : sftpLog(_T("WANT_CONNECT"), e);         break;
    case SSL_ERROR_WANT_X509_LOOKUP     : sftpLog(_T("WANT_X509_LOOKUP"), e);     break;
    case SSL_ERROR_WANT_ASYNC           : sftpLog(_T("WANT_ASYNC"), e);           break;
    case SSL_ERROR_WANT_ASYNC_JOB       : sftpLog(_T("WANT_ASYNC_JOB"), e);       break;
    case SSL_ERROR_WANT_CLIENT_HELLO_CB : sftpLog(_T("WANT_CLIENT_HELLO_CB"), e); break;
    case SSL_ERROR_SYSCALL              : sftpLog(_T("SYSCALL"), e);              break;
    case SSL_ERROR_SSL                  : sftpLog(_T("ERROR_SSL"), e);            break;
    default                             : sftpLog(_T("Unknown SSL Error"), e);    break;
    }

  return SSLFail;
  }



