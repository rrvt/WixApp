// Sftp Error class

#pragma once


class SftpErr {

String last;

public:

  SftpErr() { }
 ~SftpErr() { }

  void    wsa(TCchar* src);
  void    skt(TCchar* src, int rslt);

//  bool    ssl(void* ssl, int rslt);                    // Report if rslt is an ssl error


  void    put(TCchar* err) {last = err;}
  String& get()            {return last;}
  };




