// Sockets -- Collect sockets in one place for convenience


#pragma once
#include "pch.h"
#include "Sockets.h"
#include "Socket.h"


Sockets::Sockets() {WSADATA wsaData;   startup = !WSAStartup(MAKEWORD(2, 2), &wsaData);}


Sockets::~Sockets() {data.clear();     WSACleanup();}


void Sockets::clear() {
SktIter iter(*this);
Socket* socket;

  for (socket = iter(); socket; socket = iter++) socket->close();
  }


Socket* Sockets::create() {Socket* socket = data.allocate();   data += socket;  return socket;}


bool Sockets::open(Socket* socket, int domain) {return socket ? socket->open(domain) : false;}


