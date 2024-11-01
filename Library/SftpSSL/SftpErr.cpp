// Sftp Error class


#include "pch.h"
#include "SftpErr.h"
#include "MessageBox.h"


void SftpErr::wsa(TCchar* src) {
int    err = WSAGetLastError();
String s;

  getError(err, s);   last = src;   last += _T(" -- ") + s;   messageBox(last);
  }


void SftpErr::skt(TCchar* src, int rslt) {
String s;

  getError(rslt, s);  last = src;  last += _T(" -- ") + s;   messageBox(last);
  }
