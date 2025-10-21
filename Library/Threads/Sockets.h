// Sockets -- Collect sockets in one place for convenience



#pragma once
#include "ExpandableP.h"
#include "IterT.h"

class Socket;


class Sockets;
typedef DatumPtrT<Socket, SOCKET> SocketP;
typedef ExpandableP<Socket, SOCKET, SocketP, 2> SktData;
typedef IterT<Sockets, Socket> SktIter;


class Sockets {
bool    startup;
SktData data;

public:

  Sockets();
 ~Sockets();

  void    clear();

  Socket* create();
  bool    open(Socket* socket, int domain);

private:

  // returns either a pointer to data (or datum) at index i in array or zero

  Socket* datum(int i) {return 0 <= i && i < nData() ? data[i] : 0;}

  int     nData()      {return data.end();}                // returns number of data items in array
                                                          // not necessarily private
  void    removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

  friend typename SktIter;
  };


