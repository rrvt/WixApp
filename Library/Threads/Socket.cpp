// Sockets


#include "pch.h"
#include "Socket.h"
#include "Sockets.h"



static Cchar* UnixPipeFile = "c:\\tmp\\my_unix_socket";


Socket::Socket() : family(-1), skt(INVALID_SOCKET), lastErr(0) {memset(&addr, 0, sizeof(addr));}


bool Socket::open(int domain) {

  if (skt != INVALID_SOCKET) return true;

  family = domain;

  switch (family) {
    case AF_UNIX: skt = socket(domain, SOCK_STREAM, 0); break;
    default     : lastErr = -1;   return false;
    }

  if (skt != INVALID_SOCKET) return true;

  lastErr = WSAGetLastError();   return false;
  }


bool Socket::bind() {

  addr.net.sa_family = family;

  switch (family) {
    case AF_UNIX: strcpy_s(addr.unix.sun_path, UnixPipeFile);   remove(UnixPipeFile);   break;
    default     : lastErr = -1;   return false;
    }

  if (!::bind(skt, &addr.net, sizeof(addr.unix))) return true;

  lastErr = WSAGetLastError();   return false;
  }


bool Socket::listener() {

  if (!listen(skt, SOMAXCONN)) return true;

  lastErr = WSAGetLastError();   return false;
  }


Socket* Socket::accept(Sockets& sockets) {
Socket* socket = sockets.create();

  if (socket->accept(skt)) return socket;

  lastErr = socket->lastErr;

  socket->close();   return 0;
  }


bool Socket::accept(SOCKET socket) {

  skt = ::accept(socket, 0, 0);   if (skt != INVALID_SOCKET) return true;

  lastErr = WSAGetLastError();   return false;

  }


bool Socket::connect() {

  addr.net.sa_family = family;

  switch (family) {
    case AF_UNIX: strcpy_s(addr.unix.sun_path, UnixPipeFile); break;
    default     : lastErr = -1;   return false;
    }

  if (!::connect(skt, &addr.net, sizeof(addr.unix))) return true;

  lastErr = WSAGetLastError();   return false;
  }


bool Socket::send(String& s) {
ToAnsi toAnsi(s);

#if 1
  return send(toAnsi(), s.length() + 1);
#else
  if (::send(skt, toAnsi(), s.length() + 1, 0) >= 0) return true;

  lastErr = WSAGetLastError();

  return false;
#endif
  }

// Send buffer of size n bytes

bool Socket::send(char* p, int n) {

  if (::send(skt, p, n, 0) >= 0) return true;

  lastErr = WSAGetLastError();   return false;

  }


int Socket::recv(char* p, int lng) {
int cnt = ::recv(skt, p, lng, MSG_WAITALL);

  if (cnt >= 0) return cnt;

  lastErr = WSAGetLastError();   return -1;
  }


void Socket::close() {
  if (skt    != INVALID_SOCKET) closesocket(skt);   skt    = INVALID_SOCKET;
  }



