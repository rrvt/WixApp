// Sockets

/*
#include <iostream>
#include <winsock2.h>
#include <afunix.h> // For AF_UNIX
#include <string>

#pragma comment(lib, "ws2_32.lib") // Link with Winsock library

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed.\n";
        return 1;
    }

    SOCKET server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) {
        std::cerr << "Error creating socket: " << WSAGetLastError() << "\n";
        WSACleanup();
        return 1;
    }

    sockaddr_un server_addr;
    server_addr.sun_family = AF_UNIX;
    strcpy_s(server_addr.sun_path, sizeof(server_addr.sun_path), "my_unix_socket"); // Path to the socket file

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed: " << WSAGetLastError() << "\n";
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    if (listen(server_fd, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed: " << WSAGetLastError() << "\n";
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    std::cout << "Server listening on 'my_unix_socket'\n";

    SOCKET client_fd = accept(server_fd, NULL, NULL);
    if (client_fd == INVALID_SOCKET) {
        std::cerr << "Accept failed: " << WSAGetLastError() << "\n";
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    std::cout << "Client connected.\n";

    char buffer[1024];
    int bytes_received;

    while ((bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes_received] = '\0'; // Null-terminate the received data
        std::cout << "Received: " << buffer << "\n";
    }

    if (bytes_received == 0) {
        std::cout << "Client disconnected gracefully.\n";
    } else if (bytes_received == SOCKET_ERROR) {
        std::cerr << "Receive failed: " << WSAGetLastError() << "\n";
    }

    closesocket(client_fd);
    closesocket(server_fd);
    WSACleanup();

    // Clean up the socket file
    remove("my_unix_socket");

    return 0;
}*/

#pragma once

class Sockets;


typedef union {
SOCKADDR_UN unix;
SOCKADDR    net;
} Addr;


class Socket {
ushort   family;
Addr     addr;
SOCKET   skt;
int      lastErr;
public:

          Socket();
         ~Socket() {close();}

  bool    open(int domain);
  bool    bind();
  bool    listener();
  Socket* accept(Sockets& sockets);                   // Returns a socket for receiving input
  bool    connect();
  void    close();

  bool    send(String& s);
  bool    send(char* p, int n);        // Send buffer of size n bytes
  int     recv(char* p, int lng);

private:

  bool    accept(SOCKET socket);
  };



